#include <sstream>
#include <iomanip>

#include "geo_data/conv_geo.h"
#include "geo_tiles/geo_tiles.h"
#include "geo_nom/geo_nom.h"
#include "geom/multiline.h"
#include "err/err.h"

#include "geo_mkref.h"

using namespace std;


/********************************************************************/

GeoMap
geo_mkref(const Opt & o){
  GeoMap map;
  GeoTiles tcalc;

  if (!o.exists("mkref"))
    throw Err() << "geo_mkref: reference type (option ref) should be set";

  string reftype = o.get("mkref",string());

  /***************************************/
  if (reftype == "nom"){

    // map name
    map.name = o.get("name",string());
    if (map.name == "")
      throw "geo_mkref: nomenclature name should be set (name option)";

    // Map range (in pulkovo coordinates)
    nom_scale_t sc;
    dRect R = nom_to_range(map.name, sc, true);
    // lon0 projection parameter.
    // Should be correct at least for normal (non-multiplied) maps.
    double lon0 = lon2lon0(R.x + 1e-6);

    // map projection
    map.proj = "+ellps=krass +towgs84=+28,-130,-95 +proj=tmerc"
               " +lon_0=" + type_to_str(lon0) +
               " +x_0=" + type_to_str(lon2pref(lon0)) + "500000";

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

      if (tile_range.empty() || tile_range.zsize())
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
    if (tile_range.empty() || tile_range.zsize())
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
    map.proj = GEO_PROJ_WEB;

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
    map.proj = o.get("proj", GEO_PROJ_DEF);
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
    if (range.empty() || range.zsize())
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
