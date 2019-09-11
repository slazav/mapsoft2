#include "gobj_solid_fill.h"

GObjSolidFill::GObjSolidFill(const int c): GObj(ConvBase()), color(c) {
}

int
GObjSolidFill::draw(Image &img, const iPoint &origin){
  if (img.type() != IMAGE_32ARGB)
    throw Err() << "gobj_test_tile: 32-bpp image expected";

  if (color>>24 == 0){
    return GObj::FILL_NONE;
  }
  img.fill32((uint32_t)color);
  return (color>>24 == 0xFF) ? GObj::FILL_ALL : GObj::FILL_PART;
}

