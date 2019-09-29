#include <cstring>
#include "geo_tiles/geo_tiles.h"
#include "getopt/getopt.h"
#include "geom/poly_tools.h"

using namespace std;

GetOptSet options;

void usage(bool pod=false){
  HelpPrinter pr(pod, options, "ms2tiles");
  pr.name("TMS/Google tile calculations");
  pr.usage("<options>");

  pr.head(1, "Options");
  pr.opts(MS2OPT_NONSTD);
  pr.opts(MS2OPT_STD);

  pr.par(
    "Value of --tiles option can be a tile \"[x,y]\" or a "
    "tile range \"[x,y,w,h]\". Value of --range option is a "
    "point \"[lon,lat]\", or rectangle \"[<lon>,<lat>,<width>,<height>]\", "
    "or line \"[[lon1,lat1],[lon1,lat1],...]\"."
  );
  pr.par(
    "If both --range and --tiles options are given "
    "program returns with exit code 0 or 1 depending "
    "on whether the coordinate range intersects with the tile range.");
  pr.par(
    "By default the program works with TMS tiles. Use --google option "
    "for google tiles."
  );


  throw Err();
}

int
main(int argc, char **argv){
  try {

    options.add("zindex", 1,'z',MS2OPT_NONSTD,
      "Set tile z-index.");
    options.add("range",  1,'r',MS2OPT_NONSTD,
      "Show tile range which covers a given figure. "
      "Figure is a point, rectangle or line in WGS84 coordinates.");
    options.add("tiles", 1,'t',MS2OPT_NONSTD,
      "Show coordinate range for a given tile or files.");
    options.add("google", 0,'G',MS2OPT_NONSTD,
      "Use Google tiles instead of TMS.");
    options.add("center", 0,'c',MS2OPT_NONSTD,
      "Instead of printing a coordinate range print its central point.");

    ms2opt_add_std(options);
    options.remove("verbose");

    if (argc<2) usage();
    vector<string> nonopt;
    Opt O = parse_options_all(&argc, &argv, options, ~0, nonopt);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);

    if (nonopt.size()) throw Err() << "unexpected value: " << nonopt[0];

    bool G = O.get("google", false);
    int z  = O.get("zindex", -1);
    bool cnt = O.get("center", false);

    if (z<0) throw Err() << "z-index is not set";

    GeoTiles T;

    // get tile range if needed
    // similar code as in geo_mkref!
    iRect tiles; // given tile range
    dRect range_t; // calculated coordinate range for tiles
    if (O.exists("tiles")){
      try {
        iPoint p = O.get("tiles", iPoint());
        if (p.z>0) z = p.z;
        tiles = iRect(p, p+iPoint(1,1));
      }
      catch (Err e){
        tiles = O.get("tiles", iRect());
      }
      if (tiles.is_zsize())
        throw Err() << "empty tile range: " << O.get("tiles", "");

      // find coordinates of opposite corners:
      dPoint tlc = G ? T.gtile_to_range(tiles.blc(),z).blc():
                       T.tile_to_range(tiles.tlc(),z).tlc();
      dPoint brc = G ? T.gtile_to_range(tiles.trc(),z).blc():
                       T.tile_to_range(tiles.brc(),z).tlc();
      range_t = dRect(tlc,brc);

      if (!O.exists("range")){
        if (cnt)
          std::cout << range_t.cnt() << "\n";
        else
          std::cout << range_t << "\n";
        return 0;
      }
    }

    // get coordinate range if needed
    dRect range; // given coordinate range
    iRect tiles_r; // calculated tile range for given coordinates
    if (O.exists("range")){
      range = figure_bbox<double>(O.get("range",""));
      if (range.is_empty()) throw Err()
        << "wrong coordinate range: " << O.get("range","");

      tiles_r = G? T.range_to_gtiles(range,z):
                   T.range_to_tiles(range,z);

      if (!O.exists("tiles")){
        std::cout << tiles_r << "\n";
        return 0;
      }
    }

    if (O.exists("tiles") && O.exists("range"))
      return intersect(range, range_t).is_empty();

    throw Err() << "--tiles or --range option expected";
  }
  catch (Err e) {
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

