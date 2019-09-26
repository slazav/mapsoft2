///\cond HIDDEN (do not show this in Doxyden)

#include <string>
#include <vector>
#include "getopt/getopt.h"
#include "geo_data/geo_io.h"
#include "geo_render/write_geoimg.h"
#include <cstring>

using namespace std;

GetOptSet options;

void usage(bool pod=false, ostream & S = cout){
  const char * prog = "ms2cnv";
  S << prog << " -- mapsoft2 converter for geodata and raster maps\n";
  print_header(S, pod, 1, "Usage:");
  S << prog << "  <options> <input files> -o <output file>\n";
  print_header(S, pod, 1, "General options:");
  print_options(S, pod, options, MS2OPT_STD | MS2OPT_OUT);
  print_header(S, pod, 1, "Geodata input/output options:");
  print_options(S, pod, options, MS2OPT_GEO_I | MS2OPT_GEO_IO | MS2OPT_GEO_O);
  print_header(S, pod, 1, "Rendering images");
  print_header(S, pod, 2, "Options for saving images:");
  print_options(S, pod, options, MS2OPT_IMAGE);
  print_header(S, pod, 2, "Options for making map reference:");
  print_options(S, pod, options, MS2OPT_MKREF);
  print_header(S, pod, 2, "Options for drawing tracks:");
  print_options(S, pod, options, MS2OPT_DRAWTRK);
  print_header(S, pod, 2, "Options for drawing waypoints:");
  print_options(S, pod, options, MS2OPT_DRAWWPT);
  print_header(S, pod, 2, "Options for drawing maps:");
  print_options(S, pod, options, MS2OPT_DRAWMAP);
//  print_header( S, pod, "Options for drawing grid:");
//  print_options(S, pod, options, MS2OPT_DRAWGRD);
  S << "\n";

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
    ms2opt_add_geoimg(options);
    options.replace("out_fmt", 1, 0, MS2OPT_OUT,
      "Output format, geodata (json, gu, gpx, kml, kmz, ozi, zip) "
      "or image (jpeg, png, gif, tiff, ps, pdf, svg)");

    if (argc<2) usage();
    vector<string> infiles;
    Opt O = parse_options_all(&argc, &argv, options, ~0, infiles);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);
    bool verb = O.exists("verbose");

    GeoData data;
    for (auto const & f:infiles)
      if (!read_geo(f, data, O))
        throw Err() << "Can't determine input format for file: " << f;


    // write output file if needed
    std::string ofile = O.get("out", "");
    if (ofile != ""){

      // write geodata
      if (write_geo(ofile, data, O)) return 0;

      // render image file
      if (write_geoimg(ofile, data, O)) return 0;

      throw Err() << "Can't determine output format for file: " << ofile;
    }

    return 0;
  }

  catch(Err e){
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond
