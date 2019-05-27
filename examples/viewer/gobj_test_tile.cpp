#include "gobj_test_tile.h"
#include "err/err.h"
#include <unistd.h> // usleep

GObjTestTile::GObjTestTile(const bool slow_):slow(slow_){}

int
GObjTestTile::draw(Image &img, const iPoint &origin){
  if (img.bpp() != 32) throw Err() << "gobj_test_tile: 32-bpp image expected";
  for (int j=0; j<img.height(); j++){
    for (int i=0; i<img.width(); i++){
      uint32_t val = (0xFF << 24) + ((i*256)/img.width() << 16) + ((j*256)/img.height() << 8);
      img.set(i,j, val);
    }
  }
  if (slow) usleep(img.width()*img.height()*10);
  return GObj::FILL_ALL;
}

