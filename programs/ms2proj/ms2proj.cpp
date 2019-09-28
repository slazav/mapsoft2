///\cond HIDDEN (do not show this in Doxyden)

#include <string>
#include <vector>
#include <iostream>
#include "getopt/getopt.h"
#include "geo_data/conv_geo.h"

using namespace std;

GetOptSet options;

void usage(bool pod=false, ostream & S = cout){
  HelpPrinter pr(S, pod, options, "ms2proj");
  pr.name("mapsoft2 coordinate converter (libproj wrapper)");
  pr.usage("<options> <forms to convert>");
  pr.head(1, "Description");
  pr.head(1, "Options");
  pr.opts(MS2OPT_NONSTD);
  pr.opts(MS2OPT_STD);

  throw Err();
}


int
main(int argc, char *argv[]){
  try{
    ms2opt_add_std(options);

    int m = MS2OPT_NONSTD;
    options.add("from", 1, 'f', m,
      "Source coordinate system, \"libproj\" parameter string "
      "(e.g. \"+datum=WGS84 +proj=lonlat\") "
      "or mapsoft2 alias (\"WGS\", \"WEB\", \"FI\", \"CH\", \"SU39\", etc.). "
      "Default: \"WGS\"");
    options.add("to", 1, 't', m, "Destination coordinate system.");

    options.add("back", 0, 'b', m, "Do destination -> source conversion.");
    options.add("alt",  0, 'z', m, "Convert altitude (by default it is not converted).");
    options.add("acc",  1, 'a', m, "Convertion accuracy for lines and "
                                   "rectangles in source units, (default: 1.0).");
    options.remove("verbose");

    if (argc<2) usage();
    vector<string> forms;
    Opt O = parse_options_all(&argc, &argv, options, ~0, forms);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);

    double acc  = O.get("acc",  1.0);
    bool   back = O.get("back", false);

    ConvGeo cnv(
      O.get("from", "WGS"),
      O.get("to",   "WGS"),
      !O.get("alt",   false)
    );

    for (auto & f: forms){
      // try point
      try {
        dPoint pt(f);
        if (back) cnv.bck(pt);
        else cnv.frw(pt);
        std::cout << pt << "\n";
        continue;
      }
      catch (Err e) {}

      // try line
      try {
        dLine l0(f), l1;
        if (back) l1 = cnv.bck_acc(l0, acc);
        else l1 = cnv.frw_acc(l0, acc);
        std::cout << l1 << "\n";
        continue;
      }
      catch (Err e) {}

      // try multiline
      try {
        dMultiLine ml0(f), ml1;
        if (back) ml1 = cnv.bck_acc(ml0, acc);
        else ml1 = cnv.frw_acc(ml0, acc);
        std::cout << ml1 << "\n";
        continue;
      }
      catch (Err e) {}

      // try rectangle
      try {
        dRect r0(f), r1;
        if (back) r1 = cnv.bck_acc(r0, acc);
        else r1 = cnv.frw_acc(r0, acc);
        std::cout << r1 << "\n";
        continue;
      }
      catch (Err e) {}

      throw Err() << "Can't parse form: " << f;

    }
  }

  catch(Err e){
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
