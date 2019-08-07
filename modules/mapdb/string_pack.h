#ifndef STRING_PACK_H
#define STRING_PACK_H

#include <iostream>
#include <string>
#include <cstring>
#include <stdint.h>
#include "geom/multiline.h"


/**********************************************************/
// To keep objects in a database we pack everything in std::strings.
// Usually it is something silmilar to RIFF format:
// 4-byte tag, 4-byte size, data.

// pack a string and write to the stream
void string_pack_str(std::ostream & s, const char *tag, const std::string & str);


// read 4-byte tag
std::string string_unpack_tag(std::istream & s);

// unpack string (tag is already read)
std::string string_unpack_str(std::istream & s);


// Pack a multiline with LonLat coordinates (as a multiple "crds" tags).
// Double values are multiplied by 1e7 and rounded to nearest integer values.
void string_pack_crds(std::ostream & s, const dMultiLine & ml);

// unpack coordinate line (tag is already read)
dLine string_unpack_crds(std::istream & s);

// Pack bbox with LonLat coordinates (with "bbox" tag).
// Double values are multiplied by 1e7 and rounded to nearest integer values.
void string_pack_bbox(std::ostream & s, const dRect & box);

// unpack bbox (tag is already read)
dRect string_unpack_bbox(std::istream & s);

// pack any type
template <typename T>
void string_pack(std::ostream & s, const char *tag, const T & v){
  if (strlen(tag)!=4) throw Err() << "string_pack_str: 4-byte tag expected";
  s.write(tag, 4);
  uint32_t size = sizeof(T);
  s.write((char *)&size, sizeof(uint32_t));
  s.write((char *)&v, size);
  if (s.fail()) throw Err() << "string_pack_str: write error";
}

// unpack any type (tag is already read)
template <typename T>
T string_unpack(std::istream & s){
  uint32_t size;
  s.read((char*)&size, sizeof(uint32_t));
  if (size != sizeof(T)) throw Err() << "string_unpack: bad data size";
  T ret;
  s.read((char*)&ret, size);
  if (s.fail()) throw Err() << "string_unpack_str: read error";
  return ret;
}

#endif