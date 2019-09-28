///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <string>
#include <vector>
#include "getopt.h"
#include <cstring>

#define OPT_INP  1<<10  // input-only options
#define OPT_CMN  1<<11  // filter options (used both as input and output)
#define OPT_OUT  1<<12  // output-only options

#define MASK_IN0  (MASK_INP | MS2OPT_STD)          // mask to select global input options
#define MASK_INP  (OPT_INP | OPT_CMN | MS2OPT_OUT) // mask to select input options
#define MASK_OUT  (OPT_OUT | OPT_CMN)              // mask to select output options

using namespace std;

// non-standard options
GetOptSet options;


void
usage(bool pod=false){
  HelpPrinter pr(pod, options, "getopt.test");

  const char * prog = "getopt.test";
  pr.name("example of mapsoft-style getopt");
  pr.head(1, "Usage:");
  pr.usage("[<general_options>|<global_input_options>]\\\n"
           "         <input_file_1> [<input_options_1>]     \\\n"
           "         <input_file_2> [<input_options_2>] ... \\\n"
           "         (--out|-o) <output_file> [<output_options>]");

  cerr << "Each input file should be read and filtered according\n"
     << "with global and file-specific input options.\n"
     << "Then all data should be processed and written to the\n"
     << "output file according with output options.\n"
  ;
  pr.head(1, "General options:");
  pr.opts(MS2OPT_STD | MS2OPT_OUT);
  pr.head(1, "Input options:");
  pr.opts(OPT_INP);
  pr.head(1, "Common options (can be used as input and output options):");
  pr.opts(OPT_CMN);
  pr.head(1, "Output options:");
  pr.opts(OPT_OUT);
  throw Err();
}


int
main(int argc, char *argv[]){
  try{

    options.add("inp1",  1,'I', OPT_INP, "input option with argument");
    options.add("inp2",  0,'J', OPT_INP, "input option w/o argument");
    options.add("cmn1",  1,'C', OPT_CMN, "common option with argument");
    options.add("cmn2",  0,'D', OPT_CMN, "common option w/o argument");
    options.add("out1",  1,'O', OPT_OUT, "output option with argument");
    options.add("out2",  0,'P', OPT_OUT, "output option w/o argument");

    // standard options: MS2OPT_STD, MS2OPT_OUT
    ms2opt_add_std(options);
    ms2opt_add_out(options);

    if (argc<2) usage();

    Opt O = parse_options(&argc, &argv, options, MASK_IN0, "out");
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);
    bool verb = O.exists("verbose");

    Opt GO(O); // global options
    string ofile = O.get("out", "");

    // read input files
    while (ofile == "") {
      if (argc<1) break;
      const char * ifile = argv[0];

      // parse file-specific options and append global options
      O = parse_options(&argc, &argv, options, MASK_INP, "out");
      O.insert(GO.begin(), GO.end());

      ofile = O.get("out", "");
      O.erase("out");

      if (verb) cout << "reading: " << ifile
                     << " options: " << O << "\n";
    }
    // write output file if needed
    if (ofile != ""){
      // parse output options
      argc++; argv--;
      O = parse_options(&argc, &argv, options, MASK_OUT);

      if (verb) cout << "writing: " << ofile
                     << " options: " << O << "\n";
    }
  }

  catch(Err e){
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond