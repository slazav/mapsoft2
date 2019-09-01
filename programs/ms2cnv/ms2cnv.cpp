///\cond HIDDEN (do not show this in Doxyden)

#include <string>
#include <vector>
#include "getopt/getopt.h"
#include "geo_data/geo_io.h"
#include <cstring>

using namespace std;

ext_option_list options;

void usage(bool pod=false, ostream & S = cout){
  string head = pod? "\n=head1 ":"\n";
  const char * prog = "ms2cnv";
  S << prog << " -- mapsoft2 godata converter\n"
    << head << "Usage:\n"
    << prog << "  <options> <input files> -o <output file>\n"
  ;
  S << head << "General options:\n";
  print_options(options, MS2OPT_STD, S, pod);
  S << head << "Geodata input options:\n";
  print_options(options, MS2OPT_GEO_I, S, pod);
  S << head << "Geodata input and output options:\n";
  print_options(options, MS2OPT_GEO_IO, S, pod);
  S << head << "Geodata output options:\n";
  print_options(options, MS2OPT_OUT | MS2OPT_GEO_O, S, pod);
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

    if (argc<2) usage();
    vector<string> infiles;
    int optmask = MS2OPT_STD | MS2OPT_OUT | MS2OPT_GEO_I |
                  MS2OPT_GEO_IO | MS2OPT_GEO_O;
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
      if (!write_geo(ofile, data, O))
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
