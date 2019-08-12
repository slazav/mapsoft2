#include <cstring>
#include "geo_tiles/geo_tiles.h"

using namespace std;

void usage(){
  cout << "\n"
       << "ms2tiles -- TMS/Google tile calculations\n"
       << "\n"
       << "usage: ms2tiles [-G] -p <point> <z> -- tiles at the point\n"
       << "       ms2tiles [-G] -r <range> <z> -- tiles covering the range\n"
       << "       ms2tiles [-G] -n <x> <y> <z> -- tile range\n"
       << "       ms2tiles [-G] -c <x> <y> <z> -- tile center\n"
       << "       ms2tiles [-G] -t <x> <y> <z> <range>  -- check if the tile touches the range\n"
       << "\n"
       << "By default the program works with TMS tiles. Use -G option\n"
       << "for google tiles.\n"
       << "\n"
  ;
}

int
main(int argc, char **argv){
  try {

    bool G = false;

    // extended mode
    if ((argc>2) && (strcmp(argv[1], "-G")==0)){
      argc--; argv++; G=true;
    }

    GeoTiles T;

    if ((argc == 4) && (strcmp(argv[1], "-p") == 0)){
      dPoint p(argv[2]);
      int z = str_to_type<int>(argv[3]);
      if (G) cout << T.pt_to_gtile(p,z) << "\n";
      else   cout << T.pt_to_tile(p,z) << "\n";
      return 0;
    }

    if ((argc == 4) && (strcmp(argv[1], "-r") == 0)){
      dRect r(argv[2]);
      int z = str_to_type<int>(argv[3]);
      if (G) cout << T.range_to_gtiles(r,z) << "\n";
      else   cout << T.range_to_tiles(r,z) << "\n";
      return 0;
    }

    if ((argc == 5) && (strcmp(argv[1], "-n") == 0)){
      int x = str_to_type<int>(argv[2]);
      int y = str_to_type<int>(argv[3]);
      int z = str_to_type<int>(argv[4]);
      if (G) cout << T.gtile_to_range(x,y,z) << "\n";
      else   cout << T.tile_to_range(x,y,z) << "\n";
      return 0;
    }

    if ((argc == 5) && (strcmp(argv[1], "-c") == 0)){
      int x = str_to_type<int>(argv[2]);
      int y = str_to_type<int>(argv[3]);
      int z = str_to_type<int>(argv[4]);
      if (G) cout << T.gtile_to_range(x,y,z).cnt() << "\n";
      else   cout << T.tile_to_range(x,y,z).cnt() << "\n";
      return 0;
    }

    if ((argc == 6) && (strcmp(argv[1], "-t") == 0)){
      int x = str_to_type<int>(argv[2]);
      int y = str_to_type<int>(argv[3]);
      int z = str_to_type<int>(argv[4]);
      dRect r1(argv[5]);
      dRect r2;
      if (G) r2=T.gtile_to_range(x,y,z);
      else   r2=T.tile_to_range(x,y,z);
      return intersect(r1,r2).empty();
    }
    usage();
    return 1;

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

