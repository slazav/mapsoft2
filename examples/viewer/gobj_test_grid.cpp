#include "gobj_test_grid.h"
#include <unistd.h> // usleep

GObjTestGrid::GObjTestGrid(const int delay_):delay(delay_){}

int
GObjTestGrid::draw(Image &img, const iPoint &origin){
  if (img.bpp() != 32) throw Err() << "gobj_test_tile: 32-bpp image expected";

  for (int j=0; j<img.height(); j++){
    for (int i=0; i<img.width(); i++){
      img.set(i,j,0xFF<<24);

      int x=origin.x+i, y=origin.y+j;
      for (int n=256; n>1; n/=2){
        if ((x%n==0) || (y%n==0)){
          n--;
          uint32_t val = (0xFF<<24) + (n<<16) + (n<<8) + n;
          img.set(i,j, val);
          break;
        }
      }
    }
  }
  if (delay) usleep(delay);
  return GObj::FILL_ALL;
}

