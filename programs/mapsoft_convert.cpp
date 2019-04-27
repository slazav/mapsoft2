///\cond HIDDEN (do not show this in Doxyden)

#include <string>
#include <vector>
#include "getopt/getopt.h"
#include "mapsoft_io/mapsoft_io.h"
#include <cstring>

#define OPT_GG   1  // general options (-v, -h)
#define OPT_INP  2  // input-only options
#define OPT_CMN  4  // common (filter) options: used both as input and output
#define OPT_OUT  8  // output-only options
#define OPT_STP 16  // special option -o/--out

#define MASK_IN0  (MASK_INP | OPT_GG) // mask to select global input options
#define MASK_INP  (OPT_INP | OPT_CMN | OPT_STP) // mask to select input options
#define MASK_OUT  (OPT_OUT | OPT_CMN)  // mask to select output options

using namespace std;

static struct ext_option options[] = {
  {"out",                   0,'o', OPT_STP, ""},

  {"help",                  0,'h', OPT_GG, "show help message"},
  {"pod",                   0, 0 , OPT_GG, "show this message as POD template"},
  {"verbose",               0,'v', OPT_GG, "be verbose\n"},

  {"xml_compr",             1, 0,  OPT_OUT, "compress xml output (gpx files)"},
  {"xml_indent",            1, 0,  OPT_OUT, "use xml indentation (gpx files)"},
  {"xml_ind_str",           1, 0,  OPT_OUT, "xml indentation string (gpx files)"},
  {"xml_qchar",             1, 0,  OPT_OUT, "xml quoting character (gpx files)"},
  {"gpx_write_rte",         1, 0,  OPT_OUT, "write waypoint lists as routes (gpx files)"},

  {0,0,0,0}
};

void usage(bool pod=false){
  string head = pod? "\n=head1 ":"\n";
  const char * prog = "getopt.test";
  cerr << prog << " -- example of mapsoft-style getopt\n"
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
  cerr << head << "General options:\n";
  print_options(options, OPT_GG, cerr, pod);
  cerr << head << "Input options:\n";
  print_options(options, OPT_INP, cerr, pod);
  cerr << head << "Common options (can be used as input and output options):\n";
  print_options(options, OPT_CMN, cerr, pod);
  cerr << head << "Output options:\n";
  print_options(options, OPT_OUT, cerr, pod);
  throw Err();
}


int
main(int argc, char *argv[]){
  try{
    if (argc<2) usage();

    MapsoftData data;

    Opt O = parse_options(&argc, &argv, options, MASK_IN0, "out");
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
      O = parse_options(&argc, &argv, options, MASK_INP, "out");
      O.insert(GO.begin(), GO.end());

      go_out = O.exists("out");
      if (go_out) O.erase("out");

      mapsoft_read(ifile, data, O);
    }

    // write output file if needed
    if (argc>0){
      const char * ofile = argv[0];

      // parse output options
      O = parse_options(&argc, &argv, options, MASK_OUT);

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
