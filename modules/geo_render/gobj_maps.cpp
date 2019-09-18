#include <vector>
#include <cmath>

#include "geohash/storage.h"
#include "geom/line.h"
#include "geo_data/conv_geo.h"

#include "gobj_maps.h"

using namespace std;

void
ms2opt_add_drawmap(ext_option_list & opts){
/*
  int m = MS2OPT_DRAWMAP;
  ext_option_list add = {
  {"wpt_text_font",   1,0,m, "waypoint font (default: \"serif\")"},
  {"wpt_text_size",   1,0,m, "waypoint font size, pixels (default: 10)"},
  {"wpt_text_pad",    1,0,m, "waypoint text padding, pixels (default: 2)"},
  {"wpt_draw_size",   1,0,m, "waypoint dot radius, pixels (default: 3)"},
  {"wpt_line_width",  1,0,m, "waypoint line width), (default: 1)"},
  {"wpt_color",       1,0,m, "waypoint color (default: 0xFF000000)"},
  {"wpt_bgcolor",     1,0,m, "waypoint background color (default: 0xFFFFFFFF)"},
  {"wpt_adj",         1,0,m, "adjust waypoint flag positions to prevent collisions (default: 1)"},
  {"wpt_adj_brd",     1,0,m, "adjust waypoint flag positions to prevent boundary collisions (default: 0)"},
  };
  opts.insert(opts.end(), add.begin(), add.end());
*/
}

/**********************************************************/

GObjMaps::GObjMaps(GeoMapList & maps):
    maps(maps), img_cache(10), tiles(128), smooth(true) {

  for (auto & m:maps){
    MapData d;
    m.update_size();
    d.src_bbox = m.bbox();
    d.src = &m;
    data.push_back(d);
  }
}

int
GObjMaps::draw(const CairoWrapper & cr, const dRect & draw_range) {

  if (stop_drawing) return GObj::FILL_NONE;

  if (intersect(draw_range, range).is_zsize()) return GObj::FILL_NONE;

  for (auto const & d:data){

    if (stop_drawing) return GObj::FILL_NONE;

    dRect range_dst = intersect(draw_range, d.bbox);
    if (range_dst.is_zsize()) continue;

    range_dst.to_ceil();
    iRect key = range_dst;

    if (!tiles.contains(key)) {
      Image image_src = img_cache.get(d.src->image, d.load_sc);
      Image image_dst = Image(range_dst.w, range_dst.h, IMAGE_32ARGB);

      double avr = d.scale/d.load_sc/2;

      // render image
      for (int yd=0; yd<image_dst.height(); ++yd){
        if (stop_drawing) return GObj::FILL_NONE;
        for (int xd=0; xd<image_dst.width(); ++xd){
          dPoint p(xd,yd);
          p += range_dst.tlc();
          d.cnv.frw(p);
          int color;
          if (smooth){
            if (avr<1) color = image_src.get_argb_int4(p);
            else       color = image_src.get_argb_avrg(p, avr);
          }
          else {
            color = image_src.get_argb_safe(p);
          }
          image_dst.set32(xd, yd, color);
        }
      }
      tiles.add(key, image_dst);
    }

    // border
    cr->reset_clip();
    if (d.brd.size()>0){
      cr->mkpath(d.brd);
      cr->clip();
    }

    cr->set_source(image_to_surface(tiles.get(key)),
      range_dst.x, range_dst.y);
    cr->paint();
  }
  return GObj::FILL_PART;
}

/**********************************************************/

void
GObjMaps::on_set_cnv(){

  range = dRect();
  for (auto & d:data){

    // conversion viewer->map
    d.cnv.reset();
    if (cnv) d.cnv.push_back(cnv, true); // viewer -> WGS
    d.cnv.push_back(
      std::shared_ptr<ConvMap>(new ConvMap(*d.src)),
      false);

    // border in viewer coordinates
    d.brd = d.cnv.bck_acc(d.src->border);

    // map bbox in viewer coordinates
    d.bbox = d.cnv.bck_acc(d.src_bbox);
    range.expand(d.bbox);

    // simplify the conversion if possible
    d.simp = d.cnv.simplify(d.bbox, 5, 0.5);

    // calculate map scale (map pixels per viewer pixel)
    dPoint sc = d.cnv.scales(d.bbox);
    d.scale = 1.0/std::max(sc.x, sc.y);

    // scale for image loading
    d.load_sc = floor(0.5*d.scale + 0.05);
    if (d.load_sc <=0) d.load_sc = 1;
  }
  tiles.clear();
}

void
GObjMaps::on_rescale(double k){
  for (auto & d:data){
    d.brd*=k;
    d.bbox*=k;
    d.scale/=k;
    if (d.simp) d.cnv.rescale_src(1.0/k);
    // scale for image loading
    d.cnv.rescale_dst(d.load_sc);
    d.load_sc = floor(0.5*d.scale + 0.05);
    if (d.load_sc <=0) d.load_sc = 1;
    d.cnv.rescale_dst(1.0/d.load_sc);
  }
  range*=k;
  tiles.clear();
}
