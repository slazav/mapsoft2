///\cond HIDDEN (do not show this in Doxyden)

#include <string>
#include <vector>
#include "getopt/getopt.h"
#include "geo_data/geo_io.h"
#include "geo_data/filters.h"
#include "geo_data/conv_geo.h"
#include "geo_mkref/geo_mkref.h"
#include "geo_render/write_geoimg.h"
#include "geo_render/gobj_trk.h"
#include "geo_render/gobj_wpts.h"
#include "geo_render/gobj_maps.h"
#include "geo_render/gobj_srtm.h"
//#include "draw_pulk_grid.h"
#include "viewer/gobj_multi.h"
#include <cstring>

using namespace std;

GetOptSet options;

void usage(bool pod=false){
  HelpPrinter pr(pod, options, "ms2conv");
  pr.name("mapsoft2 converter for geodata and raster maps");
  pr.usage("<options> <input files> -o <output file>");
  pr.head(1, "Description");

  pr.par(
    "ms2conv reads geodata from files (OziExplorer, GPX, KML, GeoJSON, "
    "GarminUtils, ZIP formats are supported). Then it applies filters and "
    "saves data to the output file.");

  pr.head(1, "General options");
  pr.opts({"STD", "OUT"});
  pr.head(1, "Geodata input/output options");
  pr.opts({"GEO_I", "GEO_IO", "GEO_O"});
  pr.head(1, "Geodata filtering options");
  pr.opts({"GEOFLT"});
  pr.head(1, "Rendering images");

  pr.par(
    "ms2conv can produce images with raster maps, tracks and "
    "waypoints. Supported formats: jpeg, png, gif, tiff, ps, pdf, svg. "
    "Format is selected by output file extension or by --out_fmt option. "
    "Option --map can be used to save map reference for the image (OziExplorer or "
    "mapsoft2-specific geojson format.");

  pr.par(
    "When rendering images the reference should be specified. If --mkref "
    "option exists then it is created from options. If not, then "
    "reference of the first map is used, or it is set to some "
    "default value.");

  pr.head(2, "Options for making map reference");
  pr.opts({"MKREF_OPTS"});
  pr.opts({"MKREF_BRD"});
  pr.opts({"MKREF_DATA"});
  pr.head(2, "Options for drawing tracks");
  pr.opts({"DRAWTRK"});
  pr.head(2, "Options for drawing waypoints");
  pr.opts({"DRAWWPT"});
  pr.head(2, "Options for drawing maps");
  pr.opts({"DRAWMAP"});
//  pr.head(2, "Options for drawing grid");
//  pr.opts({"DRAWGRD"});
  pr.head(2, "Options for drawing SRTM data");
  pr.opts({"SRTM"});
  pr.opts({"DRAWSRTM"});
  pr.head(2, "Options for saving images");
  pr.opts({"IMAGE", "IMAGE_CMAP"});
  pr.head(2, "Options for downloading");
  pr.opts({"DNLDR"});

  throw Err();
}


int
main(int argc, char *argv[]){
  try{
    ms2opt_add_std(options);
    ms2opt_add_out(options);
    ms2opt_add_geo_i(options);
    ms2opt_add_geo_o(options);
    ms2opt_add_geo_io(options);
    ms2opt_add_geoflt(options);
    ms2opt_add_downloader(options);

    // Option --out_fmt appears in both geo_o and geoimg modules.
    // We will combine description for both of them.
    options.remove("out_fmt");

    ms2opt_add_mkref_opts(options);
    ms2opt_add_mkref_brd(options);

    // option --mag appeares in both mkref_opts and mkref_data modules.
    // we will combine both descriptions here.
    options.remove("mag");
    ms2opt_add_mkref_data(options);
    options.replace("mag", 1, 0, "MKREF_OPTS",
      "1. Map magnification (with --mkref nom)"
      "2. When using existing map as a reference source, rescale it.");

    ms2opt_add_drawwpt(options);
    ms2opt_add_drawtrk(options);
    ms2opt_add_drawmap(options);
//  ms2opt_add_drawgrd(options);
    ms2opt_add_drawsrtm(options);
    options.add("htm", 1,0,"DRAWMAP", "Write html map for the image.");

    ms2opt_add_geoimg(options);
    options.replace("out_fmt", 1, 0, "OUT",
      "Output format, geodata (json, gu, gpx, kml, kmz, ozi, zip) "
      "or image (jpeg, png, gif, tiff, ps, pdf, svg)");



    options.add("srtm", 0, 0, "SRTM", "Add SRTM layer");

    if (argc<2) usage();
    vector<string> infiles;
    Opt O = parse_options_all(&argc, &argv, options, {}, infiles);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);
    bool verb = O.exists("verbose");

    GeoData data;
    for (auto const & f:infiles) read_geo(f, data, O);

    geo_filters(data, O);

    // write output file if needed
    std::string ofile = O.get("out", "");
    if (ofile == "") return 0;

    // write geodata
    try {
      write_geo(ofile, data, O);
      return 0;
    }
    catch(Err & e) {if (e.code()!=-2) throw;}


    // construct GObjMulti with all the objects we want to draw:
    GObjMulti obj;

    std::shared_ptr<SRTM> s;
    if (O.exists("srtm")){
      s.reset(new SRTM(O));
      obj.add(4, std::shared_ptr<GObjSRTM>(new GObjSRTM(s.get(),O)));
    }

    for (auto & m:data.maps)
      obj.add(3, std::shared_ptr<GObjMaps>(new GObjMaps(m)));

    for (auto & t:data.trks)
      obj.add(2, std::shared_ptr<GObjTrk>(new GObjTrk(t)));

    for (auto & w:data.wpts)
      obj.add(1, std::shared_ptr<GObjWpts>(new GObjWpts(w)));

    // Create map reference
    GeoMap ref = geo_mkref_opts(O);
    if (ref.empty())
      ref = geo_mkref_data(data, O);
    geo_mkref_brd(ref, O); // update border;

    O.check_conflict({"tmap" "htm"});
    if (O.exists("htm"))
      write_html_map(O.get("htm",""), ofile, ref, data.maps);

    write_geoimg(ofile, obj, ref, O);

    return 0;
  }

  catch(Err & e){
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond
