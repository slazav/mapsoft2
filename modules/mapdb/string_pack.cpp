#include <iostream>
#include <stdint.h>
#include <cstring>
#include <string>
#include "err/err.h"
#include "geom/multiline.h"

using namespace std;

void
string_pack_str(ostream & s, const char *tag, const std::string & str){
  if (strlen(tag)!=4) throw Err() << "string_pack_str: 4-byte tag expected";
  s.write(tag, 4);
  uint32_t size = str.size();
  s.write((char *)&size, sizeof(uint32_t));
  s.write(str.data(), str.size());
  if (s.fail()) throw Err() << "string_pack_str: write error";
}

std::string
string_unpack_tag(istream & s){
  std::string tag(4,'\0');
  s.read((char*)tag.data(), 4);
  if (s.gcount()==0) return string();
  if (s.fail()) throw Err() << "string_unpack_tag: read error";
  return tag;
}

std::string
string_unpack_str(istream & s){
  uint32_t size;
  s.read((char*)&size, sizeof(uint32_t));
  std::string str(size, '\0');
  s.read((char*)str.data(), size);
  if (s.fail()) throw Err() << "string_unpack_str: read error";
  return str;
}

void
string_pack_crds(ostream & s, const dMultiLine & ml){
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
  if (s.fail()) throw Err() << "string_pack_crds: write error";
}

dLine
string_unpack_crds(istream & s){
  uint32_t size;
  s.read((char*)&size, sizeof(uint32_t));
  dLine ret;
  for (int i=0;i<size/2/sizeof(int32_t);i++) {
    int32_t crd[2];
    s.read((char*)crd, 2*sizeof(int32_t));
    dPoint p(crd[0]/1e7, crd[1]/1e7);
    ret.push_back(p);
  }
  if (s.fail()) throw Err() << "string_unpack_crds: read error";
  return ret;
}

void
string_pack_bbox(ostream & s, const dRect & box) {
  s.write("bbox", 4);
  uint32_t size = 4*sizeof(int32_t);
  s.write((char*)&size, sizeof(uint32_t));
  int32_t x1 = rint(box.tlc().x*1e7);
  int32_t y1 = rint(box.tlc().y*1e7);
  int32_t x2 = rint(box.brc().x*1e7);
  int32_t y2 = rint(box.brc().y*1e7);
  s.write((char *)&x1, sizeof(int32_t));
  s.write((char *)&y1, sizeof(int32_t));
  s.write((char *)&x2, sizeof(int32_t));
  s.write((char *)&y2, sizeof(int32_t));
  if (s.fail()) throw Err() << "string_pack_bbox: write error";
}


dRect
string_unpack_bbox(istream & s) {
  uint32_t size;
  s.read((char*)&size, sizeof(uint32_t));
  if (size!=4*sizeof(int32_t))
    throw Err() << "string_unpack_bbox: wrong data size: " << size;
  int32_t crd[4];
  s.read((char*)&crd, 4*sizeof(int32_t));
  if (s.fail()) throw Err() << "string_unpack_bbox: read error";
  return dRect(dPoint(crd[0]/1e7, crd[1]/1e7), dPoint(crd[2]/1e7, crd[3]/1e7));
}
