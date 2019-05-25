/*
The MIT License

Copyright (c) 2011 lyo.kato@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef _LIB_GEOHASH_H_
#define _LIB_GEOHASH_H_

#include <string>

typedef enum {
    GEOHASH_NORTH = 0,
    GEOHASH_EAST,
    GEOHASH_WEST,
    GEOHASH_SOUTH
} GEOHASH_direction;

struct GEOHASH_range {
    double max;
    double min;
};

struct GEOHASH_area{
    GEOHASH_range latitude;
    GEOHASH_range longitude;
};

struct GEOHASH_neighbors {
    std::string north;
    std::string east;
    std::string west;
    std::string south;
    std::string north_east;
    std::string south_east;
    std::string north_west;
    std::string south_west;
};

bool GEOHASH_verify_hash(const std::string & hash);

std::string GEOHASH_encode(double latitude, double longitude, unsigned int hash_length);
GEOHASH_area GEOHASH_decode(const std::string & hash);

GEOHASH_neighbors GEOHASH_get_neighbors(const std::string & hash);

std::string GEOHASH_get_adjacent(const std::string & hash, GEOHASH_direction dir);


#endif
