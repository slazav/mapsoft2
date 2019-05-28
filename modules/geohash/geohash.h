/*
 Based on https://github.com/lyokato/libgeohash
 Modified for mapsoft2: slazav@altlinux.org, 2019-05-25
 */

#ifndef GEOHASH_H
#define GEOHASH_H

#include <string>
#include <vector>
#include "geom/point.h"
#include "geom/rect.h"

// check if Geohash contains only valid characters
bool GEOHASH_verify(const std::string & hash);

// encode a point, return hash.
std::string GEOHASH_encode(const dPoint & p, unsigned int hash_length);

// Encode a rectangle, return longest hash (but not longer then maxlen)
// which covers the rectangle. Can return an ampty string.
std::string GEOHASH_encode(const dRect & r, unsigned int maxlen);

// Encode a rectangle, return up to 4 longest adjacent hashes (not
// longer then maxlen and non-empty) which covers the rectangle. Should be
// efficient (give longer hashas) for rectangles which cross hash bundaries.
std::vector<std::string> GEOHASH_encode4(const dRect & r, unsigned int maxlen);

// Decode a hash, return coordinate range. Range is empty if hash is not valid.
dRect GEOHASH_decode(const std::string & hash);

// Find adjacent hash.
// Direction starts from north and goes clockwise (0:N, 1:NE, 2:E, 3:SE, 4:S, etc.)
// return empty string on errors.
std::string GEOHASH_adjacent(const std::string & hash, int dir);

#endif
