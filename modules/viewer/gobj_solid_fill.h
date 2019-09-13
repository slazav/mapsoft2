#ifndef GOBJ_SOLID_FILL_H
#define GOBJ_SOLID_FILL_H

#include "gobj.h"

// An example of GObj: fill imeage with a solid color.

class GObjSolidFill: public GObj{
  int color;

public:
  GObjSolidFill(ConvBase & cnv, const int c=0xFF000000);
  int draw(const CairoWrapper & cr, const dRect & draw_range) override;
};

#endif
