///\cond HIDDEN (do not show this in Doxyden)

#include "getopt/getopt.h"
#include "getopt/help_printer.h"
#include "filename/filename.h"
#include "geo_data/geo_mkref.h"
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
  pr.opts({"HELP","POD","VERB"});
  pr.head(1, "Options for reading geodata");
  pr.opts({"GEO_I","GEO_IO"});
  pr.head(1, "Options for drawing tracks");
  pr.opts({"DRAWTRK"});
  pr.head(1, "Options for drawing waypoints");
  pr.opts({"DRAWWPT"});
  pr.head(1, "Options for drawing maps");
  pr.opts({"DRAWMAP"});
//  pr.head(1, "Options for drawing grid");
//  pr.opts({"DRAWGRD"});
  pr.head(1, "Options for drawing SRTM data");
  pr.opts({"SRTM"});
  pr.opts({"DRAWSRTM"});
  pr.head(1, "Other options");
  pr.opts({"NONSTD"});
  throw Err();
}

int
main(int argc, char **argv){
  try {

    //options.add("grid", 0,0, MS2OPT_NONSTD, "draw grid");

    ms2opt_add_std(options, {"HELP","POD","VERB"});
    ms2opt_add_geo_i(options);
    ms2opt_add_geo_io(options);
    ms2opt_add_drawwpt(options);
    ms2opt_add_drawtrk(options);
    ms2opt_add_drawmap(options);
//  ms2opt_add_drawgrd(options);
    ms2opt_add_drawsrtm(options);

    const char *g = "NONSTD";
    options.add("vmap", 1,0,g,
      "Open vector map");

    options.add("vmap_config", 1,0,g,
      "Rasterisation config-file instead of default raster.txt");


    std::vector<std::string> files;
    Opt opts = parse_options_all(&argc, &argv, options, {}, files);

    if (opts.exists("help")) usage();
    if (opts.exists("pod")) usage(true);

    // fix scrollbar problem
    setenv("GDK_CORE_DEVICE_EVENTS", "1", 1);

    auto app = Gtk::Application::create();
    Mapview mapview(opts);
    mapview.add_files(files);
    app->run(mapview, argc, argv);


    return 0;

  }
  catch (Err & e) {
    if (e.str()!="") std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond

