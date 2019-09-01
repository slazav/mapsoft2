///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "cairo/cairo_wrapper.h"
#include "mapsoft_data/mapsoft_data.h"
#include "getopt/getopt.h"
#include "filename/filename.h"
#include "geo_mkref/geo_mkref.h"
#include "geo_data/conv_geo.h"
#include "geo_data/geo_io.h"

#include "draw_trk.h"
#include "draw_pulk_grid.h"

using namespace std;

#define STD 1
#define REF 2
#define TRK 4
#define DAT 8
#define MAP 16

#define ALL 0xFFFFFF
ext_option_list options = {
  {"out",                   1,'o', STD, "output file (.pdf, .ps, .svg, .png)"},
  {"map",                   1,'m', STD, "write map file in OziExprorer format"},
  {"help",                  0,'h', STD, "show help message"},
  {"pod",                   0, 0 , STD, "show this message as POD template"},
  {"verbose",               0,'v', STD, "print some information"},

  // geo_mkref options
  {"mkref",                 1,0, REF, "map reference type (nom, google_tile, tms_tile, proj)"},
  {"name",                  1,0, REF, "map name (for \"nom\" references)"},
  {"dpi",                   1,0, REF, "map resolution, pixels per inch (for \"nom\" and \"proj\" references)"},
  {"mag",                   1,0, REF, "map magnification (for \"nom\" references)"},
  {"margins",               1,0, REF, "map margins, pixels (for \"nom\" and \"proj\" references)"},
  {"top_margin",            1,0, REF, "override top margin value (for \"nom\" and \"proj\" references)"},
  {"left_margin",           1,0, REF, "override left margin value (for \"nom\" and \"proj\" references)"},
  {"right_margin",          1,0, REF, "override right margin value (for \"nom\" and \"proj\" references)"},
  {"bottom_margin",         1,0, REF, "override bottom margin value (for \"nom\" and \"proj\" references)"},
  {"zindex",                1,0, REF, "tile zindex (for \"*_tile\" references)"},
  {"tiles",                 1,0, REF, "tile or tile range (for \"*_tile\" references)"},
  {"coords",                1,0, REF, "figure in projection coordinates to be covered by the map (for \"proj\" references)"},
  {"border",                1,0, REF, "map border in projection coordinates (for \"proj\" references)"},
  {"coords_wgs",            1,0, REF, "figure in wgs84 coordinates to be covered by the map (for \"*_tile\" or \"proj\" references)"},
  {"border_wgs",            1,0, REF, "map border in wgs84 coordinates (for \"*_tile\" or \"proj\" references)"},
  {"proj",                  1,0, REF, "projection setting (for \"proj\" references)"},
  {"scale",                 1,0, REF, "map scale, projection units per map cm (for \"proj\" references)"},

  // track drawing options
  {"trk_draw_mode",         1,0, TRK, "track drawing mode (normal, speed, height)"},
  {"trk_draw_color",        1,0, TRK, "color (for normal drawing mode), default: BCGYRM"},
  {"trk_draw_dots",         1,0, TRK, "draw dots (for normal drawing mode), default: 1"},
  {"trk_draw_arrows",       1,0, TRK, "draw arrows (for normal drawing mode), default: 0"},
  {"trk_draw_min",          1,0, TRK, "min value (km/h for speed mode, m for height mode)"},
  {"trk_draw_max",          1,0, TRK, "max value (km/h for speed mode, m for height mode)"},
  {"trk_draw_grad",         1,0, TRK, "color gradient (for speed or height modes), default: BCGYRM"},

  // grid drawing options
  {"grid",                  0,0, TRK, "draw grid"},
  {"grid_draw_color",       1,0, TRK, "grid color"},
  {"grid_draw_thick",       1,0, TRK, "grid line thickness"},

  // geodata reading options
  // ...

  // map writing  options
  {"ozi_enc",               1,0, MAP, "encoding of the map file, default: Windows-1251"},
  {"ozi_map_grid",          0,0, MAP, "write grid coordinates in reference points"},
  {"ozi_map_wgs",           0,0, MAP, "use wgs84 datum for map coordinates"},
};

void usage(bool pod=false, ostream & S = cout){
  string head = pod? "\n=head1 ":"\n";
  const char * prog = "draw_trk";
  S << prog << " -- draw_trk test program\n"
    << head << "Usage:\n"
    << prog << "  [<options>] <input files>\n"
    << "\n";
  S << head << "General options:\n";
  print_options(options, STD, S, pod);
  S << head << "Options for making map reference:\n";
  print_options(options, REF, S, pod);
  S << head << "Options for drawing tracks:\n";
  print_options(options, TRK, S, pod);
  S << head << "Options for reading geodata:\n";
  print_options(options, DAT, S, pod);
  S << head << "Options for writing map in OziExplorer format:\n";
  print_options(options, MAP, S, pod);
  throw Err();
}

int
main(int argc, char **argv){
  try {
    if (argc<2) usage();

    std::vector<std::string> files;
    Opt opts = parse_options_all(&argc, &argv, options, ALL, files);

    if (opts.exists("help")) usage();
    if (opts.exists("pod")) usage(true);

    // read geodata
    MapsoftData data;
    for (auto const &f:files) mapsoft_read(f.c_str(), data, opts);

    // set some defaults:
    if (!opts.exists("mkref")) opts.put("mkref", "proj");
    if (!opts.exists("coords") && !opts.exists("coords_wgs")){
       dRect bbox;
       for (auto const & t:data.trks) bbox.expand(t.bbox());
       opts.put("coords_wgs", bbox);
      //opts.put("coords_wgs", data.bbox());
    }
    // get output file name
    std::string fname = opts.get("out", "");
    if (fname == "") throw Err() << "No output file specified (use -o option)";

    // create map reference
    GeoMap map = geo_mkref(opts);
    ConvMap cnv(map);


    // setup cairo context
    CairoWrapper cr;
    if      (file_ext_check(fname, ".pdf")) cr.set_surface_pdf(fname.c_str(), map.image_size.x,map.image_size.y);
    else if (file_ext_check(fname, ".ps"))  cr.set_surface_ps(fname.c_str(), map.image_size.x,map.image_size.y);
    else if (file_ext_check(fname, ".svg")) cr.set_surface_svg(fname.c_str(), map.image_size.x,map.image_size.y);
    else if (file_ext_check(fname, ".png")) cr.set_surface_img(Image(map.image_size.x,map.image_size.y,32));
    else throw Err() << "Unknown output format, use .pdf, .ps, .svg, .png extension";

    // draw tracks
    dPoint origin;
    for (auto const & t:data.trks)
      draw_trk(cr, origin, cnv, t, opts);

    // draw grid
    if (opts.get("grid", 0))
      draw_pulk_grid(cr, origin, cnv, opts);

    // if format is png write file
    if (file_ext_check(fname, ".png"))
      cr->save_png(fname.c_str());

    if (opts.exists("map")){
      map.image = fname;
      write_ozi_map(opts.get("map","").c_str(), map, opts);
    }


  }
  catch (Err e) {
    if (e.str()!="") std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond

