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
  string head = pod? "\n=head1 ":"\n";
  const char * prog = "ms2mkref";
  S << prog << " -- creating a map reference\n"
    << head << "Usage: "
    << prog << "  <options>\n"
  ;
  S << head << "General options:\n";
  print_options(options, MS2OPT_STD, S, pod);
  print_options(options, MS2OPT_OUT, S, pod);
  S << head << "Making reference:\n";
  print_options(options, MS2OPT_MKREF, S, pod);
  print_options(options, MS2OPT_NONSTD, S, pod);
  S << head << "Output options (OziExplorer map format):\n";
  print_options(options, MS2OPT_GEO_O, S, pod);
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
