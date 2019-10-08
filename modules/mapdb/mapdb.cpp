#include <fstream>
#include <sstream>
#include <stdint.h>
#include <cstring>
#include <string>

#include "mapdb.h"
#include "mp/mp.h"
#include "string_pack.h"

#include <sys/types.h> // FolderMaker
#include <sys/stat.h>

// key names in the INF database
#define INF_KEY_NAME 1
#define INF_KEY_BRD  2

using namespace std;

/**********************************************************/
// pack object to a string (for DB storage)
string
MapDBObj::pack() const {
  ostringstream s;

  // two integer numbers: flags, type are packed in a single 32-bit integer:
  uint32_t v = (cl << 16) | type;
  s.write((char *)&v, sizeof(uint32_t));

  // optional angle (integer value, 1/1000 degrees)
  if (angle!=0) string_pack<int32_t>(s, "angl", (int32_t)(angle*1000));

  // optional direction (int value)
  if (dir!=MAPDB_DIR_NO) string_pack<uint32_t>(s, "dir ", (uint32_t)dir);

  // optional text fields (4-byte tag, 4-byte length, data);
  if (name!="") string_pack_str(s, "name", name);
  if (comm!="") string_pack_str(s, "comm", comm);

  // tags
  for (auto const & t: tags)
    string_pack_str(s, "tags", t);

  // coordinates
  string_pack_crds(s, "crds,", *this);

  return s.str();
}

// unpack object from a string (for DB storage)
void
MapDBObj::unpack(const std::string & str) {

  // re-initialize
  *this = MapDBObj();

  istringstream s(str);

  // type + class
  uint32_t v;
  s.read((char*)&v, sizeof(int32_t));

  type = v & 0xFFFF;
  v >>= 16;
  if (v<0 || v>2) throw Err() << "MapDBObj::unpack: bad class value: " << v;
  cl   = (MapDBObjClass)v;

  // other fields
  while (1){
    string tag = string_unpack_tag(s);
    if (tag == "") break;
    else if (tag == "dir ") dir = (MapDBObjDir)string_unpack<uint32_t>(s);
    else if (tag == "angl") angle = string_unpack<int32_t>(s)/1000.0;
    else if (tag == "name") name  = string_unpack_str(s);
    else if (tag == "comm") comm  = string_unpack_str(s);
    else if (tag == "tags") tags.insert(string_unpack_str(s));
    else if (tag == "crds") push_back(string_unpack_crds(s));
    else throw Err() << "Unknown tag: " << tag;
  }

}
/**********************************************************/
MapDB::MapDB(std::string name, bool create):
    folder(name, create),
    mapinfo(name + "/mapinfo.db", NULL, create, false),
    objects(name + "/objects.db", NULL, create, false),
    labels(name  + "/labels.db",  NULL, create, true),
    geohash(name + "/geohash.db", NULL, create){

  // get map version
  uint32_t key = 0;
  std::string vstr = mapinfo.get(key);

  // set version if it is not set
  if (key == 0xFFFFFFFF){
    vstr = type_to_str<int>(MAPDB_VERSION);
    mapinfo.put(0, vstr);
  }
  // parse version and write to map_version variable
  map_version = str_to_type<int>(vstr);

  // some tests
  if (map_version > MAPDB_VERSION)
    throw Err() << "MapDB version is too new, update mapsoft:" << map_version;
};


/**********************************************************/

MapDB::FolderMaker::FolderMaker(std::string name, bool create){
  struct stat info;
  if (stat(name.c_str(), &info ) != 0 ){
    if (create){
      if (mkdir(name.c_str(),0755) != 0)
        throw Err() << "Can't create MapDB folder: " << name;
    }
    else throw Err() << "Can't find MapDB folder: " << name;
  }
  else if( !(info.st_mode & S_IFDIR))
    throw Err() << "Not a MapDB folder: " << name;
}

/**********************************************************/

std::string
MapDB::get_map_name() {
  dMultiLine ret;
  uint32_t key = INF_KEY_NAME;
  return mapinfo.get(key);
}

/// Set map name
void
MapDB::set_map_name(const std::string & name) {
  mapinfo.put(INF_KEY_NAME, name);
}

dMultiLine
MapDB::get_map_brd() {
  dMultiLine ret;
  uint32_t key = INF_KEY_BRD;
  istringstream s(mapinfo.get(key));
  if (key == 0xFFFFFFFF) return ret;

  // searching for crds tags
  while (1){
    string tag = string_unpack_tag(s);
    if (tag == "") break;
    else if (tag == "crds") ret.push_back(string_unpack_crds(s));
    else throw Err() << "MapDB::get_map_brd: unknown tag: [" << tag << "]";
  }
  return ret;
}

void
MapDB::set_map_brd(const dMultiLine & b) {
  ostringstream s;
  string_pack_crds(s, "crds", b);
  mapinfo.put(INF_KEY_BRD, s.str());
}

/**********************************************************/
uint32_t
MapDB::add(const MapDBObj & o){
  // get last id + 1
  uint32_t id;
  uint32_t ct = (o.cl << 16) | o.type;
  objects.get_last(id);
  if (id == 0xFFFFFFFF) id=0;
  else id++;

  if (id == 0xFFFFFFFF)
    throw Err() << "MapDB::add: object ID overfull";

  // write object
  objects.put(id, o.pack());

  // write geohash
  geohash.put(id, ct, o.bbox());

  return id;
}

void
MapDB::put(uint32_t id, const MapDBObj & o){

  // get old object
  std::string str = objects.get(id);
  if (id == 0xFFFFFFFF) return;

  MapDBObj o1;
  o1.unpack(str);
  uint32_t ct1 = (o1.cl << 16) | o1.type;
  uint32_t ct  = (o.cl  << 16) | o.type;

  // Delete geohashes
  geohash.del(id, ct1, o1.bbox());

  // write new object
  objects.put(id, o.pack());

  // write geohash
  geohash.put(id, ct, o.bbox());

}

MapDBObj
MapDB::get(uint32_t id){
  uint32_t id1 = id;
  std::string str = objects.get(id1);
  if (id1 == 0xFFFFFFFF)
    throw Err() << "MapDB::get: no such object: " << id;
  MapDBObj ret;
  ret.unpack(str);
  return ret;
}


void
MapDB::del(uint32_t id){

  // get old object
  std::string str = objects.get(id);
  if (id == 0xFFFFFFFF) return;

  MapDBObj o;
  o.unpack(str);
  uint32_t ct  = (o.cl  << 16) | o.type;

  // Delete heohashes
  geohash.del(id, ct, o.bbox());

  // Delete the object
  objects.del(id);
}

std::set<int>
MapDB::find(MapDBObjClass cl, int type, const dRect & range){
  uint32_t ct  = (cl  << 16) | type;
  return geohash.get(ct, range);
}
