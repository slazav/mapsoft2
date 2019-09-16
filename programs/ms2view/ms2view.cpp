///\cond HIDDEN (do not show this in Doxyden)

#include "cairo/cairo_wrapper.h"
#include "mapsoft_data/mapsoft_data.h"
#include "getopt/getopt.h"
#include "filename/filename.h"
#include "geo_mkref/geo_mkref.h"
#include "geo_data/conv_geo.h"
#include "geo_data/geo_io.h"
#include "viewer/dthread_viewer.h"

#include "viewer/gobj_multi.h"
#include "geo_render/gobj_trk.h"
#include "geo_render/gobj_wpts.h"
#include "geo_render/gobj_maps.h"

using namespace std;

ext_option_list options = {
//  {"grid", 0,0, MS2OPT_DRAWGRD, "draw grid"},
};

void usage(bool pod=false, ostream & S = cout){
  string head = pod? "\n=head1 ":"\n";
  const char * prog = "draw_trk";
  S << prog << " -- draw_trk test program\n"
    << head << "Usage:\n"
    << prog << "  [<options>] <input files>\n"
    << "\n";
  S << head << "General options:\n";
  print_options(options, MS2OPT_STD, S, pod);
  S << head << "Options for making map reference:\n";
  print_options(options, MS2OPT_MKREF, S, pod);
  S << head << "Options for drawing tracks:\n";
  print_options(options, MS2OPT_DRAWTRK, S, pod);
  S << head << "Options for drawing waypoints:\n";
  print_options(options, MS2OPT_DRAWWPT, S, pod);
  S << head << "Options for drawing map grid:\n";
  print_options(options, MS2OPT_GEO_I | MS2OPT_GEO_IO, S, pod);
  throw Err();
}

int
main(int argc, char **argv){
  try {
    ms2opt_add_std(options);
    ms2opt_add_geo_i(options);
    ms2opt_add_geo_io(options);
    ms2opt_add_mkref(options);
    ms2opt_add_drawtrk(options);
    ms2opt_add_drawwpt(options);

    if (argc<2) usage();

    std::vector<std::string> files;
    Opt opts = parse_options_all(&argc, &argv, options, ~0, files);

    if (opts.exists("help")) usage();
    if (opts.exists("pod")) usage(true);

    // read geodata
    MapsoftData data;
    for (auto const &f:files) mapsoft_read(f, data, opts);

    // make reference
    GeoMap map = geo_mkref(data, opts);

    // create conversion map -> WGS84
    std::shared_ptr<ConvMap> cnv(new ConvMap(map));

    // in the viewer we don't want to fit waypoints inside tiles
    opts.put("wpt_adj_brd", 0);

    // construct GObjMulti with all the objects we want to draw:
    GObjMulti obj(cnv);

    for (auto & m:data.maps)
      obj.add(3, std::shared_ptr<GObj>(new GObjMaps(cnv, m, opts)));

    for (auto & t:data.trks)
      obj.add(2, std::shared_ptr<GObj>(new GObjTrk(cnv, t, opts)));

    for (auto & w:data.wpts)
      obj.add(1, std::shared_ptr<GObj>(new GObjWpts(cnv, w, opts)));


    Gtk::Main     kit(argc, argv);
    Gtk::Window   win;
    DThreadViewer viewer(&obj);
    viewer.set_bgcolor(0x809090);
    win.add(viewer);
    win.set_default_size(640,480);
    win.show_all();

    kit.run(win);
    return 0;

  }
  catch (Err e) {
    if (e.str()!="") std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond

