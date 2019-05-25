#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>

#include "geohash.h"


void test_geohash_encode(void) {
    assert(GEOHASH_encode(dPoint(      -121.7,       45.37),  6) == "c216ne"       );
    assert(GEOHASH_encode(dPoint(-122.3320708,  47.6062095), 13) == "c23nb62w20sth");
    assert(GEOHASH_encode(dPoint( 139.6917064,  35.6894875), 13) == "xn774c06kdtve");
    assert(GEOHASH_encode(dPoint( 151.2071140, -33.8671390), 13) == "r3gx2f9tt5sne");
    assert(GEOHASH_encode(dPoint(  -0.1262362,  51.5001524), 13) == "gcpuvpk44kprq");
}

void verify_area(
        const char *hash,
        double lat_min, double lon_min,
        double lat_max, double lon_max) {
    dRect r = GEOHASH_decode(hash);
    assert(dist(r.tlc(), dPoint(lon_min, lat_min)) < 0.001);
    assert(dist(r.brc(), dPoint(lon_max, lat_max)) < 0.001);
}

void test_geohash_decode(void) {
    verify_area("c216ne", 45.3680419921875, -121.70654296875, 45.37353515625, -121.695556640625);
    verify_area("C216Ne", 45.3680419921875, -121.70654296875, 45.37353515625, -121.695556640625);
    verify_area("dqcw4", 39.0234375, -76.552734375, 39.0673828125, -76.5087890625);
    verify_area("DQCW4", 39.0234375, -76.552734375, 39.0673828125, -76.5087890625);
}

void test_geohash_adjacent(void) {
    assert(GEOHASH_adjacent("dqcjq", 0) == "dqcjw");
    assert(GEOHASH_adjacent("dqcjq", 4) == "dqcjn");
    assert(GEOHASH_adjacent("dqcjq", 6) ==  "dqcjm");
    assert(GEOHASH_adjacent("dqcjq", 2) ==  "dqcjr");
}

void verify_neighbors(
        const std::string & origin,
        const std::string & N,
        const std::string & S,
        const std::string & W,
        const std::string & E,
        const std::string & NW,
        const std::string & NE,
        const std::string & SW,
        const std::string & SE
        ) {
    assert(GEOHASH_adjacent(origin, 0) == N);
    assert(GEOHASH_adjacent(origin, 4) == S);
    assert(GEOHASH_adjacent(origin, 6) == W);
    assert(GEOHASH_adjacent(origin, 2) == E);
    assert(GEOHASH_adjacent(origin, 7) == NW);
    assert(GEOHASH_adjacent(origin, 1) == NE);
    assert(GEOHASH_adjacent(origin, 5) == SW);
    assert(GEOHASH_adjacent(origin, 3) == SE);
}

void test_geohash_neighbors(void) {
    verify_neighbors("dqcw5", "dqcw7", "dqctg", "dqcw4", "dqcwh", "dqcw6", "dqcwk", "dqctf", "dqctu");
    verify_neighbors("xn774c", "xn774f", "xn774b", "xn7749", "xn7751", "xn774d", "xn7754", "xn7748", "xn7750");
    verify_neighbors("gcpuvpk", "gcpuvps", "gcpuvph", "gcpuvp7", "gcpuvpm", "gcpuvpe", "gcpuvpt", "gcpuvp5", "gcpuvpj");
    verify_neighbors("c23nb62w", "c23nb62x", "c23nb62t", "c23nb62q", "c23nb62y", "c23nb62r", "c23nb62z", "c23nb62m", "c23nb62v");
}

void test_geohash_verification(void) {
  assert(GEOHASH_verify("dqcw5") == 1);
  assert(GEOHASH_verify("dqcw7") == 1);
  assert(GEOHASH_verify("abcwd") == 0);
  assert(GEOHASH_verify("dqcw5@") == 0);
}

int main( /* int argc, char **argv */ ) {
  test_geohash_decode();
  test_geohash_encode();
  test_geohash_adjacent();
  test_geohash_neighbors();
  test_geohash_verification();
  return 0;
}
