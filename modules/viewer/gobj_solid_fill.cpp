#include "gobj_solid_fill.h"

GObjSolidFill::GObjSolidFill(std::shared_ptr<ConvBase> cnv, const int c):
     GObj(cnv), color(c) { }

int
GObjSolidFill::draw(const CairoWrapper & cr, const dRect & draw_range){

  if (color>>24 == 0) return GObj::FILL_NONE;
  cr->set_color(color);
  cr->paint();
  return (color>>24 == 0xFF) ? GObj::FILL_ALL : GObj::FILL_PART;
}

