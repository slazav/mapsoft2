///\cond HIDDEN (do not show this in Doxyden)

#include <string>
#include <vector>
#include "getopt/getopt.h"
#include "geo_data/geo_io.h"
#include "geo_render/write_geoimg.h"
#include <cstring>

using namespace std;

ext_option_list options;

void usage(bool pod=false, ostream & S = cout){
  string head = pod? "\n=head1 ":"\n";
  const char * prog = "ms2cnv";
  S << prog << " -- mapsoft2 converter for geodata and raster maps\n"
    << head << "Usage:\n"
    << prog << "  <options> <input files> -o <output file>\n"
  ;
  S << head << "General options:\n";
  print_options(options, MS2OPT_STD, S, pod);
  print_options(options, MS2OPT_OUT, S, pod);
  S << head << "Geodata input options:\n";
  print_options(options, MS2OPT_GEO_I, S, pod);
  S << head << "Geodata input and output options:\n";
  print_options(options, MS2OPT_GEO_IO, S, pod);
  S << head << "Geodata output options:\n";
  print_options(options, MS2OPT_GEO_O, S, pod);
  S << head << "Options for making map reference:\n";
  print_options(options, MS2OPT_MKREF, S, pod);
  S << head << "Options for drawing tracks:\n";
  print_options(options, MS2OPT_DRAWTRK, S, pod);
  S << head << "Options for drawing waypoints:\n";
  print_options(options, MS2OPT_DRAWWPT, S, pod);
//  S << head << "Options for drawing maps:\n";
//  print_options(options, MS2OPT_DRAWMAP, S, pod);
//  S << head << "Options for drawing grid:\n";
//  print_options(options, MS2OPT_DRAWGRD, S, pod);

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
