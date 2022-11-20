///\cond HIDDEN (do not show this in Doxyden)
// Program for working with map database

#include <unistd.h>
#include <cassert>
#include <iostream>
#include <string>
#include <memory>

#include "err/err.h"
#include "getopt/getopt.h"
#include "vmap2/vmap2io.h"

#include "filename/filename.h"

using namespace std;
GetOptSet options;

/**********************************************************/
void usage(bool pod=false){
  HelpPrinter pr(pod, options, "ms2vmap");

  pr.name("convert vector maps between different formats");
  pr.usage("(-h|--help|--pod)");
  pr.usage("<file> ... -o <out file> [<options>]");

  pr.head(2, "Options:");
  pr.opts({"STD", "OUT", "VMAP2I", "VMAP2O", "VMAP2IO", "VMAP2T"});
  pr.head(2, "Options for reading/writing MP format:");
  pr.opts({"MP"});
  pr.head(2, "Options for reading/writing FIG format:");
  pr.opts({"FIG"});

  throw Err();
}
/**********************************************************/

int
main(int argc, char *argv[]){
  try{

    ms2opt_add_std(options);
    ms2opt_add_out(options);
    ms2opt_add_vmap2i(options);
    ms2opt_add_vmap2o(options);
    ms2opt_add_vmap2io(options);
    ms2opt_add_vmap2t(options);
    options.remove("verbose");

    // general options -- up to first non-option argument
    vector<string> infiles;
    Opt O = parse_options_all(&argc, &argv, options, {}, infiles);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);
    if (infiles.size()<1) usage();

    // check output file
    std::string ofile = O.get("out", "");
    if (ofile == "") throw Err()
      << "non-empty output file expected (use -o option)";

    // read file with type information if it's available
    VMap2types types;
    if (O.exists("types")) types.load(O.get("types"));

    // move to import? how to avoid copying of vmap?
    VMap2 vmap;
    if (infiles.size()==1 && file_ext_check(infiles[0], ".vmap2db"))
      vmap = VMap2(infiles[0]); // open vmap2 database (no need to read!)
    else
      vmap2_import(infiles, types, vmap, O); // read all files (incl databases)

    vmap2_export(vmap, types, ofile, O);
  }
  catch (Err & e) {
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
