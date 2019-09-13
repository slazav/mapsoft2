#include <vector>
#include <cmath>

#include "geohash/storage.h"
#include "geom/line.h"

#include "gobj_wpts.h"

using namespace std;

void
ms2opt_add_drawwpt(ext_option_list & opts){

  int m = MS2OPT_DRAWWPT;
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
}


/********************************************************************/

void
draw_wpts(CairoWrapper & cr, const dRect & box,
         ConvBase & cnv, GeoWptList & wpts,
         const Opt & opt){

  GObjWpts gobj(cnv, wpts, opt);
  gobj.draw(cr, box);
}

/**********************************************************/

GObjWpts::GObjWpts(ConvBase & cnv, GeoWptList & wpts, const Opt & opt):
           GObj(cnv), wpts(wpts){
  on_set_opt(opt);
  on_set_cnv();
}

int
GObjWpts::draw(const CairoWrapper & cr, const dRect & draw_range) {

  if (stop_drawing) return GObj::FILL_NONE;

  if (!intersect(draw_range, range)) return GObj::FILL_NONE;

  if (do_adj_brd) adjust_text_brd(draw_range);

  for (auto const & wt:tmpls){

    if (stop_drawing){
      cr->restore();
      return GObj::FILL_NONE;
    }

    if (!intersect(draw_range, wt.bbox)) continue;

    cr->set_line_width(wt.linewidth);

    // circle
    cr->circle(wt, wt.size);

    // flag bar
    cr->move_to(wt);
    cr->line_to(wt.text_pt);

    cr->set_color(wt.color);
    cr->stroke();

    // flag
    cr->rectangle(wt.text_pt+wt.text_box);
    cr->set_color(wt.bgcolor);
    cr->fill_preserve();

    cr->set_color(wt.color);
    cr->stroke();

    // text
    cr->move_to(wt.text_pt);
    cr->set_fc_font(wt.color, wt.text_font.c_str(), wt.text_size);
    cr->show_text(wt.name);

    // bbox
    //cr->rectangle(wt.bbox);
    //cr->stroke();

  }
  return GObj::FILL_PART;
}

/**********************************************************/

void
GObjWpts::update_pt_crd(WptDrawTmpl & wt){
  dPoint pt(*wt.src);
  cnv.bck(pt);
  wt.x = pt.x; wt.y = pt.y;
  wt.text_pt = (dPoint)wt;
  wt.text_pt.y -= wt.text_size + wpt_text_pad + wpt_bar_length;
  update_pt_bbox(wt);
}

void
GObjWpts::update_pt_bbox(WptDrawTmpl & wt){
  wt.bbox = dRect(dPoint(wt), dPoint(wt));
  wt.bbox.expand(wt.size + wt.linewidth);
  wt.bbox.expand(wt.text_pt + wt.text_box);
  wt.bbox.to_ceil();
}

void
GObjWpts::update_pt_name(const CairoWrapper & cr, WptDrawTmpl & wt){
  wt.name = wt.src->name;
  cr->move_to(0,0);
  wt.text_box = cr->get_text_extents(wt.name);
  wt.text_box.expand(wpt_text_pad);
}

void
GObjWpts::update_range(){
  range = dRect();
  for (auto & wt:tmpls) range.expand(wt.bbox);
  range.to_ceil();
}

void
GObjWpts::adjust_text_pos() {
  // create geohash storage
  GeoHashStorage db;
  db.set_bbox(GObj::MAX_RANGE);

  // for each box
  for (int i=0; i<tmpls.size(); ++i){
    dRect bi = tmpls[i].text_box + tmpls[i].text_pt;
    db.put(i, bi);
    std::set<int> v = db.get(bi);
    // find box with smaller number which may touch this one
    int i0 = -1;
    for (auto j:v){
      if (j>=i) continue;
      dRect bj = tmpls[j].text_box + tmpls[j].text_pt;
      if (!intersect(bi,bj)) continue;
      else i0=j;
    }
    if (i0==-1) continue;
    // delete old position drom the db
    db.del(i, bi);
    // adjust box position
    tmpls[i].text_pt = tmpls[i0].text_pt
                     + dPoint(10, tmpls[i0].text_box.h + 2);
    // update bbox
    update_pt_bbox(tmpls[i]);
    // put new position in the tmplsbase
    db.put(i, tmpls[i].text_box + tmpls[i].text_pt);
    i--;
  }
}

void
GObjWpts::adjust_text_brd(const dRect & rng){
  for (auto & wt:tmpls){

    if (!rng.contains(wt)) continue;

    dPoint sh(0,0);
    dRect b = wt.text_box + wt.text_pt;
    if (b.x < rng.x) sh.x = rng.x - b.x;
    if (b.y < rng.y) sh.y = rng.y - b.y;

    if (b.x + b.w > rng.x + rng.w)
      sh.x = rng.x + rng.w - b.x - b.w;
    if (b.y + b.h > rng.y + rng.h)
      sh.y = rng.y + rng.h - b.y - b.h;
    wt.text_pt += sh;
    update_pt_bbox(wt);
  }
}

/**********************************************************/

void
GObjWpts::on_set_opt(const Opt & opt){

  stop_drawing = true;
  auto lock = get_lock();

  wt0.text_font = opt.get("wpt_text_font",  "serif");
  wt0.text_size = opt.get("wpt_text_size",  10);
  wt0.size      = opt.get("wpt_draw_size",  3);
  wt0.linewidth = opt.get("wpt_line_width", 1);
  wt0.color     = opt.get("wpt_color",      0xFF000000);
  wt0.bgcolor   = opt.get("wpt_bgcolor",    0xFFFFFFFF);

  do_adj_pos = opt.get("wpt_adj", 1);
  do_adj_brd = opt.get("wpt_adj_brd", 0);

  wpt_text_pad  = opt.get("wpt_text_pad",   2);
  wpt_bar_length = 10; // default bar length

  CairoWrapper cr;
  cr.set_surface_img(1000,1000);
  cr->save();
  cr->set_fc_font(wt0.color, wt0.text_font.c_str(), wt0.text_size);

  for (auto & w:wpts){
    WptDrawTmpl wt(wt0);
    wt.src = &w;
    update_pt_name(cr, wt); // update name
    tmpls.push_back(wt);
  }
  cr->restore();

  stop_drawing = false;
}

void
GObjWpts::on_set_cnv(){
  // recalculate coordinates, update range

  if (wpts.size()!=tmpls.size())
    throw Err() << "GObjWpts: templates are not syncronized with data";

  for (auto & wt:tmpls) update_pt_crd(wt);

  if (do_adj_pos) adjust_text_pos();
  update_range();

  stop_drawing = false;
}

void
GObjWpts::on_rescale(double k){
  // rescale coordinates, update range

  for (auto & wt:tmpls){
    wt.x*=k; wt.y*=k;
    wt.text_pt = wt;
    wt.text_pt.y -= wt.text_size + wpt_text_pad + wpt_bar_length;
    update_pt_bbox(wt);
  }
  if (do_adj_pos) adjust_text_pos();
  update_range();

  stop_drawing = false;
}

