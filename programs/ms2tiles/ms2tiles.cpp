#include <cstring>
#include "geo_tiles/geo_tiles.h"
#include "getopt/getopt.h"
#include "getopt/help_printer.h"
#include "geo_data/geo_utils.h"
#include "geom/poly_tools.h"

using namespace std;

GetOptSet options;

void usage(bool pod=false){
  HelpPrinter pr(pod, options, "ms2tiles");
  pr.name("TMS/Google tile calculations");
  pr.usage("<options>");

  pr.head(1, "Options");
  pr.opts({"NONSTD", "STD"});

  throw Err();
}

int
main(int argc, char **argv){
  try {

    const char *g = "NONSTD";
    options.add("zindex", 1,'z',g,
      "Set tile z-index.");
    options.add("range",  1,'r',g,
      "Show tile range which covers a given figure. "
      "Figure is a point ([lon,lat]), rectangle ([<lon>,<lat>,<width>,<height>]) "
      "line ([[lon1,lat1],[lon1,lat1],...]), multi-segment line in WGS84 coordinates, "
      "or a geodata file with tracks/waypoints.");
    options.add("cover",  1,0,g,
      "Show all tiles which cover a given figure. "
      "Figure is set in the same way as in the --range option.");
    options.add("tiles", 1,'t',g,
      "Show coordinate range for a given tile \"[x,y]\" or a tile range \"[x,y,w,h]\". "
      "If --tiles option is combined with --cover or --range, program returns with "
      "exit code 0 or 1 depending on whether the figure or its bounding is covered "
      "with the tile or the tile range.");
    options.add("google", 0,'G',g,
      "By default the program works with TMS tiles. "
      "Use --google option to work with Google tiles instead.");
    options.add("center", 0,'c',g,
      "Instead of printing a coordinate range print its central point.");

    ms2opt_add_std(options);
    options.remove("verbose");

    if (argc<2) usage();
    vector<string> nonopt;
    Opt O = parse_options_all(&argc, &argv, options, {}, nonopt);
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
      catch (Err & e){
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

      if (!O.exists("range") && !O.exists("cover")){
        if (cnt)
          std::cout << range_t.cnt() << "\n";
        else
          std::cout << range_t << "\n";
        return 0;
      }
    }

    // range or cover options
    O.check_conflict({"range", "cover"});

    // get coordinate range for --range or --cover options
    if (O.exists("range")){
      // wgs range of the figure given in --range option
      dRect range = figure_geo_line(O.get("range","")).bbox();
      if (range.is_empty()) throw Err()
        << "empty coordinate range: " << O.get("range","");

      dRect tiles_r = G? T.range_to_gtiles(range,z):
                         T.range_to_tiles(range,z);

      if (O.exists("tiles"))
        return intersect(range, range_t).is_empty();

      std::cout << tiles_r << "\n";
      return 0;
    }


    if (O.exists("cover")){
      // wgs figure given in --range option
      dMultiLine f = figure_geo_line(O.get("cover",""));
      if (f.size()==0) throw Err()
        << "empty coordinates: " << O.get("cover","");
      dRect range = f.bbox();

      // tile range
      dRect tiles_r = G? T.range_to_gtiles(range,z):
                         T.range_to_tiles(range,z);

      for (int y = tiles_r.y; y < tiles_r.y + tiles_r.h; y++){
        for (int x = tiles_r.x; x < tiles_r.x + tiles_r.w; x++){
          dRect r = G? T.gtile_to_range(iPoint(x,y), z):
                       T.tile_to_range(iPoint(x,y), z);
          if (rect_in_polygon(r, f) == 0) continue;
          if (O.exists("tiles")) {
            if (tiles.contains_l(iPoint(x,y))) return 0;
          }
          else {
            std::cout << iPoint(x,y,z) << "\n";
          }
        }
      }
      if (O.exists("tiles")) return 1;
      return 0;
    }

    throw Err() << "--tiles, --range or --cover option expected";
  }
  catch (Err & e) {
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

