///\cond HIDDEN (do not show this in Doxyden)

#include <string>
#include <vector>
#include "getopt/getopt.h"
#include "geo_data/geo_io.h"
#include "mapsoft_data/mapsoft_data.h"
#include <cstring>

using namespace std;

ext_option_list options;

void usage(bool pod=false, ostream & S = cout){
  string head = pod? "\n=head1 ":"\n";
  const char * prog = "mapsoft";
  S << prog << " -- mapsoft2 geodata converter\n"
    << head << "Usage:\n"
    << prog << " [<general_options>|<global_input_options>]\\\n"
    << "         <input_file_1> [<input_options_1>]     \\\n"
    << "         <input_file_2> [<input_options_2>] ... \\\n"
    << "         (--out|-o) <output_file> [<output_options>]\n"
    << "\n"
    << "Each input file should be read and filtered according\n"
    << "with global and file-specific input options.\n"
    << "Then all data should be processed and written to the\n"
    << "output file according with output options.\n"
  ;
  S << head << "General options:\n";
  print_options(options, MS2OPT_STD, S, pod);
  S << head << "Input options:\n";
  print_options(options, MS2OPT_GEO_I, S, pod);
  S << head << "Common options (can be used as input and output options):\n";
  print_options(options, MS2OPT_GEO_IO, S, pod);
  S << head << "Output options:\n";
  print_options(options, MS2OPT_OUT, S, pod);
  print_options(options, MS2OPT_GEO_O, S, pod);
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

    MapsoftData data;

    Opt O = parse_options(&argc, &argv, options,
       MS2OPT_STD | MS2OPT_GEO_I | MS2OPT_GEO_IO | MS2OPT_OUT, "out");

    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);
    bool verb = O.exists("verbose");

    Opt GO(O); // global options
    bool go_out = O.exists("out");

    // read input files
    while (!go_out) {
      if (argc<1) break;
      const char * ifile = argv[0];

      // parse file-specific options and append global options
      O = parse_options(&argc, &argv, options,
        MS2OPT_GEO_I | MS2OPT_GEO_IO | MS2OPT_OUT, "out");
      O.insert(GO.begin(), GO.end());

      go_out = O.exists("out");
      if (go_out) O.erase("out");

      mapsoft_read(ifile, data, O);
    }

    // write output file if needed
    if (argc>0){
      const char * ofile = argv[0];

      // parse output options
      O = parse_options(&argc, &argv, options,
        MS2OPT_GEO_IO | MS2OPT_GEO_O);

      // copy verbose option to output options
      if (GO.exists("verbose")) O.put("verbose", GO["verbose"]);

      mapsoft_write(ofile, data, O);
    }
    else if (go_out) throw Err() << "output file expected";

    return 0;
  }

  catch(Err e){
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond
