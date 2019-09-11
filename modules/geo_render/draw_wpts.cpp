#include <vector>
#include <cmath>

#include "draw_wpts.h"
#include "geom/line.h"

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
draw_wpts(CairoWrapper & cr, const iPoint & origin,
         const ConvBase & cnv, const GeoWptList & wpts,
         const Opt & opt){

  WptsDrawTmpl WT(cr, origin, cnv, wpts, opt);
  if (opt.get("wpt_adj", 1))     WT.adjust();
  if (opt.get("wpt_adj_brd", 1)) WT.adjust_brd(cr.bbox());
  WT.draw(cr);
}

/**********************************************************/

WptsDrawTmpl::WptsDrawTmpl(CairoWrapper & cr, const iPoint & origin,
                     const ConvBase & cnv, const GeoWptList & wpts,
                     const Opt & opt){

  // create default waypoint template
  WptDrawTmpl wt0;
  wt0.text_font = opt.get("wpt_text_font",  "serif");
  wt0.text_size = opt.get("wpt_text_size",  10);
  wt0.text_pad  = opt.get("wpt_text_pad",   2);
  wt0.size      = opt.get("wpt_draw_size",  3);
  wt0.linewidth = opt.get("wpt_line_width", 1);
  wt0.color     = opt.get("wpt_color",      0xFF000000);
  wt0.bgcolor   = opt.get("wpt_bgcolor",    0xFFFFFFFF);

  // default bar length
  int wpt_bar    = 10;

  cr->save();
  cr->set_fc_font(wt0.color, wt0.text_font.c_str(), wt0.text_size);
  for (auto const & w:wpts){

    dPoint pt(w);
    cnv.bck(pt);
    pt-=origin;

    WptDrawTmpl wt(wt0);
    wt.x = pt.x; wt.y = pt.y;
    wt.name = w.name;
    wt.text_pt = pt;
    wt.text_pt.y -= wt.text_size + wt.text_pad + wpt_bar;

    cr->move_to(wt.text_pt);
    wt.text_box = cr->get_text_extents(w.name);
    wt.text_box.expand(wt.text_pad);
//    wt.color = w.opts.get("color", wt.color);
//    wt.bgcolor = w.opts.get("color", wt.bgcolor);

    data.push_back(wt);

    bbox.expand(wt.text_box);
    bbox.expand(wt.text_pt);
    bbox.expand((dPoint)wt);
  }
  cr->restore();
}

#include "geohash/storage.h"

/* adjust text positions */
void
WptsDrawTmpl::adjust(){

  // create geohash storage
  GeoHashStorage db;
  db.set_bbox(bbox);

  // but all text boxes in the storage
  for (int i=0; i<data.size(); ++i)
    db.put(i, data[i].text_box);

  // for each box
  for (int i=0; i<data.size(); ++i){
    std::set<int> v = db.get(data[i].text_box);
    // find box with smaller number which may touch this one
    int i0 = -1;
    for (auto j:v){
      if (j>=i || intersect(data[j].text_box,data[i].text_box).empty()) continue;
      else i0=j;
    }
    if (i0==-1) continue;
    // delete old position drom the db
    db.del(i, data[i].text_box);
    // adjust box position
    dPoint shift(
      data[i0].text_box.x - data[i].text_box.x + 10,
      data[i0].text_box.y - data[i].text_box.y + data[i0].text_box.h + 1
    );
    data[i].text_box += shift;
    data[i].text_pt += shift;
    // put new position in the database
    db.put(i, data[i].text_box);
    i--;
  }
}

void
WptsDrawTmpl::adjust_brd(const dRect & rng){
  for (auto & w:data){
    dPoint sh(0,0);
    if (w.text_box.x < rng.x) sh.x = rng.x - w.text_box.x;
    if (w.text_box.y < rng.y) sh.y = rng.y - w.text_box.y;

    if (w.text_box.x + w.text_box.w > rng.x + rng.w)
      sh.x = rng.x + rng.w - w.text_box.x - w.text_box.w;
    if (w.text_box.y + w.text_box.h > rng.y + rng.h)
      sh.y = rng.y + rng.h - w.text_box.y - w.text_box.h;
    w.text_box += sh;
    w.text_pt += sh;
  }
}

/* plot template */
void
WptsDrawTmpl::draw(CairoWrapper & cr) const{
  cr->save();
  for (auto const & wt:data){

    cr->set_line_width(wt.linewidth);

    // circle
    cr->circle(wt, wt.size);

    // flag bar
    cr->move_to(wt);
    cr->line_to(wt.text_pt);

    cr->set_color(wt.color);
    cr->stroke();

    // flag
    cr->rectangle(wt.text_box);
    cr->set_color(wt.bgcolor);
    cr->fill_preserve();

    cr->set_color(wt.color);
    cr->stroke();

    // text
    cr->move_to(wt.text_pt);
    cr->set_fc_font(wt.color, wt.text_font.c_str(), wt.text_size);
    cr->show_text(wt.name);

  }
  cr->restore();
}

