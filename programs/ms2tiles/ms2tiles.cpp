#include <cstring>
#include "geo_tiles/geo_tiles.h"
#include "getopt/getopt.h"

using namespace std;

void usage(bool pod=false){
  GetOptSet options;
  HelpPrinter pr(pod, options, "ms2tiles");
  pr.name("TMS/Google tile calculations");
  pr.usage("ms2tiles [-G] -p <point> <z> -- tiles at the point");
  pr.usage("ms2tiles [-G] -r <range> <z> -- tiles covering the range");
  pr.usage("ms2tiles [-G] -n <tile-point> -- tile range");
  pr.usage("ms2tiles [-G] -c <tile-point> -- tile center");
  pr.usage("ms2tiles [-G] -t <tile-point> <range>  -- check if the tile touches the range");

  pr.par("Order of options is important.");
  pr.par(
    "By default the program works with TMS tiles. Use -G option "
    "for google tiles."
  );
  pr.par(
    "Parameters <point> and <range> are JSON arrays of WGS84 "
    "coordinates: [<lon>,<lat>], and [<lon>,<lat>,<width>,<height>]. "
    "Parameter <tile-point> is JSON array of three integers: [<x>,<y>,<z>]."
  );
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

    if ((argc == 3) && (strcmp(argv[1], "-n") == 0)){
      iPoint tile(argv[2]);
      if (G) cout << T.gtile_to_range(tile.x,tile.y,tile.z) << "\n";
      else   cout << T.tile_to_range(tile.x,tile.y,tile.z) << "\n";
      return 0;
    }

    if ((argc == 3) && (strcmp(argv[1], "-c") == 0)){
      iPoint tile(argv[2]);
      if (G) cout << T.gtile_to_range(tile.x,tile.y,tile.z).cnt() << "\n";
      else   cout << T.tile_to_range(tile.x,tile.y,tile.z).cnt() << "\n";
      return 0;
    }

    if ((argc == 4) && (strcmp(argv[1], "-t") == 0)){
      iPoint tile(argv[2]);
      dRect r1(argv[3]);
      dRect r2;
      if (G) r2=T.gtile_to_range(tile.x,tile.y,tile.z);
      else   r2=T.tile_to_range(tile.x,tile.y,tile.z);
      return intersect(r1,r2).is_zsize();
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

