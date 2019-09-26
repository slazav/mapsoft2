///\cond HIDDEN (do not show this in Doxyden)

#include <string>
#include <vector>
#include "getopt/getopt.h"
#include "geo_data/geo_io.h"
#include "geo_mkref/geo_mkref.h"
#include <cstring>

using namespace std;

GetOptSet options;

void usage(bool pod=false, ostream & S = cout){
  const char * prog = "ms2mkref";
  S << prog << " -- creating a map reference\n";
  print_header(S, pod, 1, "Usage:");
  S << prog << "  <options>\n";
  ;
  print_header(S, pod, 1, "General options:");
  print_options(S, pod, options, MS2OPT_STD);
  print_options(S, pod, options, MS2OPT_OUT);
  print_header(S, pod, 1, "Making reference:");
  print_options(S, pod, options, MS2OPT_MKREF);
  print_options(S, pod, options, MS2OPT_NONSTD);
  print_header(S, pod, 1, "Output options (OziExplorer map format):");
  print_options(S, pod, options, MS2OPT_GEO_O);
  throw Err();
}


int
main(int argc, char *argv[]){
  try{

    options.add("image", 1,0,MS2OPT_NONSTD, "set image file name in the map");

    ms2opt_add_std(options);
    ms2opt_add_out(options);
    ms2opt_add_mkref(options);
    ms2opt_add_ozimap_o(options);

    if (argc<2) usage();
    vector<string> nonopt;
    Opt O = parse_options_all(&argc, &argv, options, ~0, nonopt);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);
    bool verb = O.exists("verbose");

    if (nonopt.size()) usage();

    // create map reference
    GeoMap map = geo_mkref(O);
    map.image = O.get("image", "");

    // write output file if needed
    std::string ofile = O.get("out", "");
    if (ofile != "") write_ozi_map(ofile, map, O);

    return 0;
  }

  catch(Err e){
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond
