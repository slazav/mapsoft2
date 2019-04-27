///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <fstream>
#include "io_gpx.h"
#include "io_kml.h"

int
main(int argc, char **argv){
  try{
    if (argc!=3) throw Err() << "usage: " << argv[0] << " <in_file> <out_file>";

    GeoData D;

    {
      std::ifstream s(argv[1]);
      if (s.fail()) throw Err() << "Can't open file: " << argv[1];
    }

    read_gpx(argv[1], D);
    write_gpx(argv[2], D);
  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond
