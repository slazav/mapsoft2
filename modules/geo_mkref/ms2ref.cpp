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

    write_ozi_map("map.map", map, o);

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

