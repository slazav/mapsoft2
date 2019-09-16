#include "gobj_test_grid.h"
#include <unistd.h> // usleep

GObjTestGrid::GObjTestGrid(const int delay_, std::shared_ptr<ConvBase> c):
   GObj(c), delay(delay_){}

int
GObjTestGrid::draw(const CairoWrapper & cr, const dRect &box){

  Image img = cr.get_image();
  if (img.is_empty()) return GObj::FILL_NONE;

  for (int j=0; j<box.h; j++){
    for (int i=0; i<box.w; i++){
      img.set32(i,j,0xFF<<24);

      int x=box.x+i, y=box.y+j;
      for (int n=256; n>1; n/=2){
        if ((x%n==0) || (y%n==0)){
          n--;
          uint32_t val = (0xFF<<24) + (n<<16) + (n<<8) + n;
          img.set32(i,j, val);
          break;
        }
      }
    }
  }

  if (delay) usleep(delay);
  return GObj::FILL_ALL;
}

