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

void usage(bool pod=false){
  HelpPrinter pr(pod, options, "ms2view");

  pr.name("mapsoft2 viewer for geodata and raster maps");
  pr.usage("[<options>] <input files>");
  pr.head(1, "General options");
  pr.opts(MS2OPT_STD);
  pr.head(1, "Options for reading geodata");
  pr.opts(MS2OPT_GEO_I | MS2OPT_GEO_IO);
  pr.head(1, "Options for making map reference");
  pr.opts(MS2OPT_MKREF);
  pr.head(1, "Options for drawing tracks");
  pr.opts(MS2OPT_DRAWTRK);
  pr.head(1, "Options for drawing waypoints");
  pr.opts(MS2OPT_DRAWWPT);
  pr.head(1, "Options for drawing maps");
  pr.opts(MS2OPT_DRAWMAP);
//  pr.head(1, "Options for drawing grid");
//  pr.opts(MS2OPT_DRAWGRD);
  pr.head(1, "Other options");
  pr.opts(MS2OPT_NONSTD);
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

    options.add("mapdb", 1,0,MS2OPT_NONSTD,
      "Open MapDB project");

    options.add("mapdb_config", 1,0,MS2OPT_NONSTD,
      "Resterisation config-file instead of default <mapdb dir>/raster.txt");

    std::vector<std::string> files;
    Opt opts = parse_options_all(&argc, &argv, options, ~0, files);
    std::shared_ptr<Opt> optsp(new Opt(opts));

    if (opts.exists("help")) usage();
    if (opts.exists("pod")) usage(true);

    // fix scrollbar problem
    setenv("GDK_CORE_DEVICE_EVENTS", "1", 1);

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

