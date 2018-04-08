#include <cassert>
#include <string>
#include <vector>
#include "getopt.h"
#include <cstring>

#define OPT_IN0  1  // global input options (-v, -h)
#define OPT_INP  2  // input-only options
#define OPT_CMN  4  // filter options (used both as input and output)
#define OPT_OUT  8  // output-only options
#define OPT_STP 16  // special option -o/--out

#define MASK_IN0  (MASK_INP | OPT_IN0) // mask to select global input options
#define MASK_INP  (OPT_INP | OPT_CMN | OPT_STP) // mask to select input options
#define MASK_OUT  (OPT_OUT | OPT_CMN)  // mask to select output options

using namespace std;

static struct ext_option options[] = {
  {"out",                   0,'o', OPT_STP, ""},

  {"help",                  0,'h', OPT_IN0, "show help message"},
  {"pod",                   0, 0 , OPT_IN0, "show this message as POD template"},
  {"verbose",               0,'v', OPT_IN0, "be verbose\n"},

  {"inp1",                  1,'I', OPT_INP, "input option with argument"},
  {"inp2",                  0,'J', OPT_INP, "input option w/o argument"},
  {"cmn1",                  1,'C', OPT_CMN, "common option with argument"},
  {"cmn2",                  0,'D', OPT_CMN, "common option w/o argument"},
  {"out1",                  1,'O', OPT_OUT, "output option with argument"},
  {"out2",                  0,'P', OPT_OUT, "output option w/o argument"},
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
  print_options(options, OPT_IN0, cerr, pod);
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

      if (verb) cout << "reading: " << ifile
                     << " options: " << O << "\n";
    }

    // write output file if needed
    if (argc>0){
      const char * ofile = argv[0];

      // parse output options
      O = parse_options(&argc, &argv, options, MASK_OUT);

      if (verb) cout << "writing: " << ofile
                     << " options: " << O << "\n";
    }
    else if (go_out) throw Err() << "output file expected";

    return 0;
  }

  catch(Err e){
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}
