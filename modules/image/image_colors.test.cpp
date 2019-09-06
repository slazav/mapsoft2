///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include <iomanip>
#include "image_colors.h"
#include "err/assert_err.h"

int
main(){
  try{

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
