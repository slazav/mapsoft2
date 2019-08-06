#include <fstream>
#include <sstream>
#include <stdint.h>
#include <cstring>
#include <string>

#include "vmap.h"
#include "mp/mp.h"
#include "pack.h"

using namespace std;

/**********************************************************/
// pack object to a string (for DB storage)
string
VMapObj::pack() const {
  ostringstream s;

  // two integer numbers: class, type:
  int32_t v;
  v = (int32_t)cl;   s.write((char *)&v, sizeof(int32_t));
  v = (int32_t)type; s.write((char *)&v, sizeof(int32_t));

  // optional direction (int value)
  if (dir!=VMAP_DIR_NO) vmap_pack<uint32_t>(s, "dir ", (uint32_t)dir);

  // optional angle (integer value, 1/1000 degrees)
  if (angle!=0) vmap_pack<int32_t>(s, "angl", (int32_t)(angle*1000));

  // optional text fields (4-byte tag, 4-byte length, data);
  if (name!="") vmap_pack_str(s, "name", name);
  if (comm!="") vmap_pack_str(s, "comm", comm);
  if (src!="")  vmap_pack_str(s, "src ", src);

  // coordinates (4-byte tag, 4-byte length, data);
  vmap_pack_crds(s, *this);
  return s.str();
}

// unpack object from a string (for DB storage)
void
VMapObj::unpack(const std::string & str) {

  // re-initialize
  *this = VMapObj();

  istringstream s(str);

  // class
  int32_t v;
  s.read((char*)&v, sizeof(int32_t));
  if (v<0 || v>2) throw Err() << "VMapObj::unpack: bad class value: " << v;
  cl = (VMapObjClass)v;

  // type
  s.read((char*)&v, sizeof(int32_t));
  type = v;

  // other fields
  while (1){
    string tag = vmap_unpack_tag(s);
    if (tag == "") break;
    else if (tag == "dir ") dir = (VMapObjDir)vmap_unpack<uint32_t>(s);
    else if (tag == "angl") angle = vmap_unpack<int32_t>(s)/1000.0;
    else if (tag == "name") name = vmap_unpack_str(s);
    else if (tag == "comm") comm = vmap_unpack_str(s);
    else if (tag == "src ") src = vmap_unpack_str(s);
    else if (tag == "crds") push_back(vmap_unpack_crds(s));
    else throw Err() << "Unknown tag: " << tag;
  }

}

/**********************************************************/

std::string
VMap::get_name() {
  dMultiLine ret;
  uint32_t key = MAPINFO_NAME;
  return mapinfo.get(key);
}

/// Set map name
void
VMap::set_name(const std::string & name) {
  mapinfo.put(MAPINFO_NAME, name);
}

dMultiLine
VMap::get_brd() {
  dMultiLine ret;
  uint32_t key = MAPINFO_BRD;
  istringstream s(mapinfo.get(key));
  if (key == 0xFFFFFFFF) return ret;

  // searching for crds tags
  while (1){
    string tag = vmap_unpack_tag(s);
    if (tag == "") break;
    else if (tag == "crds") ret.push_back(vmap_unpack_crds(s));
    else throw Err() << "VMap::get_brd: unknown tag: " << tag;
  }
  return ret;
}

void
VMap::set_brd(const dMultiLine & b) {
  ostringstream s;
  vmap_pack_crds(s, b);
  mapinfo.put(MAPINFO_BRD, s.str());
}

dRect
VMap::get_bbox() {
  dRect ret;
  uint32_t key = MAPINFO_BBOX;
  istringstream s(mapinfo.get(key));
  if (key == 0xFFFFFFFF) return ret;

  // searching for first bbox tag
  while (1){
    string tag = vmap_unpack_tag(s);
    if (tag == "") break;
    else if (tag == "bbox") ret = vmap_unpack_bbox(s);
    else throw Err() << "VMap::get_brd: unknown tag: " << tag;
  }
  return ret;
}

void
VMap::set_bbox(const dRect & b) {
  ostringstream s;
  vmap_pack_bbox(s, b);
  mapinfo.put(MAPINFO_BBOX, s.str());
}


/**********************************************************/
uint32_t
VMap::add(const VMapObj & o){
  // do not work with empty objects
  if (o.is_empty()) throw Err() << "VMap::add: empty object";

  // get last id + 1
  uint32_t id;
  objects.get_last(id);
  if (id == 0xFFFFFFFF) id=0;
  else id++;

  if (id == 0xFFFFFFFF)
    throw Err() << "VMap::add: object ID overfull";


  // insert object
  objects.put(id, o.pack());

  // update bbox
  dRect bbox0 = get_bbox();
  dRect bbox1 = o.bbox2d();
  bbox0.expand(bbox1);
  set_bbox(bbox0);

  // update statial index
  geo_ind.put(id, bbox1);
  return id;
}

