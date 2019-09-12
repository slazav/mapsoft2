#include "gobj_test_tile.h"
#include "err/err.h"
#include <unistd.h> // usleep

GObjTestTile::GObjTestTile(const bool slow_):
  GObj(ConvBase()), slow(slow_){}

int
GObjTestTile::draw(const CairoWrapper & cr, const iPoint &origin){

  Image img = cr.get_image();
  if (img.is_empty()) return GObj::FILL_NONE;

  for (int j=0; j<img.height(); j++){
    for (int i=0; i<img.width(); i++){
      uint32_t val = (0xFF << 24) + ((i*256)/img.width() << 16) + ((j*256)/img.height() << 8);
      img.set32(i,j, val);
    }
  }

  if (slow) usleep(img.width()*img.height()*10);
  return GObj::FILL_ALL;
}

