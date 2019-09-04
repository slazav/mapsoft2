///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "image_png.h"
#include "err/assert_err.h"

int
main(){
  try{

    // size
    assert_err(image_size_png("test_data/missing"),
      "PNG error: can't open file: test_data/missing");
    assert_err(image_size_png("test_data/image.gif"),
      "PNG error: not a PNG file: test_data/image.gif");
    assert(image_size_png("test_data/image.png") == iPoint(61,64));


    // load
    assert_err(image_load_png("test_data/missing"),
      "PNG error: can't open file: test_data/missing");
    assert_err(image_load_png("test_data/image.gif"),
      "PNG error: not a PNG file: test_data/image.gif");
    // 8bit
    Image img1 = image_load_png("test_data/image.png", 1);
    // 32bit, rgba
    Image img2 = image_load_png("test_data/image_rgba.png", 1);
/*
    // save
    assert_err(image_save_png(img, "test_data/"),
      "PNG error: can't open file: test_data/");
    image_save_png(img, "test_data/image.tmp.png");
    image_save_png(img, "test_data/image.tmp.png"); // existing file
*/

    assert(img1.width()==61);
    assert(img1.height()==64);
    assert(img1.get<int>(0,0)  == 0xFFFFFFFF);
    assert(img1.get<int>(7,8)  == 0xFF000000);
    assert(img1.get<int>(7,24) == 0xFFFF0000);
    assert(img1.get<int>(7,36) == 0xFF0000FF);
    assert(img1.get<int>(21,7) == 0x00000000);

    assert(img2.get<int>(0,0)  == 0xFFFFFFFF);
    assert(img2.get<int>(7,8)  == 0xFF000000);
    assert(img2.get<int>(7,24) == 0xFFFF0000);
    assert(img2.get<int>(7,36) == 0xFF0000FF);
    assert(img2.get<int>(21,7) == 0x00000000);
    assert(img2.get<int>(22,22) == 0x80808080);

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond