#include "gobj_solid_fill.h"

GObjSolidFill::GObjSolidFill(const int c):
     GObj(ConvBase()), color(c) { }

int
GObjSolidFill::draw(const CairoWrapper & cr,
                    const iPoint &origin){

  if (color>>24 == 0) return GObj::FILL_NONE;
  cr->set_color(color);
  cr->paint();
  return (color>>24 == 0xFF) ? GObj::FILL_ALL : GObj::FILL_PART;
}

