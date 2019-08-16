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

  if (!o.exists("ref"))
    throw Err() << "mk_ref: reference type (option ref) should be set";

  string reftype = o.get("ref",string());

  /***************************************/
  if (reftype == "nom"){

    // map name
    map.name = o.get("name",string());
    if (map.name == "")
      throw "mk_ref: nomenclature name should be set (name option)";

    // Map range (in pulkovo coordinates)
    nom_scale_t sc;
    dRect R = nom_to_range(map.name, sc, true);
    // lon0 projection parameter.
    // Should be correct at least for normal (non-multiplied) maps.
    double lon0 = lon2lon0(R.x + 1e-6);

    // map projection
    map.proj = "+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=500000 +lon_0="
             + type_to_str(lon0);
    string proj_pulk = "+ellps=krass +towgs84=28,-130,-95 +proj=lonlat";
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
    dLine brd = open(cnv1.bck_acc(rect_to_line(R, true), 0.5));

    // Refpoints:
    dLine pts_r = rect_to_line(R, false),
          pts_w = pts_r;
    cnv1.bck(pts_r); // pulkovo -> map points
    cnv2.frw(pts_w);  // pulkovo -> wgs

    // image origin and size
    iRect image_bbox = ceil(pts_r.bbox());
    // margins
    int mt,ml,mr,mb;
    mt=ml=mr=mb=o.get("margins", 0);
    mt=o.get("top_margin", mt);
    ml=o.get("left_margin", ml);
    mr=o.get("right_margin", mr);
    mb=o.get("bottom_margin", mb);
    image_bbox = iRect(image_bbox.x-ml, image_bbox.y-mt,
                       image_bbox.w+ml+mr, image_bbox.h+mt+mb);

    brd -= image_bbox.tlc();
    pts_r -= image_bbox.tlc();
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
    dMultiLine brd;

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

    if (o.exists("coords")){

      // try coordinate point
      try {
        dPoint p = o.get("coords", dPoint());
        iPoint t;
        if (G) t = tcalc.pt_to_gtile(p, z);
        else   t = tcalc.pt_to_tile(p, z);
        tile_range = iRect(t, t+iPoint(1,1));
        goto coord_end;
      }
      catch (Err e){}

      // try coordinate range
      try {
        dRect r = o.get("coords", dRect());
        if (G) tile_range = tcalc.range_to_gtiles(r, z);
        else   tile_range = tcalc.range_to_tiles(r, z);
        goto coord_end;
      }

      // try border
      catch (Err e){}
      try {
        brd = o.get("coords", dMultiLine());
        if (G) tile_range = tcalc.range_to_gtiles(brd.bbox(), z);
        else   tile_range = tcalc.range_to_tiles(brd.bbox(), z);
        goto coord_end;
      }
      catch (Err e){}

      throw Err() << "geo_mkref: can't parse coords option: " << o.get("coords", string());

      coord_end:;
    }

    // Override border
    if (o.exists("border"))
      brd = o.get("border", dMultiLine());

    // here tile_range should be set to non-zero rectangle
    // border may be set
    if (tile_range.empty() || tile_range.zsize())
      throw Err() << "geo_mkref: empty tile range, use coords or tiles opotions";

    // z-index should be set here
    if (z<0) throw Err() << "geo_mkref: z-index not set";

    //std::cerr << "Z:   " << z << "\n";
    //std::cerr << "RNG: " << tile_range << "\n";
    //std::cerr << "BRD: " << brd << "\n";

    /*** fill map parameters ***/

    // map name
    map.name = type_to_str(tile_range);

    // map projection
    map.proj = "+proj=webmerc +datum=WGS84"; // from libproj-5.1.0
    // map.prog = "+proj=merc +a=6378137 +b=6378137 +nadgrids=@null +no_defs";

    // map magnification
    double mag = o.get("mag",1);

    // find coordinates of opposite corners:
    dPoint tlc = tcalc.tile_to_range(tile_range.tlc(),z).tlc();
    dPoint brc = tcalc.tile_to_range(tile_range.brc(),z).brc();

    // Refpoints:
    map.image_size = iPoint(tile_range.w, tile_range.h)*tcalc.get_tsize() * mag;
    dLine pts_w = rect_to_line(dRect(tlc,brc), false);
    dLine pts_r = rect_to_line(dRect(dPoint(0,0),map.image_size), false);
    map.add_ref(pts_r, pts_w);

    // convert border to map pixels
    if (brd.size()>0){
      // We need ConvGeo because of convenient bck_acc function.
      ConvGeo cnv(map.proj); // webmercator -> wgs84

      map.border = cnv.bck_acc(brd,0.5); // lonlat -> mercator m
      for (auto & l:map.border) for (auto & pt:l)
        pt = tcalc.m_to_px(pt, z); // mercator m -> px
      map.border -= tcalc.get_tsize()*tile_range.tlc();
      map.border *= mag;
    }
    return map;
  }

  /***************************************/
  if (reftype == "proj"){

    // map projection
    map.proj = o.get("proj", string());
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
    dMultiLine brd;

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
      // try border
      catch (Err e){
      }
      try {
        brd = o.get("coords", dMultiLine())/k;
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
        range = cnv.bck_acc(o.get("coords", dRect()),0.5);
        dLine b = cnv.bck_acc(rect_to_line(range, true), 0.5);
        b.open();
        brd.push_back(b);
        goto coord_end_r2;
      }
      // try border
      catch (Err e){
      }
      try {
        brd = cnv.bck_acc(o.get("coords", dMultiLine()),0.5);
        range = brd.bbox();
        goto coord_end_r2;
      }
      catch (Err e){}
      throw Err() << "geo_mkref: can't parse coords option: " << o.get("coords", string());
      coord_end_r2:;
    }

    // check if range is set
    if (range.empty() || range.zsize())
      throw Err() << "geo_mkref: empty coordinate range";

    // Override border
    if (o.exists("border"))
      brd = o.get("border", dMultiLine())/k;

    if (o.exists("border_wgs"))
      brd = cnv.bck_acc(o.get("border", dMultiLine()),0.5);

    /* border and range are set now */

    // Refpoints
    dLine pts_r = rect_to_line(range, false);
    dLine pts_w(pts_r);
    pts_r -= range.tlc();
    cnv.frw(pts_w);
    map.add_ref(pts_r, pts_w);

    // border
    map.border = brd - range.tlc();

    // image_size
    map.image_size = dPoint(range.w, range.h);

    return map;
  }

  /***************************************/
  throw Err() << "mk_ref: unknown reference type: " << reftype;
}
