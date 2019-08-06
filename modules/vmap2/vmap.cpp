#include <fstream>
#include <sstream>
#include <stdint.h>
#include <cstring>
#include <string>

#include "vmap.h"
#include "read_words/read_words.h"
#include "mp/mp.h"

using namespace std;

/**********************************************************/
// To keep objects in a database we pack everything in std::strings.
// Usually it is something silmilar to RIFF format:
// 4-byte tag, 4-byte size, data.

// pack a string
void
vmap_pack_str(ostream & s, const char *tag, const std::string & str){
  if (strlen(tag)!=4) throw Err() << "vmap_pack_str: 4-byte tag expected";
  s.write(tag, 4);
  uint32_t size = str.size();
  s.write((char *)&size, sizeof(uint32_t));
  s.write(str.data(), str.size());
  if (s.fail()) throw Err() << "vmap_pack_str: write error";
}

// pack any type
template <typename T>
void
vmap_pack(ostream & s, const char *tag, const T & v){
  if (strlen(tag)!=4) throw Err() << "vmap_pack_str: 4-byte tag expected";
  s.write(tag, 4);
  uint32_t size = sizeof(T);
  s.write((char *)&size, sizeof(uint32_t));
  s.write((char *)&v, size);
  if (s.fail()) throw Err() << "vmap_pack_str: write error";
}

// Pack a multiline with LonLat coordinates (as a multiple "crds" tags).
// Double values are multiplied by 1e7 and rounded to integer values.
void
vmap_pack_crds(ostream & s, const dMultiLine & ml){
  for (auto const &l:ml) {
    s.write("crds", 4);
    uint32_t size = l.size()*2*sizeof(int32_t); // 2 ints per point
      s.write((char *)&size, sizeof(uint32_t));
    for (auto p:l) {
      while (p.x >  180) p.x-=360;
      while (p.x < -180) p.x+=360;
      while (p.y >   90) p.y-=180;
      while (p.y <  -90) p.y+=180;
      int32_t crd[2] = {(int32_t)rint(p.x * 1e7), (int32_t)rint(p.y * 1e7)};
      s.write((char *)crd, 2*sizeof(int32_t));
    }
  }
  if (s.fail()) throw Err() << "vmap_pack_crds: write error";
}

// read 4-byte tag
std::string
vmap_unpack_tag(istream & s){
  std::string tag(4,'\0');
  s.read((char*)tag.data(), 4);
  if (s.eof()) return std::string();
  if (s.fail()) throw Err() << "vmap_unpack_tag: read error";
  return tag;
}

// unpack string (tag is already read)
std::string
vmap_unpack_str(istream & s){
  uint32_t size;
  s.read((char*)&size, sizeof(uint32_t));
  std::string str(size, '\0');
  s.read((char*)str.data(), size);
  if (s.fail()) throw Err() << "vmap_unpack_str: read error";
  return str;
}

// unpack any type (tag is already read)
template <typename T>
T vmap_unpack(istream & s){
  uint32_t size;
  s.read((char*)&size, sizeof(uint32_t));
  if (size != sizeof(T)) throw Err() << "vmap_unpack: bad data size";
  T ret;
  s.read((char*)&ret, size);
  if (s.fail()) throw Err() << "vmap_unpack_str: read error";
  return ret;
}

// unpack coordinate line (tag is already read)
dLine
vmap_unpack_crds(istream & s){
  uint32_t size;
  s.read((char*)&size, sizeof(uint32_t));
  dLine ret;
  for (int i=0;i<size/2/sizeof(int32_t);i++) {
    int32_t crd[2];
    s.read((char*)crd, 2*sizeof(int32_t));
    dPoint p(crd[0]/1e7, crd[1]/1e7);
    ret.push_back(p);
  }
  if (s.fail()) throw Err() << "vmap_unpack_crds: read error";
  return ret;
}



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

  // crds fields in the string
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

  int32_t x1,y1,x2,y2;
  s.read((char*)&x1, sizeof(int32_t));
  s.read((char*)&y1, sizeof(int32_t));
  s.read((char*)&x2, sizeof(int32_t));
  s.read((char*)&y2, sizeof(int32_t));
  if (s.fail())
    throw Err() << "VMap::get_bbox: broken field";
  return dRect(dPoint(x1/1e7, y1/1e7), dPoint(x2/1e7, y2/1e7));
}

void
VMap::set_bbox(const dRect & b) {
  int32_t x1 = rint(b.tlc().x*1e7);
  int32_t y1 = rint(b.tlc().y*1e7);
  int32_t x2 = rint(b.brc().x*1e7);
  int32_t y2 = rint(b.brc().y*1e7);
  ostringstream s;
  s.write((char *)&x1, sizeof(int32_t));
  s.write((char *)&y1, sizeof(int32_t));
  s.write((char *)&x2, sizeof(int32_t));
  s.write((char *)&y2, sizeof(int32_t));
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

