#include <sstream>
#include <iomanip>

#include "geo_data/conv_geo.h"
#include "geo_tiles/geo_tiles.h"
#include "geo_nom/geo_nom.h"
#include "geo_data/geo_utils.h"
#include "geom/multiline.h"
#include "err/err.h"

#include "geo_mkref.h"

using namespace std;

void
ms2opt_add_mkref(ext_option_list & opts){
  int m = MS2OPT_MKREF;
  ext_option_list add = {
  {"mkref",         1,0,m, "map reference type (nom, google_tile, tms_tile, proj)"},
  {"image",         1,0,m, "set image file name in the map"},
  {"name",          1,0,m, "map name (\"nom\" references)"},
  {"dpi",           1,0,m, "map resolution, pixels per inch (\"nom\" and \"proj\" references)"},
  {"mag",           1,0,m, "map magnification (\"nom\" references)"},
  {"margins",       1,0,m, "map margins, pixels (\"nom\" and \"proj\" references)"},
  {"top_margin",    1,0,m, "override top margin value (\"nom\" and \"proj\" references)"},
  {"left_margin",   1,0,m, "override left margin value (\"nom\" and \"proj\" references)"},
  {"right_margin",  1,0,m, "override right margin value (\"nom\" and \"proj\" references)"},
  {"bottom_margin", 1,0,m, "override bottom margin value (\"nom\" and \"proj\" references)"},
  {"zindex",        1,0,m, "tile zindex (\"*_tile\" references)"},
  {"tiles",         1,0,m, "tile or tile range (\"*_tile\" references)"},
  {"coords",        1,0,m, "figure in projection coordinates to be covered by the map (\"proj\" references)"},
  {"border",        1,0,m, "map border in projection coordinates (\"proj\" references)"},
  {"coords_wgs",    1,0,m, "figure in wgs84 coordinates to be covered by the map (\"*_tile\" or \"proj\" references)"},
  {"border_wgs",    1,0,m, "map border in wgs84 coordinates (\"*_tile\" or \"proj\" references)"},
  {"proj",          1,0,m, "projection setting, \"proj\" parameter string (e.g. \"+datum=WGS84 +proj=lonlat\")"
                           " or mapsoft2 alias (\"WGS\", \"WEB\", \"FI\", \"CH\", \"SU(39)\", etc.)"},
  {"scale",         1,0,m, "map scale, projection units per map cm (\"proj\" references)"},
  };
  opts.insert(opts.end(), add.begin(), add.end());
}


/********************************************************************/

