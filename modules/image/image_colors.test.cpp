///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include <iomanip>
#include "image_colors.h"
#include "err/assert_err.h"

int
main(){
  try{

    assert(image_reduce_color_enc(0xF0F0F0F0) == 0xFFFF0000);
    assert(image_reduce_color_enc(0xF1F1F1F1) == 0xFFFF000F);
    assert(image_reduce_color_enc(0x11111111) == 0x000F000F);
    assert(image_reduce_color_enc(0x01000000) == 0x00000008);
    assert(image_reduce_color_enc(0x00010000) == 0x00000004);
    assert(image_reduce_color_enc(0x00000100) == 0x00000002);
    assert(image_reduce_color_enc(0x00000001) == 0x00000001);

    assert(image_reduce_color_dec(0xFFFF0000) == 0xF0F0F0F0);
    assert(image_reduce_color_dec(0xFFFF000F) == 0xF1F1F1F1);
    assert(image_reduce_color_dec(0x000F000F) == 0x11111111);
    assert(image_reduce_color_dec(0x00000008) == 0x01000000);
    assert(image_reduce_color_dec(0x00000004) == 0x00010000);
    assert(image_reduce_color_dec(0x00000002) == 0x00000100);
    assert(image_reduce_color_dec(0x00000001) == 0x00000001);

    assert(color_dist(0xFFFFFFFF, 0xFEFEFEFE) == 2);
    assert(color_dist(0xFFFFFFFF, 0xFFFFFFFE) == 1);
    assert(color_dist(0xFEFFFFFF, 0xFFFFFFFF) == 1);


    uint32_t colors[256];

    Image img(300,10,32, 0xFF000000);
    assert(image_classify_color(img, colors)==0);
    assert(image_classify_alpha(img)==0);

    img.set<uint32_t>(5,5,0xFF0000FF);
    assert(image_classify_color(img, colors)==1);
    assert(image_classify_alpha(img)==0);

    img.set<uint32_t>(6,6,0x000000FF);
    assert(image_classify_alpha(img)==1);

    img.set<uint32_t>(8,8,0x100000FF);
    assert(image_classify_alpha(img)==2);

    for (int i=0; i<300; i++){
      img.set<uint32_t>(i,1,0xFF000000 + i);
    }
    assert(image_classify_color(img, colors)==2);
    assert(image_classify_alpha(img)==2);

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
