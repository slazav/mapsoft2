///\cond HIDDEN (do not show this in Doxyden)

#include <string>
#include <vector>
#include <iostream>
#include "getopt/getopt.h"
#include "geo_data/conv_geo.h"

using namespace std;

GetOptSet options;

void usage(bool pod=false){
  HelpPrinter pr(pod, options, "ms2proj");
  pr.name("mapsoft2 coordinate converter (libproj wrapper)");
  pr.usage("<options> <forms to convert>");
  pr.head(1, "Description");
  pr.par(
    "This is a command-line interface to mapsoft2 coordinate conversions. "
    "You can specify source and destination coordinate systems using "
    "libproj parameter strings or mapsoft aliases and convert points, "
    "lines, multisegment lines, rectangles. When converting lines "
    "additional points may appear if converted segments are not liner "
    "in the destination coordinate system. Accuracy of such transformation "
    "can be set using --acc parameter. Use --acc 0 to do point-to-point "
    "conversion of lines. A rectangle is transformed as a line, "
    "and bounding box of the result is returned. Conversions of angles and "
    "scales is supported by mapsoft2 library, but not by this program yet."
  );
  pr.par(
    "Ponts are written as JSON arrays of two of three numbers: [x,y] or "
    "[x,y,z]. Altitude (z-coordinate) is converted if --alt option is used. "
    "Lines are written as JSON arrays of points: [[x1,y1],[x2,y2],...]. "
    "Multisegments lines are JSON arrays of lines. Rectangle is a JSON array "
    "with four numbers: [x,y,w,h]."
  );
  pr.head(1, "Options");
  pr.opts({"NONSTD", "STD"});

  pr.head(1, "Examples");

  pr.par("Convert point from WGS84 to Web mercator:");
  pr.par("	ms2proj --to WEB [30,50]");

  pr.par("Convert point from Web mercator to WGS84:");
  pr.par("	ms2proj --from WEB [3339584,6446275]");

  pr.par("Same, but coordinates are in kilometers:");
  pr.par("	ms2proj --from WEB --scale 1000 [3339.584,6446.275]");

  pr.par("Same, but source and target projections are written as libproj options:");
  pr.par("	ms2proj --from '+proj=webmerc +datum=WGS84' --to ' +proj=lonlat +datum=WGS84'"
         " [3339584,6446275]");

  pr.par("Convert line from Finnish KKJ system to WGS84:");
  pr.par("	ms2proj --from FI [[3385908,6675072],[3385908,6677072]]");

  pr.par("Convert line from WGS84 to Soviet map system with lon0=33. Accuracy which is "
         "1.0 by default is measured in source system (WGS84) and straight lines are "
         "converted to straight lines here:");
  pr.par("	ms2proj --to SU33 [[30,50],[30,54],[36,54],[36,50],[30,50]]");

  pr.par("To specify accuracy in meters we do this trick. Now the result contains"
         " more points when stright lines are converted to arcs:");
  pr.par("	ms2proj --back --from SU33 [[30,50],[30,54],[36,54],[36,50],[30,50]]");




  throw Err();
}


int
main(int argc, char *argv[]){
  try{
    ms2opt_add_std(options);

    const char *g = "NONSTD";
    options.add("from", 1, 'f', g,
      "Source coordinate systeg, \"libproj\" parameter string "
      "(e.g. \"+datum=WGS84 +proj=lonlat\") "
      "or mapsoft2 alias (\"WGS\", \"WEB\", \"FI\", \"CH\", \"SU39\", etc.). "
      "Default: \"WGS\"");
    options.add("to", 1, 't', g, "Destination coordinate system.");

    options.add("back", 0, 'b', g, "Do inverse conversion, destination -> source.");
    options.add("alt",  0, 'z', g, "Convert altitude (by default it is not converted).");
    options.add("acc",  1, 'a', g, "Convertion accuracy for lines and "
                                   "rectangles in source units, (default: 1.0).");
    options.add("shift", 1, 'M', g, "Shift coordinates before conversion (default: [0,0])");
    options.add("scale", 1, 'S', g, "Scale coordinate after shifting and before conversion (default: 1)");
    options.remove("verbose");

    if (argc<2) usage();
    vector<string> forms;
    Opt O = parse_options_all(&argc, &argv, options, {}, forms);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);

    double acc  = O.get("acc",  1.0);
    bool   back = O.get("back", false);
    double sc   = O.get("scale", 1.0);
    dPoint sh   = O.get("shift", dPoint(0,0));

    ConvGeo cnv(
      O.get("from", "WGS"),
      O.get("to",   "WGS"),
      !O.get("alt",   false)
    );

    for (auto & f: forms){
      // We want to ignore parse errors (to try points,lines,rects),
      // but throw errors during conversions
      bool parse_done = false;
      // try point
      try {
        dPoint pt(f);
        parse_done = true;
        pt = (pt+sh)*sc;
        if (back) cnv.bck(pt);
        else cnv.frw(pt);
        std::cout << pt << "\n";
        continue;
      }
      catch (Err e) { if (parse_done) throw e;}

      // try line
      try {
        dLine l0(f), l1;
        parse_done = true;
        l0 = (l0+sh)*sc;
        if (back) l1 = cnv.bck_acc(l0, acc);
        else l1 = cnv.frw_acc(l0, acc);
        std::cout << l1 << "\n";
        continue;
      }
      catch (Err e) { if (parse_done) throw e;}

      // try multiline
      try {
        dMultiLine ml0(f), ml1;
        parse_done = true;
        ml0 = (ml0+sh)*sc;
        if (back) ml1 = cnv.bck_acc(ml0, acc);
        else ml1 = cnv.frw_acc(ml0, acc);
        std::cout << ml1 << "\n";
        continue;
      }
      catch (Err e) { if (parse_done) throw e;}

      // try rectangle
      try {
        dRect r0(f), r1;
        parse_done = true;
        r0 = (r0+sh)*sc;
        if (back) r1 = cnv.bck_acc(r0, acc);
        else r1 = cnv.frw_acc(r0, acc);
        std::cout << r1 << "\n";
        continue;
      }
      catch (Err e) { if (parse_done) throw e;}

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
