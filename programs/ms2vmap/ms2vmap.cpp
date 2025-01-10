///\cond HIDDEN (do not show this in Doxyden)
// Program for working with map database

#include <unistd.h>
#include <cassert>
#include <iostream>
#include <string>
#include <memory>

#include "err/err.h"
#include "getopt/getopt.h"
#include "getopt/help_printer.h"
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

  pr.par("\nRead vector map objects from a number of files (having no input files"
         " is also allowed), optionally apply some modifications and save to the"
         " output file. Normally output file is just overwritten, except for fig "
         " format where existing geo reference is used, all non-map objects are"
         " preserved (accroding with --min_depth/--max_depth options)."
         " Also some options (--keep_labels, --update_tag, --fix_rounding)"
         " read old data from the output file and merge them with new data.");

  pr.head(2, "Options:");
  pr.opts({"HELP","POD","VERB","OUT", "VMAP2"});
  pr.head(2, "Options for MP format:");
  pr.opts({"MP"});
  pr.head(2, "Options for FIG format:");
  pr.opts({"FIG"});
  pr.head(2, "Options for VMAP format:");
  pr.opts({"VMAP"});
  pr.head(2, "Options for OSM XML format:");
  pr.opts({"OSM"});
  pr.head(2, "Options for writing TYP file:");
  pr.opts({"TYP"});

  throw Err();
}
/**********************************************************/

int
main(int argc, char *argv[]){
  try{

    ms2opt_add_std(options, {"HELP","POD","VERB","OUT"});
    ms2opt_add_vmap2t(options);
    ms2opt_add_vmap2(options, 1, 1);
    options.add("typ",     1, 0, "TYP", "Write TYP file source for cgpsmapper");
    options.add("typ_fid", 1, 0, "TYP", "FID number for TYP file (by default it's not written)");
    options.add("typ_cp",  1, 0, "TYP", "CodePage setting for TYP file (default:1251)");
    options.remove("verbose");

    // general options -- up to first non-option argument
    vector<string> infiles;
    Opt O = parse_options_all(&argc, &argv, options, {}, infiles);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);

    // read file with type information if it's available
    VMap2types types(O);

    // write typ file
    if (O.get("typ")!="")
      types.write_typ(O.get("typ"), O.get<int>("typ_cp", 1251), O.get<int>("typ_fid", -1));

    // move to import? how to avoid copying of vmap?
    VMap2 vmap;
    if (infiles.size()==1 && file_ext_check(infiles[0], ".vmap2db"))
      vmap = VMap2(infiles[0]); // open vmap2 database (no need to read!)
    else
      vmap2_import(infiles, types, vmap, O); // read all files (incl databases)

    if (O.get("out")!="") vmap2_export(vmap, types, O.get("out"), O);
  }
  catch (Err & e) {
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
