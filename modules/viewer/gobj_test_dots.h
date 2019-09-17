#ifndef GOBJ_TEST_DOTS_H
#define GOBJ_TEST_DOTS_H
#include "gobj.h"

// An axample of GObj: draw 10x10 grid of dots

class GObjDots: public GObj{
  int color;

public:
  GObjDots(const int c=0xFF000000): color(c) { }

  int draw(const CairoWrapper & cr, const dRect & draw_range) override {
    if (color>>24 == 0) return GObj::FILL_NONE;
    cr->set_color(color);
    cr->cap_round();
    cr->set_line_width(3);
    iRect r = 10*floor(cnv->frw_acc(draw_range)/10.0);

    for (int y = r.y; y<=r.y+r.h; y+=10){
      for (int x = r.x; x<=r.x+r.w; x+=10){
        dPoint p(x,y);
        cnv->bck(p);
        cr->move_to(p);
        cr->line_to(p);
      }
    }
    cr->stroke();
    return GObj::FILL_PART;
  }
};

#endif