GeoMap
geo_mkref(const Opt & o){
  GeoMap map;
  GeoTiles tcalc;

  if (!o.exists("mkref"))
    throw Err() << "geo_mkref: reference type (option ref) should be set";

  map.image = o.get("image","");

  string reftype = o.get("mkref","");

  /***************************************/
  if (reftype == "nom"){

    // map name
    map.name = o.get("name",string());
    if (map.name == "")
      throw "geo_mkref: nomenclature name should be set (name option)";

    // Map range (in pulkovo coordinates)
    nom_scale_t sc;
    dRect R = nom_to_range(map.name, sc, true);

    // map projection (use a bit shifted longitude to calculate boundary lon0)
    map.proj = "SU(" + type_to_str(lon2lon0(R.x + 1e-6)) + ")";

    string proj_pulk = "+ellps=krass +towgs84=+28,-130,-95 +proj=lonlat";
    // conversion map_projection -> pulkovo
    ConvGeo cnv1(map.proj, proj_pulk);
    // conversion pulkovo -> wgs84
    ConvGeo cnv2(proj_pulk);

    // map resolution
    map.image_dpi = o.get("dpi",300);
    double mag = o.get("mag",1);

    // factor (map coordinates (m))/(map point)
    double k = (int)sc/mag * 25.4e-3 /*m/in*/ / map.image_dpi;
    cnv1.rescale_src(k); // now cnv1: map points -> pulkovo

    // Border in map points (1pt accuracy);
    // We convert a closed line, then removing the last point.
    dLine brd = open(cnv1.bck_acc(rect_to_line(R, true)));

    // image size
    iRect image_bbox = ceil(cnv1.bck_acc(R));

    // Refpoints:
    dLine pts_r = rect_to_line(R, false);
    cnv1.bck(pts_r); // pulkovo -> map points
    pts_r.to_floor();

    dLine pts_w = pts_r;
    cnv1.frw(pts_w); // map points -> pulkovo
    cnv2.frw(pts_w);  // pulkovo -> wgs

    // margins
    int mt,ml,mr,mb;
    mt=ml=mr=mb=o.get("margins", 0);
    mt=o.get("top_margin", mt);
    ml=o.get("left_margin", ml);
    mr=o.get("right_margin", mr);
    mb=o.get("bottom_margin", mb);
    image_bbox = iRect(image_bbox.x-ml, image_bbox.y-mb,
                       image_bbox.w+ml+mr, image_bbox.h+mt+mb);

    brd -= image_bbox.tlc();
    pts_r -= image_bbox.tlc();
    brd.flip_y(image_bbox.h);
    pts_r.flip_y(image_bbox.h);
    map.image_size = iPoint(image_bbox.w, image_bbox.h);

    // Add map border:
    map.border.push_back(brd);

    // Add refpoints:
    map.add_ref(pts_r, pts_w);

    return map;
  }

  /***************************************/
  if (reftype == "tms_tile" || reftype == "google_tile"){

    bool G = (reftype == "google_tile");
    int  z = o.get("zindex",-1);
    list<string> confl = {"tiles", "coords"};
    o.check_conflict(confl);

    // get tile range and WGS84 border
    iRect tile_range;

    if (o.exists("tiles")){
      try {
        iPoint p = o.get("tiles", iPoint());
        if (p.z>0) z = p.z;
        tile_range = iRect(p, p+iPoint(1,1));
      }
      catch (Err e){
        tile_range = o.get("tiles", iRect());
      }

      if (tile_range.is_empty() || tile_range.is_zsize())
        throw Err() << "geo_mkref: empty tile range: " << o.get("tiles", string());

    }

    if (o.exists("coords_wgs")){

      // try coordinate point
      try {
        dPoint p = o.get("coords_wgs", dPoint());
        iPoint t;
        if (G) t = tcalc.pt_to_gtile(p, z);
        else   t = tcalc.pt_to_tile(p, z);
        tile_range = iRect(t, t+iPoint(1,1));
        goto coord_end;
      }
      catch (Err e){}

      // try coordinate range
      try {
        dRect r = o.get("coords_wgs", dRect());
        if (G) tile_range = tcalc.range_to_gtiles(r, z);
        else   tile_range = tcalc.range_to_tiles(r, z);
        goto coord_end;
      }

      // try line
      catch (Err e){}
      try {
        dMultiLine brd = o.get("coords_wgs", dMultiLine());
        if (G) tile_range = tcalc.range_to_gtiles(brd.bbox(), z);
        else   tile_range = tcalc.range_to_tiles(brd.bbox(), z);
        goto coord_end;
      }
      catch (Err e){}

      throw Err() << "geo_mkref: can't parse coords option: " << o.get("coords", string());

      coord_end:;
    }

    // here tile_range should be set to non-zero rectangle
    if (tile_range.is_empty() || tile_range.is_zsize())
      throw Err() << "geo_mkref: empty tile range, use coords or tiles opotions";

    // z-index should be set here
    if (z<0) throw Err() << "geo_mkref: z-index not set";

    // Set border
    dMultiLine brd;
    if (o.exists("border_wgs"))
      brd = o.get("border_wgs", dMultiLine());

    /*** fill map parameters ***/

    // map name
    map.name = type_to_str(tile_range);

    // map projection
    map.proj = "WEB";

    // map magnification
    double mag = o.get("mag",1);

    // find coordinates of opposite corners:
    dPoint tlc = G ? tcalc.gtile_to_range(tile_range.blc(),z).blc():
                     tcalc.tile_to_range(tile_range.tlc(),z).tlc();
    dPoint brc = G ? tcalc.gtile_to_range(tile_range.trc(),z).blc():
                     tcalc.tile_to_range(tile_range.brc(),z).tlc();

    // Refpoints. Corners correspond to points 0,image_size.
    // Should it be -0.5 .. image_size-0.5
    map.image_size = iPoint(tile_range.w, tile_range.h)*tcalc.get_tsize() * mag;
    dLine pts_w = rect_to_line(dRect(tlc,brc), false);
    dLine pts_r = rect_to_line(dRect(dPoint(0,0),map.image_size), false);

    pts_r.flip_y(map.image_size.y);
    map.add_ref(pts_r, pts_w);

    // convert border to map pixels
    if (brd.size()>0){
      // We need ConvGeo because of convenient bck_acc function.
      ConvGeo cnv(map.proj); // webmercator -> wgs84

      map.border = cnv.bck_acc(brd); // lonlat -> mercator m
      for (auto & l:map.border) for (auto & pt:l)
        pt = tcalc.m_to_px(pt, z); // mercator m -> px
      map.border -= tcalc.get_tsize()*tile_range.tlc();
      map.border.flip_y(map.image_size.y);
      map.border *= mag;
    }
    return map;
  }

  /***************************************/
  if (reftype == "proj"){

    // map projection
    map.proj = o.get("proj", "WGS");
    if (map.proj == "") throw Err() << "Option --proj is not set";

    // try to build conversion, proj -> wgs84
    ConvGeo cnv(map.proj);

    // map resolution
    double scale = cnv.is_src_deg() ? 0.01:1000;
    scale = o.get("scale",scale);
    map.image_dpi = o.get("dpi",300);

    // factor (map coordinates)/(pixels)
    double k = scale * 2.54/ map.image_dpi;
    cnv.rescale_src(k); // now cnv1: map pixels -> wgs84

    // get bounding box and border (in map pixels)
    dRect range;

    list<string> confl = {"coords", "coords_wgs"};
    o.check_conflict(confl);
    confl = {"border", "border_wgs"};
    o.check_conflict(confl);


    if (o.exists("coords")){
      // try coordinate range
      try {
        range = o.get("coords", dRect())/k;
        goto coord_end_r1;
      }
      catch (Err e){
      }
      // try line
      try {
        dMultiLine brd = o.get("coords", dMultiLine())/k;
        range = brd.bbox();
        goto coord_end_r1;
      }
      catch (Err e){}
      throw Err() << "geo_mkref: can't parse coords option: " << o.get("coords", string());
      coord_end_r1:;
    }

    if (o.exists("coords_wgs")){
      // try coordinate range
      try {
        dRect R = o.get("coords_wgs", dRect());
        range = cnv.bck_acc(R);
        goto coord_end_r2;
      }
      catch (Err e){
      }
      // try line
      try {
        range = cnv.bck_acc(o.get("coords_wgs", dMultiLine())).bbox();
        goto coord_end_r2;
      }
      catch (Err e){}
      throw Err() << "geo_mkref: can't parse coords option: " << o.get("coords", string());
      coord_end_r2:;
    }

    // check if range is set
    if (range.is_empty() || range.is_zsize())
      throw Err() << "geo_mkref: empty coordinate range";

    // expand range to closiest integers
    range.to_ceil();

    // set margins
    int mt,ml,mr,mb;
    mt=ml=mr=mb=o.get("margins", 0);
    mt=o.get("top_margin", mt);
    ml=o.get("left_margin", ml);
    mr=o.get("right_margin", mr);
    mb=o.get("bottom_margin", mb);
    range = dRect(range.x-ml, range.y-mb,
                  range.w+ml+mr, range.h+mt+mb);

    // Set border
    dMultiLine brd;
    if (o.exists("border"))
      brd = o.get("border", dMultiLine())/k;
    if (o.exists("border_wgs"))
      brd = cnv.bck_acc(o.get("border_wgs", dMultiLine()));

    /* border and range are set now */

    // Refpoints
    dLine pts_r = rect_to_line(range, false);
    dLine pts_w(pts_r);
    pts_r -= range.tlc();
    pts_r.flip_y(range.h);
    cnv.frw(pts_w);
    map.add_ref(pts_r, pts_w);

    // border
    map.border = rint(brd - range.tlc());
    map.border.flip_y(range.h);

    // image_size
    map.image_size = dPoint(range.w, range.h);

    return map;
  }

  /***************************************/
  throw Err() << "geo_mkref: unknown reference type: " << reftype;
}

// try to get some information from GeoData if there is
// no "mkref" option.
GeoMap geo_mkref(const GeoData & data, const Opt & o){

  // If "mkref" option exists build reference using options
  if (o.exists("mkref")) return geo_mkref(o);

  // If there is at list one map - use its reference.
  // This map will have the best quality.
  if (data.maps.size()>0 && data.maps.begin()->size()>0)
    return *(data.maps.begin()->begin());

  // Here we should have something smart: smaller scales
  // for large areas, different projections, etc.
  Opt opts(o);
  opts.put("mkref", "proj");

  // data bounding box
  dRect bbox;
  for (auto const & t:data.trks) bbox.expand(t.bbox());
  for (auto const & w:data.wpts) bbox.expand(w.bbox());
  opts.put("coords_wgs", bbox);
  //opts.put("coords_wgs", data.bbox());

  return geo_mkref(opts);
}

