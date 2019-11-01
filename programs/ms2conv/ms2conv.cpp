///\cond HIDDEN (do not show this in Doxyden)

#include <string>
#include <vector>
#include "getopt/getopt.h"
#include "geo_data/geo_io.h"
#include "geo_data/filters.h"
#include "geo_render/write_geoimg.h"
#include <cstring>

using namespace std;

GetOptSet options;

void usage(bool pod=false){
  HelpPrinter pr(pod, options, "ms2conv");
  pr.name("mapsoft2 converter for geodata and raster maps");
  pr.usage("<options> <input files> -o <output file>");
  pr.head(1, "Description");

  pr.par(
    "ms2conv reads geodata from files (OziExplorer, GPX, KML, GeoJSON, "
    "GarminUtils, ZIP formats are supported). Then it applies filters and "
    "saves data to the output file.");

  pr.head(1, "General options");
  pr.opts({"STD", "OUT"});
  pr.head(1, "Geodata input/output options");
  pr.opts({"GEO_I", "GEO_IO", "GEO_O"});
  pr.head(1, "Geodata filtering options");
  pr.opts({"GEOFLT"});
  pr.head(1, "Rendering images");

  pr.par(
    "ms2conv can produce images with raster maps, tracks and "
    "waypoints. Supported formats: jpeg, png, gif, tiff, ps, pdf, svg. "
    "Format is selected by output file extension or by --out_fmt option. "
    "Option --map can be used to save map reference for the image (at the "
    "moment only OziExplorer map format is supported).");

  pr.par(
    "When rendering images the reference should be specified. If --mkref "
    "option exists then it is created from options. If not, then "
    "reference of the first map is used, or it is set to some "
    "default value.");

  pr.head(2, "Options for making map reference");
  pr.opts({"MKREF"});
  pr.head(2, "Options for drawing tracks");
  pr.opts({"DRAWTRK"});
  pr.head(2, "Options for drawing waypoints");
  pr.opts({"DRAWWPT"});
  pr.head(2, "Options for drawing maps");
  pr.opts({"DRAWMAP"});
//  pr.head(2, "Options for drawing grid");
//  pr.opts({"DRAWGRD"});
  pr.head(2, "Options for saving images");
  pr.opts({"IMAGE", "IMAGE_CMAP"});

  throw Err();
}


int
main(int argc, char *argv[]){
  try{
    ms2opt_add_std(options);
    ms2opt_add_out(options);
    ms2opt_add_geo_i(options);
    ms2opt_add_geo_o(options);
    ms2opt_add_geo_io(options);
    ms2opt_add_geoflt(options);
    ms2opt_add_geoimg(options);
    options.replace("out_fmt", 1, 0, "OUT",
      "Output format, geodata (json, gu, gpx, kml, kmz, ozi, zip) "
      "or image (jpeg, png, gif, tiff, ps, pdf, svg)");

    if (argc<2) usage();
    vector<string> infiles;
    Opt O = parse_options_all(&argc, &argv, options, {}, infiles);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);
    bool verb = O.exists("verbose");

    GeoData data;
    for (auto const & f:infiles) read_geo(f, data, O);

    geo_filters(data, O);

    // write output file if needed
    std::string ofile = O.get("out", "");
    if (ofile == "") return 0;

    // write geodata
    try {
      write_geo(ofile, data, O);
      return 0;
    }
    catch(Err e) {if (e.code()!=-2) throw e;}

    // render image file
    try {
      write_geoimg(ofile, data, O);
      return 0;
    }
    catch(Err e) {if (e.code()!=-2) throw e;}

    throw Err() << "Can't determine output format for file: " << ofile;
  }

  catch(Err e){
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond
