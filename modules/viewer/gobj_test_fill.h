#ifndef GOBJ_TEST_FILL_H
#define GOBJ_TEST_FILL_H

#include "gobj.h"

// An example of GObj: fill imeage with a solid color.

class GObjFill: public GObj{
  int color;

public:
  GObjFill(const int c=0xFF000000): color(c) { }

  int draw(const CairoWrapper & cr, const dRect & draw_range) override{
    if (color>>24 == 0) return GObj::FILL_NONE;
    cr->set_color(color);
    cr->paint();
    return (color>>24 == 0xFF) ? GObj::FILL_ALL : GObj::FILL_PART;
  }
};

#endif
