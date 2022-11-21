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

#include "vmap2/vmap2.h"
#include "vmap2/vmap2io.h"
#include "vmap2/vmap2types.h"
#include "vmap2/vmap2gobj.h"

using namespace std;

GetOptSet options;

void usage(bool pod=false){
  HelpPrinter pr(pod, options, "ms2render");
  pr.name("render geodata, srtm, raster and vector maps");
  pr.usage("<options> <input files> -o <output file>");
  pr.head(1, "Description");

  pr.par(
    "ms2render produce images with raster maps, tracks, "
    "waypoints, srtm data. Supported formats: jpeg, png, gif, tiff, ps, pdf, svg. "
    "Format is selected by output file extension or by --out_fmt option. "
    "Option --map can be used to save map reference for the image (OziExplorer or "
    "mapsoft2-specific geojson format.");

  pr.par(
    "Reference for the image should be specified. If --mkref "
    "option exists then it is created from options. If not, then "
    "reference of the first map is used, or it is set to some "
    "default value.");

  pr.head(1, "General options");
  pr.opts({"STD", "OUT"});
  pr.head(1, "Geodata input/output options");
  pr.opts({"GEO_I", "GEO_IO", "GEO_O"});
  pr.head(1, "Options for making map reference");
  pr.opts({"MKREF_OPTS"});
  pr.opts({"MKREF_BRD"});
  pr.opts({"MKREF_DATA"});
  pr.head(1, "Options for rendering tracks");
  pr.opts({"DRAWTRK"});
  pr.head(1, "Options for rendering waypoints");
  pr.opts({"DRAWWPT"});
  pr.head(1, "Options for rendering raster maps");
  pr.opts({"DRAWMAP"});
  pr.head(1, "Options for rendering vector maps");
  pr.opts({"VMAP2_RENDER"});
//  pr.head(1, "Options for drawing grid");
//  pr.opts({"DRAWGRD"});
  pr.head(1, "Options for drawing SRTM data");
  pr.opts({"SRTM", "DRAWSRTM"});
  pr.head(1, "Options for saving images");
  pr.opts({"IMAGE", "IMAGE_CMAP"});
  pr.head(1, "Options for downloading");
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
    ms2opt_add_downloader(options);
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
    ms2opt_add_vmap2_render(options);

    options.add("types", 1, 't', "VMAP2_RENDER", "File with type information.");

    // Option --out_fmt appears in both geo_o and geoimg modules.
    options.remove("out_fmt");
    ms2opt_add_geoimg(options);

    options.add("htm", 1,0,"DRAWMAP", "Write html map for the image.");
    options.add("srtm", 0, 0, "SRTM", "Add SRTM layer");


    if (argc<2) usage();
    vector<string> infiles;
    Opt O = parse_options_all(&argc, &argv, options, {}, infiles);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);
    bool verb = O.exists("verbose");

    // construct GObjMulti with all the objects we want to draw:
    GObjMulti obj;


    // Srtm layer
    std::shared_ptr<SRTMSurf> s;
    if (O.exists("srtm")){
      s.reset(new SRTMSurf(O));
      obj.add(5, std::shared_ptr<GObjSRTM>(new GObjSRTM(s.get(),O)));
    }

    // Geodata
    std::vector<std::string> non_geodata;
    GeoData data;
    for (auto const & f:infiles){
      // try reading geodata
      try {
        read_geo(f, data, O);
      }
      catch (const Err & e){
        if (e.code()!=-2) throw;
        non_geodata.push_back(f);
      }
    }
    for (auto & m:data.maps)
      obj.add(3, std::shared_ptr<GObjMaps>(new GObjMaps(m)));
    for (auto & t:data.trks)
      obj.add(2, std::shared_ptr<GObjTrk>(new GObjTrk(t)));
    for (auto & w:data.wpts)
      obj.add(1, std::shared_ptr<GObjWpts>(new GObjWpts(w)));

    // Vector maps
    VMap2types types; // read file with type information if it's available
    GeoMap ref_v;
    VMap2 vmap2; // keeps data for GObjV
    if (O.exists("types")) types.load(O.get("types"));
    for (auto const & f:non_geodata) {
      if (file_ext_check(f, ".vmap2db")) vmap2 = VMap2(f);
      else vmap2_import({f}, types, vmap2, O);
      auto obj_v = std::shared_ptr<GObjVMap2>(new GObjVMap2(vmap2, O));
      obj.add(4, obj_v);
      if (ref_v.empty()) ref_v = obj_v->get_ref();
    }

    std::string ofile = O.get("out", "");

    // Create map reference
    GeoMap ref = geo_mkref_opts(O);
    if (ref.empty())
      ref = geo_mkref_data(data, O);
    if (ref.empty())
      ref=ref_v;
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
