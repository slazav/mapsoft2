#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>

#include "geohash.h"


void test_geohash_encode(void) {
}

void verify_area(
        const std::string & hash,
        double lat_min, double lon_min,
        double lat_max, double lon_max) {
    dRect r = GEOHASH_decode(hash);
    assert(dist(r.tlc(), dPoint(lon_min, lat_min)) < 0.001);
    assert(dist(r.brc(), dPoint(lon_max, lat_max)) < 0.001);
}

void verify_area( const std::string & hash, const dRect & r0) {
    dRect r = GEOHASH_decode(hash);
    assert(dist(r.tlc(), r0.tlc()) < 0.001);
    assert(dist(r.brc(), r0.brc()) < 0.001);
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

int main( /* int argc, char **argv */ ) {

  verify_area("c216ne", 45.3680419921875, -121.70654296875, 45.37353515625, -121.695556640625);
  verify_area("C216Ne", 45.3680419921875, -121.70654296875, 45.37353515625, -121.695556640625);
  verify_area("dqcw4", 39.0234375, -76.552734375, 39.0673828125, -76.5087890625);
  verify_area("DQCW4", 39.0234375, -76.552734375, 39.0673828125, -76.5087890625);
  verify_area("",      -90, -180, 90, 180);

  assert(GEOHASH_encode(dPoint(      -121.7,       45.37),  6) == "c216ne"       );
  assert(GEOHASH_encode(dPoint(-122.3320708,  47.6062095), 13) == "c23nb62w20sth");
  assert(GEOHASH_encode(dPoint( 139.6917064,  35.6894875), 13) == "xn774c06kdtve");
  assert(GEOHASH_encode(dPoint( 151.2071140, -33.8671390), 13) == "r3gx2f9tt5sne");
  assert(GEOHASH_encode(dPoint(  -0.1262362,  51.5001524), 13) == "gcpuvpk44kprq");

  assert(GEOHASH_adjacent("dqcjq", 0) == "dqcjw");
  assert(GEOHASH_adjacent("dqcjq", 4) == "dqcjn");
  assert(GEOHASH_adjacent("dqcjq", 6) ==  "dqcjm");
  assert(GEOHASH_adjacent("dqcjq", 2) ==  "dqcjr");
  assert(GEOHASH_adjacent("", 2) ==  "");

  verify_neighbors("dqcw5", "dqcw7", "dqctg", "dqcw4", "dqcwh", "dqcw6", "dqcwk", "dqctf", "dqctu");
  verify_neighbors("xn774c", "xn774f", "xn774b", "xn7749", "xn7751", "xn774d", "xn7754", "xn7748", "xn7750");
  verify_neighbors("gcpuvpk", "gcpuvps", "gcpuvph", "gcpuvp7", "gcpuvpm", "gcpuvpe", "gcpuvpt", "gcpuvp5", "gcpuvpj");
  verify_neighbors("c23nb62w", "c23nb62x", "c23nb62t", "c23nb62q", "c23nb62y", "c23nb62r", "c23nb62z", "c23nb62m", "c23nb62v");

  assert(GEOHASH_verify("dqcw5") == 1);
  assert(GEOHASH_verify("dqcw7") == 1);
  assert(GEOHASH_verify("abcwd") == 0);
  assert(GEOHASH_verify("dqcw5@") == 0);


  verify_area(GEOHASH_encode(dRect(37,55,0.001,0.001), 10),
     dRect(36.9909668,54.9975586, 0.0109863281, 0.00549316406));

  verify_area(GEOHASH_encode(dRect(37,55,0.01,0.01), 10),
     dRect(36.9140625,54.84375,0.3515625,0.17578125));

  // "bad" rectangles with empty hash
  verify_area(GEOHASH_encode(dRect(-0.001,-0.001,0.002,0.002), 10),
     dRect(-180,-90,360,180));

  verify_area(GEOHASH_encode(dRect(-0.001,1,0.002,1.002), 10),
     dRect(-180,-90,360,180));

  verify_area(GEOHASH_encode(dRect(1, -0.001,0.002,1.002), 10),
     dRect(-180,-90,360,180));

  // GEOHASH_encode4

  {
    dRect r(1,-0.0001,0.0002,0.0002);
    std::set<std::string> v = GEOHASH_encode4(r, 10);
    // for (auto i:v) std::cerr << i << " " << GEOHASH_decode(i) << "\n";
    assert(v.size() == 4);
    assert(v.count("kpbxyzbp") == 1); // [0.999755859,-0.000171661377,0.000343322754,0.000171661377]
    assert(v.count("kpbxyzbr") == 1); // [1.00009918,-0.000171661377,0.000343322754,0.000171661377]
    assert(v.count("s008nb00") == 1); // [0.999755859,0,0.000343322754,0.000171661377]
    assert(v.count("s008nb02") == 1); // [1.00009918,0,0.000343322754,0.000171661377]
  }

  {
    dRect r(-0.0001,1,0.0002,0.0002);
    std::set<std::string> v = GEOHASH_encode4(r, 10);
    // for (auto i:v) std::cerr << i << " " << GEOHASH_decode(i) << "\n";
    assert(v.size() == 4);
    assert(v.count("ebpvxypc") == 1); // [-0.000343322754,0.999927521,0.000343322754,0.000171661377]
    assert(v.count("ebpvxypf") == 1); // [-0.000343322754,1.00009918,0.000343322754,0.000171661377]
    assert(v.count("s00j8n01") == 1); // [0,0.999927521,0.000343322754,0.000171661377]
    assert(v.count("s00j8n04") == 1); // [0,1.00009918,0.000343322754,0.000171661377]
  }

  {
    dRect r(-0.0001,-0.0001,0.0002,0.0002);
    std::set<std::string> v = GEOHASH_encode4(r, 10);
    // for (auto i:v) std::cerr << i << " " << GEOHASH_decode(i) << "\n";
    assert(v.size() == 4);
    assert(v.count("7zzzzzzz") == 1); // [-0.000343322754,-0.000171661377,0.000343322754,0.000171661377]
    assert(v.count("ebpbpbpb") == 1); // [-0.000343322754,0,0.000343322754,0.000171661377]
    assert(v.count("kpbpbpbp") == 1); // [0,-0.000171661377,0.000343322754,0.000171661377]
    assert(v.count("s0000000") == 1); // [0,0,0.000343322754,0.000171661377]
  }

  { // "good" rectange
    dRect r(0.0001,0.0001,0.000002,0.000002);
    std::set<std::string> v = GEOHASH_encode4(r, 10);
    // for (auto i:v) std::cerr << i << " " << GEOHASH_decode(i) << "\n";
    assert(v.size() == 2);
    assert(v.count("s0000000d6") == 1); // [9.65595245e-05,9.65595245e-05,1.07288361e-05,5.36441803e-06]
    assert(v.count("s0000000d7") == 1); // [9.65595245e-05,0.000101923943,1.07288361e-05,5.36441803e-06]
  }

  { // "good" rectange
    dRect r(0.0001,0.0001,0.000002,0.000002);
    std::set<std::string> v = GEOHASH_encode4(r, 6);
    // for (auto i:v) std::cerr << i << " " << GEOHASH_decode(i) << "\n";
    assert(v.size() == 1);
    assert(v.count("s00000") == 1); // [0,0,0.0109863281,0.00549316406]
  }

  { // "big" rectange
    dRect r(-100,-50, 220,100);
    std::set<std::string> v = GEOHASH_encode4(r, 6);
    // for (auto i:v) std::cerr << i << " " << GEOHASH_decode(i) << "\n";
    assert(v.size() == 1);
    assert(v.count("") == 1); // [-180,-90,360,180]
  }

  {
    assert(GEOHASH_convert_box(dRect(1,1,1,1), dRect(0,0,10,10)) ==
           dRect(-180+36,-90+18,36,18));

    assert(GEOHASH_convert_box(dRect(1,1,1,1), dRect()) ==
           dRect(1,1,1,1));

    assert(GEOHASH_convert_box(dRect(), dRect(0,0,10,10)) ==
           dRect());
  }

  return 0;
}
