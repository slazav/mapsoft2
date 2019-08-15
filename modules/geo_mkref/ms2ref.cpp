#include <cstring>
#include "geo_mkref.h"
#include "geo_data/geo_io.h"

using namespace std;

void usage(){
  cout << "\n"
       << "ms2ref -- make map reference\n"
       << "\n"
  ;
}

int
main(int argc, char **argv){
  try {

    Opt o;
    o.put("ref", "nom");
    o.put("name", "n37-001");
    o.put("dpi",  200);
    GeoMap map = geo_mkref(o);
    write_ozi_map("map1.map", map, o);

    o=Opt();
    o.put("ref", "tms_tile");
    o.put("tiles", "[1,1,10]");
    map = geo_mkref(o);
    write_ozi_map("map2.map", map, o);

    o=Opt();
    o.put("ref", "tms_tile");
    o.put("tiles", "[1,1,2,2]");
    o.put("zindex", "3");
    map = geo_mkref(o);
    write_ozi_map("map3.map", map, o);

    o=Opt();
    o.put("ref", "tms_tile");
    o.put("coords", "[64,32]");
    o.put("zindex", "3");
    map = geo_mkref(o);
    write_ozi_map("map4.map", map, o);

    o=Opt();
    o.put("ref", "tms_tile");
    o.put("coords", "[[64,32],[65,31],[63,29]]");
    o.put("zindex", "7");
    map = geo_mkref(o);
    write_ozi_map("map4.map", map, o);

    o=Opt();
    o.put("ref", "proj");
    o.put("proj", "+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=7500000 +lon_0=39");
    o.put("coords", "[7376000,6208000,2000,2000]");
    o.put("scale", 1000);
    map = geo_mkref(o);
    write_ozi_map("map5.map", map, o);

    o=Opt();
    o.put("ref", "proj");
    o.put("proj", "+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=7500000 +lon_0=39");
    o.put("coords", "[[7376000,6208000],[7380000,6208000],[7380000,6212000],"
                     "[7378000,6212000],[7378000,6210000],[7376000,6210000]]");
    o.put("scale", 1000);
    map = geo_mkref(o);
    write_ozi_map("map6.map", map, o);

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

