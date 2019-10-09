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
    assert_eq(image_classify_color(img, colors),0);
    assert_eq(image_classify_alpha(img),0);

    img.set32(5,5,0xFF0000FF);
    assert_eq(image_classify_color(img, colors),1);
    assert_eq(image_classify_alpha(img),0);

    img.set32(6,6,0x000000FF);
    assert_eq(image_classify_alpha(img),1);

    img.set32(8,8,0x100000FF);
    assert_eq(image_classify_alpha(img),2);

    for (int i=0; i<300; i++){
      img.set32(i,1,0xFF000000 + i);
    }
    assert_eq(image_classify_color(img, colors),2);
    assert_eq(image_classify_alpha(img),2);


    // apply border
    img = Image(256,128, IMAGE_32ARGB);
    int c = 0xFF203040;
    img.fill32(c);
    dLine brd("[[-10,-10], [64,-10], [128,64], [-10,100]]");
    image_apply_border(img, brd, 0);
    assert_eq(img.get32(0,0), c);
    assert_eq(img.get32(64,0), c);
    assert_eq(img.get32(100,0), 0);
    assert_eq(img.get32(0,101), 0);
    assert_eq(img.get32(0,64), c);
    assert_eq(img.get32(130,64), 0);

    // apply border
    img.fill32(c);
    brd = iLine("[[10,10], [64,10], [128,64], [10,100]]");
    image_apply_border(img, brd, 0);
    assert_eq(img.get32(0,0), 0);
    assert_eq(img.get32(64,0), 0);
    assert_eq(img.get32(20,20), c);
    assert_eq(img.get32(0,101), 0);
    assert_eq(img.get32(0,64), 0);

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
