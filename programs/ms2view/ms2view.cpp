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
  pr.opts({"STD"});
  pr.head(1, "Options for reading geodata");
  pr.opts({"GEO_I","GEO_IO"});
  pr.head(1, "Options for making map reference");
  pr.opts({"MKREF"});
  pr.head(1, "Options for drawing tracks");
  pr.opts({"DRAWTRK"});
  pr.head(1, "Options for drawing waypoints");
  pr.opts({"DRAWWPT"});
  pr.head(1, "Options for drawing maps");
  pr.opts({"DRAWMAP"});
//  pr.head(1, "Options for drawing grid");
//  pr.opts({"DRAWGRD"});
  pr.head(1, "Other options");
  pr.opts({"NONSTD"});
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

    const char *g = "NONSTD";
    options.add("mapdb", 1,0,g,
      "Open MapDB project");

    options.add("mapdb_config", 1,0,g,
      "Rasterisation config-file instead of default <mapdb dir>/raster.txt");


    std::vector<std::string> files;
    Opt opts = parse_options_all(&argc, &argv, options, {}, files);
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

