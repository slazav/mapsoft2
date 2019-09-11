#include <sstream>

#include "geo_data/conv_geo.h"
#include "geo_data/geo_utils.h"
#include "geom/line.h"
#include "cache/cache.h"

#include "draw_pulk_grid.h"

using namespace std;

Cache<int, ConvGeo> draw_pulk_grid_convs(10); // lon0 -> Conv

void
ms2opt_add_drawgrd(ext_option_list & opts){
  int m = MS2OPT_DRAWGRD;
  ext_option_list add = {
  {"grid_draw_color", 1,0,m, "grid color (default 0x80000000)"},
  {"grid_draw_thick", 1,0,m, "grid line thickness (default 1)"},
  {"grid_text_color", 1,0,m, "grid text color (default 0xff000000)"},
  {"grid_text_font",  1,0,m, "grid text color (default \"serif:bold\")"},
  {"grid_text_size",  1,0,m, "grid text size, pixels (default 15)"},
  };
  opts.insert(opts.end(), add.begin(), add.end());
}

/********************************************************************/

void
draw_pulk_grid(CairoWrapper & cr, const iPoint & origin,
               const ConvBase & cnv, const Opt & opt){

  /* find wgs coordinate range and 6 degree zones */
  dRect rng = cr.bbox() + origin;
  dRect rng_wgs = cnv.frw_acc(rng); // screen -> wgs
  int lon0a = lon2lon0(rng_wgs.tlc().x);
  int lon0b = lon2lon0(rng_wgs.brc().x);

  int text_size = opt.get("grid_text_size",  15);
  cr->set_color_a(opt.get("grid_draw_color", 0x8000000));
  cr->set_line_width(opt.get("grid_draw_thick", 1));
  cr->set_fc_font(opt.get("grid_text_color", 0xFF000000),
                  opt.get("grid_text_font",  "serif:bold").c_str(),
                  text_size);
  int text_space = 5;

  /* for all zones */
  for (int lon0=lon0a; lon0<=lon0b; lon0+=6){
    /* build  pulkovo -> wgs conversion or get it from the cache */
    if (!draw_pulk_grid_convs.contains(lon0))
      draw_pulk_grid_convs.add(lon0, ConvGeo(GEO_PROJ_SU(lon0)));

    ConvGeo cnv1(draw_pulk_grid_convs.get(lon0));
    ConvMulti cnv2(&cnv,&cnv1, 1,0); // map -> pulk

    dRect rng_pulk = cnv1.bck_acc(rng_wgs); // wgs -> pulkovo

    // Grid step. We want to have ~1 line/ 200 px
    int nlines = rng.w/200;
    double step = 1000;
    while (rng_pulk.w > step*nlines) step*=10.0;
    while (rng_pulk.w < step*nlines) step/=10.0;

    dPoint tlc = rng_pulk.tlc(), brc= rng_pulk.brc();
    double xmin = floor(tlc.x/step)*step;
    double xmax = ceil(brc.x/step)*step;
    double ymin = floor(tlc.y/step)*step;
    double ymax = ceil(brc.y/step)*step;

    cr->save();

    if (lon0a!=lon0b){ /* clip the zone */
      dRect rng_clip(lon0-3.0, rng_wgs.y, 6.0, rng_wgs.h);
      rng_clip.y-=rng_clip.h*0.2;
      rng_clip.h+=rng_clip.h*0.4;
      cr->mkpath(cnv.bck_acc(rect_to_line(rng_clip,true)) - origin);
      cr->clip_preserve();
      cr->stroke();
    }

    // draw lines
    for (double x=xmin; x<=xmax; x+=step){
      dLine l;
      l.push_back(dPoint(x, tlc.y));
      l.push_back(dPoint(x, brc.y));
      cr->mkpath(cnv2.bck_acc(l), false);
    }
    for (double y=ymin; y<=ymax; y+=step){
      dLine l;
      l.push_back(dPoint(tlc.x, y));
      l.push_back(dPoint(brc.x, y));
      cr->mkpath(cnv2.bck_acc(l), false);
    }
    cr->stroke();

    // draw labels
    step*=2;
    xmin = floor(tlc.x/step)*step;
    xmax = ceil(brc.x/step)*step;
    ymin = floor(tlc.y/step)*step;
    ymax = ceil(brc.y/step)*step;

    for (double x=xmin; x<=xmax; x+=step){
      for (double y=ymin; y<=ymax; y+=step){
        stringstream sx, sy;
        sx << int(x/1000), sy << int(y/1000);
        dPoint p1(x,y);
        cnv2.bck(p1); // pulkovo -> wgs
        double a = cnv2.frw_ang(p1, 0, 1000);
        cr->save();
        cr->translate(p1-origin);
        cr->rotate(a);
        cr->move_to(dPoint(text_space,-text_space));
        cr->show_text(sy.str());

        dRect e = cr->get_text_extents(sx.str());
        cr->move_to(dPoint(text_space+e.h, -2*text_space-e.h));
        cr->rotate(-M_PI/2.0);
        cr->show_text(sx.str());
        cr->restore();
      }
    }
    cr->restore();

  }
}


