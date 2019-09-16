#ifndef GOBJ_SOLID_FILL_H
#define GOBJ_SOLID_FILL_H

#include "gobj.h"

// An example of GObj: fill imeage with a solid color.

class GObjSolidFill: public GObj{
  int color;

public:
  GObjSolidFill(
    std::shared_ptr<ConvBase> cnv,
    const int c=0xFF000000): GObj(cnv), color(c) { }

  int draw(const CairoWrapper & cr, const dRect & draw_range) override{
    if (color>>24 == 0) return GObj::FILL_NONE;
    cr->set_color(color);
    cr->paint();
    return (color>>24 == 0xFF) ? GObj::FILL_ALL : GObj::FILL_PART;
  }
};

#endif
