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
#define INF_KEY_BBOX 3

using namespace std;

/**********************************************************/
// pack object to a string (for DB storage)
string
MapDBObj::pack(bool write_bbox) const {
  ostringstream s;

  // two integer numbers: class, type:
  int32_t v;
  v = (int32_t)cl;   s.write((char *)&v, sizeof(int32_t));
  v = (int32_t)type; s.write((char *)&v, sizeof(int32_t));

  // bbox field
  if (write_bbox && !bbox.empty()) string_pack_bbox(s, "bbox", bbox);

  // optional direction (int value)
  if (dir!=MAPDB_DIR_NO) string_pack<uint32_t>(s, "dir ", (uint32_t)dir);

  // optional angle (integer value, 1/1000 degrees)
  if (angle!=0) string_pack<int32_t>(s, "angl", (int32_t)(angle*1000));

  // optional text fields (4-byte tag, 4-byte length, data);
  if (name!="") string_pack_str(s, "name", name);
  if (comm!="") string_pack_str(s, "comm", comm);
  if (src!="")  string_pack_str(s, "src ", src);

  return s.str();
}

// unpack object from a string (for DB storage)
void
MapDBObj::unpack(const std::string & str) {

  // re-initialize
  *this = MapDBObj();

  istringstream s(str);

  // class
  int32_t v;
  s.read((char*)&v, sizeof(int32_t));
  if (v<0 || v>2) throw Err() << "MapDBObj::unpack: bad class value: " << v;
  cl = (MapDBObjClass)v;

  // type
  s.read((char*)&v, sizeof(int32_t));
  type = v;

  // other fields
  while (1){
    string tag = string_unpack_tag(s);
    if (tag == "") break;
    else if (tag == "dir ") dir   = (MapDBObjDir)string_unpack<uint32_t>(s);
    else if (tag == "angl") angle = string_unpack<int32_t>(s)/1000.0;
    else if (tag == "name") name  = string_unpack_str(s);
    else if (tag == "comm") comm  = string_unpack_str(s);
    else if (tag == "src ") src   = string_unpack_str(s);
    else if (tag == "bbox") bbox  = string_unpack_bbox(s);
    else throw Err() << "Unknown tag: " << tag;
  }

}
/**********************************************************/
MapDB::MapDB(std::string name, bool create):
    folder(name, create),
    mapinfo(name + "/mapinfo.db", NULL, create, false),
    objects(name + "/objects.db", NULL, create, false),
    coords(name  + "/coords.db",  NULL, create, false),
    labels(name  + "/labels.db",  NULL, create, true),
    geohash(name + "/geohash.db", NULL, create
){
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
    else throw Err() << "MapDB::get_brd: unknown tag: [" << tag << "]";
  }
  return ret;
}

void
MapDB::set_map_brd(const dMultiLine & b) {
  ostringstream s;
  string_pack_crds(s, "crds", b);
  mapinfo.put(INF_KEY_BRD, s.str());
}

dRect
MapDB::get_map_bbox() {
  dRect ret;
  uint32_t key = INF_KEY_BBOX;
  istringstream s(mapinfo.get(key));
  if (key == 0xFFFFFFFF) return ret;

  // searching for first bbox tag
  while (1){
    string tag = string_unpack_tag(s);
    if (tag == "") break;
    else if (tag == "bbox") ret = string_unpack_bbox(s);
    else throw Err() << "MapDB::get_bbox: unknown tag: [" << tag << "]";
  }
  return ret;
}

void
MapDB::set_map_bbox(const dRect & b) {
  ostringstream s;
  string_pack_bbox(s, "bbox", b);
  mapinfo.put(INF_KEY_BBOX, s.str());
}


/**********************************************************/
uint32_t
MapDB::add(const MapDBObj & o){
  // get last id + 1
  uint32_t id;
  objects.get_last(id);
  if (id == 0xFFFFFFFF) id=0;
  else id++;

  if (id == 0xFFFFFFFF)
    throw Err() << "MapDB::add: object ID overfull";

  // insert object without bbox
  objects.put(id, o.pack(false));
  return id;
}

MapDBObj
MapDB::get(const uint32_t id){
  uint32_t id1 = id;
  std::string str = objects.get(id1);
  if (id == 0xFFFFFFFF)
    throw Err() << "MapDB::get: no such object: " << id;
  MapDBObj ret;
  ret.unpack(str);
  return ret;
}


void
MapDB::del(const uint32_t id){

  // Delete coordinates, update geohashes
  // Throw error if the object does not exist.
  set_coord(id, dMultiLine());

  // Delete the object
  objects.del(id);
}


/// set coordinates of an object
void
MapDB::set_coord(uint32_t id, const dMultiLine & crd){

  // get old object
  MapDBObj obj;
  obj.unpack(objects.get(id));
  if (id == 0xFFFFFFFF)
    throw Err() << "MapDB::set_coord: object does not exist";

  // old bounding box exists
  if (!obj.bbox.empty()){
    // remove old coordinates
    coords.del(id);

    // remove old geohash
    geohash.del(id, obj.bbox);

    // TODO: what to do with the map bbox?
    // can it be shrinked efficiently using geohashes?
  }

  // update object bbox
  obj.bbox = crd.bbox2d();

  // update object
  objects.put(id, obj.pack());

  // new bounding box non-empty
  if (!obj.bbox.empty()){

    // set coordinates (overwrite if needed)
    ostringstream s;
    string_pack_crds(s, "crds", crd);
    coords.put(id, s.str());

    // update geohash
    geohash.put(id, obj.bbox);

    // update map bbox
    dRect bbox0 = get_map_bbox();
    bbox0.expand(obj.bbox);
    set_map_bbox(bbox0);

  }
}

/// get coordinates of an object
/// similar to get_border method
dMultiLine
MapDB::get_coord(uint32_t id){
  dMultiLine ret;
  istringstream s(coords.get(id));
  if (id == 0xFFFFFFFF) return ret;

  // searching for crds tags
  while (1){
    string tag = string_unpack_tag(s);
    if (tag=="") break;
    else if (tag == "crds") ret.push_back(string_unpack_crds(s));
    else throw Err() << "MapDB::get_coord: unknown tag: [" << tag << "]";
  }
  return ret;
}

