///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include <iomanip>
#include "image_colors.h"
#include "err/assert_err.h"

int
main(){
  try{

    assert(color_dist(0xFFFFFFFF, 0xFEFEFEFE) == 2);
    assert(color_dist(0xFFFFFFFF, 0xFFFFFFFE) == 1);
    assert(color_dist(0xFEFFFFFF, 0xFFFFFFFF) == 1);

    assert(color_rem_transp(0x80FFFF00, 0) == 0xFFFFFF00);
    assert(color_rem_transp(0x80808000, 0) == 0xFFFFFF00);
    assert(color_rem_transp(0x00FF00FF, 0) == 0xFFFFFFFF);
    assert(color_rem_transp(0xFF00FFFF, 0) == 0xFF00FFFF);

    assert(color_rem_transp(0x80FFFF00, 1) == 0xFFFFFF00);
    assert(color_rem_transp(0x80808000, 1) == 0xFFFFFF00);
    assert(color_rem_transp(0x00FF00FF, 1) == 0x00000000);
    assert(color_rem_transp(0xFF00FFFF, 1) == 0xFF00FFFF);

    assert(color_argb(0,1,2,3) == 0x00000000);
    assert(color_argb(0xFF,1,2,3) == 0xFF010203);
    assert(color_argb(0x80,2,4,6) == 0x80010203);

    uint32_t colors[256];

    Image img(300,10,IMAGE_32ARGB);
    img.fill32(0xFF000000);
    assert(image_classify_color(img, colors)==0);
    assert(image_classify_alpha(img)==0);

    img.set32(5,5,0xFF0000FF);
    assert(image_classify_color(img, colors)==1);
    assert(image_classify_alpha(img)==0);

    img.set32(6,6,0x000000FF);
    assert(image_classify_alpha(img)==1);

    img.set32(8,8,0x100000FF);
    assert(image_classify_alpha(img)==2);

    for (int i=0; i<300; i++){
      img.set32(i,1,0xFF000000 + i);
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
