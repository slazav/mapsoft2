/*
 Source: from https://github.com/lyokato/libgeohash
 Modified for mapsoft2: slazav@altlinux.org, 2019-05-25
 */

#ifndef GEOHASH_H
#define GEOHASH_H

#include <string>
#include "geom/point.h"
#include "geom/rect.h"

// check if Geohash contains only valid characters
bool GEOHASH_verify(const std::string & hash);

// encode a point, return hash
std::string GEOHASH_encode(const dPoint & p, unsigned int hash_length);

// decode a hash, return coordinate range
dRect GEOHASH_decode(const std::string & hash);

// Find adjacent hash.
// Direction starts from north and goes clockwise (0:N, 1:NE, 2:E, 3:SE, 4:S, etc.)
std::string GEOHASH_adjacent(const std::string & hash, int dir);

#endif
