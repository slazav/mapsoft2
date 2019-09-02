///\cond HIDDEN (do not show this in Doxyden)

#include <numeric>
#include <cassert>
#include <string>
#include <vector>
#include "err/assert_err.h"

using namespace std;

// a few internal functions for tests:
vector<string> unpack_ozi_csv(const string & str, unsigned int count = 0, bool trim=false);
string pack_ozi_csv(const vector<string> & vec);
string convert_ozi_text(const string & str);


string convert_ozi2proj(const string & s);
string convert_proj2ozi(const string & s);
string convert_ozi2datum(const string & s);
string convert_datum2ozi(const string & s);


string merge_strings(const vector<string> & v){
  string ret;
  for (size_t i=0; i!=v.size(); ++i) ret += "[" + v[i] + "] ";
  return ret;
}

int
main() {
  try{

    assert( merge_strings(unpack_ozi_csv(",aa	,b ,  c,ddd,,   ,eee\r", 0, 0)) ==
            "[] [aa	] [b ] [  c] [ddd] [] [   ] [eee] ");
    assert( merge_strings(unpack_ozi_csv(",aa	,b ,  c,ddd,,   ,eee\r", 0, 1)) ==
            "[] [aa] [b] [c] [ddd] [] [] [eee] ");

    assert( merge_strings(unpack_ozi_csv("v,,,eee,,")) ==
            "[v] [] [] [eee] [] [] ");
    assert( merge_strings(unpack_ozi_csv("a,b", 5)) ==
            "[a] [b] [] [] [] ");
    assert( merge_strings(unpack_ozi_csv("a", 5)) ==
            "[a] [] [] [] [] ");
    assert( merge_strings(unpack_ozi_csv(",", 5)) ==
            "[] [] [] [] [] ");
    assert( merge_strings(unpack_ozi_csv("", 5)) ==
            "[] [] [] [] [] ");
    assert( merge_strings(unpack_ozi_csv("a,b,c,,d", 1)) ==
            "[a] ");

    assert( pack_ozi_csv(unpack_ozi_csv(",aa,b,c,ddd,,eee")) ==
            ",aa,b,c,ddd,,eee");
    assert( pack_ozi_csv(unpack_ozi_csv("v,,,eee,,")) ==
            "v,,,eee");
    assert( pack_ozi_csv(unpack_ozi_csv("a,b", 5)) ==
            "a,b");
    assert( pack_ozi_csv(unpack_ozi_csv("a,b,c,,d", 1)) == "a");

    vector<string> v;
    v.push_back("a,b,c");
    v.push_back("d");
    string s = pack_ozi_csv(v);
    assert( s == "a—b—c,d");
    assert( merge_strings(unpack_ozi_csv(s)) == "[a,b,c] [d] ");

    assert( convert_ozi_text("abc,def\nghi") == "abc—def ghi");

    // ozi2proj
    assert(convert_ozi2proj("Latitude/Longitude") == "+proj=lonlat");
    assert(convert_ozi2proj("Transverse Mercator") == "+proj=tmerc");
    assert(convert_ozi2proj("Mercator") == "+proj=merc");
    assert(convert_ozi2proj("Lambert Conformal Conic") == "+proj=lcc");
    assert_err(convert_ozi2proj("Mercator 1"),
       "io_ozi: unsupported Ozi projection: Mercator 1");

    // ozi2datum
    assert(convert_ozi2datum("WGS 84") == "+datum=WGS84");
    assert(convert_ozi2datum("Pulkovo 1942") == "+ellps=krass +towgs84=28,-130,-95");
    assert(convert_ozi2datum("Pulkovo 1942 (1)") == "+ellps=krass +towgs84=28,-130,-95");
    assert(convert_ozi2datum("Pulkovo 1942 (2)") == "+ellps=krass +towgs84=28,-130,-95");
    assert_err(convert_ozi2datum(""),
      "io_ozi: unsupported Ozi datum: ");

    // proj2ozi
    assert(convert_proj2ozi("+datum=wgs84 +proj=tmerc") == "Transverse Mercator");
    assert(convert_proj2ozi("+datum=wgs84 +proj=tmerc +lon0=0") == "Transverse Mercator");
    assert_err(convert_proj2ozi("+datum=wgs84 +proj=tmerc1"),
      "io_ozi: unsupported projection: tmerc1");
    assert_err(convert_proj2ozi("+datum=WGS84 +proj=tmerc1 a"),
      "io_ozi: unsupported projection: tmerc1");
    assert_err(convert_proj2ozi("+datum=WGS84 a"),
      "io_ozi: can't find proj setting: +datum=WGS84 a");

    // datum2ozi
    assert(convert_datum2ozi("+datum=WGS84 +proj=tmerc") == "WGS 84");
    assert(convert_datum2ozi("+ellps=krass +proj=tmerc +towgs84=28,-130,-95 +lon0=0") == "Pulkovo 1942 (2)");
    assert_err(convert_datum2ozi("+datum=xxx +proj=tmerc"),
      "io_ozi: unsupported datum: +datum=xxx +proj=tmerc");
    assert_err(convert_datum2ozi("+proj=tmerc"),
      "io_ozi: unsupported datum: +proj=tmerc");

  }
  catch (Err e) {
    cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond