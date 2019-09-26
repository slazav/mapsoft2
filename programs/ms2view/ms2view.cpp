///\cond HIDDEN (do not show this in Doxyden)

#include "getopt/getopt.h"
#include "filename/filename.h"
#include "geo_mkref/geo_mkref.h"
#include "geo_data/conv_geo.h"
#include "geo_data/geo_io.h"

#include "mapview/mapview.h"

#include "viewer/gobj_multi.h"
#include "geo_render/gobj_trk.h"
#include "geo_render/gobj_wpts.h"
#include "geo_render/gobj_maps.h"

using namespace std;

GetOptSet options;

void usage(bool pod=false, ostream & S = cout){
  const char * prog = "ms2view";
  S << prog << " -- mapsoft2 viewer for geodata and raster maps\n";
  print_header(S, pod, 1, "Usage:");
  S << prog << "  [<options>] <input files>\n"
    << "\n";
  print_header(S, pod, 1, "General options:");
  print_options(S, pod, options, MS2OPT_STD);
  print_header(S, pod, 1, "Options for reading geodata:");
  print_options(S, pod, options, MS2OPT_GEO_I | MS2OPT_GEO_IO);
  print_header(S, pod, 1, "Options for making map reference:");
  print_options(S, pod, options, MS2OPT_MKREF);
  print_header(S, pod, 1, "Options for drawing tracks:");
  print_options(S, pod, options, MS2OPT_DRAWTRK);
  print_header(S, pod, 1, "Options for drawing waypoints:");
  print_options(S, pod, options, MS2OPT_DRAWWPT);
  print_header(S, pod, 1, "Options for drawing maps:");
  print_options(S, pod, options, MS2OPT_DRAWMAP);
//  S << head << "Options for drawing grid:");
//  print_options(S, pod, options, MS2OPT_DRAWGRD);
  S << "\n";
  throw Err();
}

int
main(int argc, char **argv){
  try {

    //options.add("grid", 0,0, MS2OPT_NONSTD, "draw grid");

    ms2opt_add_std(options);
    ms2opt_add_geo_i(options);
    ms2opt_add_geo_io(options);
    ms2opt_add_mkref(options);
    ms2opt_add_drawwpt(options);
    ms2opt_add_drawtrk(options);
    ms2opt_add_drawmap(options);
//  ms2opt_add_drawgrd(options);

    std::vector<std::string> files;
    Opt opts = parse_options_all(&argc, &argv, options, ~0, files);
    std::shared_ptr<Opt> optsp(new Opt(opts));

    if (opts.exists("help")) usage();
    if (opts.exists("pod")) usage(true);

    Gtk::Main kit(argc, argv);
    Mapview mapview(optsp);
    mapview.add_files(files);
    kit.run(mapview);
    return 0;

  }
  catch (Err e) {
    if (e.str()!="") std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond

