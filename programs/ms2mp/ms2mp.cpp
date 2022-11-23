#include <fstream>
#include "mp/mp.h"
#include "getopt/getopt.h"
#include "getopt/help_printer.h"

using namespace std;

GetOptSet options;

void usage(bool pod=false){

  HelpPrinter pr(pod, options, "ms2mp");
  pr.name("MP format converter");

  pr.usage("[<options>] <in_files> > <out_file>");

  pr.head(1, "Options");
  pr.opts({"OUT", "MP"});
  throw Err();
}

int
main(int argc, char **argv){
  try {

    ms2opt_add_out(options);
    ms2opt_add_mp_io(options);
    options.remove("verbose");

    if (argc<2) usage();
    vector<string> files;
    Opt O = parse_options_all(&argc, &argv, options, {}, files);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);


    MP data;
    for (auto const &f: files){
      std::ifstream s(f);
      read_mp(s, data, O);
    }

    write_mp(std::cout, data, O);

  }
  catch (Err & e) {
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

