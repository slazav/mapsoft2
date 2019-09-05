///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "image_jpeg.h"
#include "image/image_colors.h"
#include "err/assert_err.h"

int
main(){
  try{

    // size
    assert_err(image_size_jpeg("test_data/missing"),
      "JPEG error: can't open file: test_data/missing");
    assert_err(image_size_jpeg("test_data/image.gif"),
      "JPEG error: Not a JPEG file: starts with 0x47 0x49");
    assert(image_size_jpeg("test_data/image.jpg") == iPoint(61,64));

    // load
    assert_err(image_load_jpeg("test_data/missing"),
      "JPEG error: can't open file: test_data/missing");
    assert_err(image_load_jpeg("test_data/image.gif"),
      "JPEG error: Not a JPEG file: starts with 0x47 0x49");
    Image img = image_load_jpeg("test_data/image.jpg", 1);

    // save
    assert_err(image_save_jpeg(img, "test_data/image.tmp.jpg", 195),
      "JPEG error: quality not in range 0..100: 195");
    assert_err(image_save_jpeg(img, "test_data/", 95),
      "JPEG error: can't open file: test_data/");
    image_save_jpeg(img, "test_data/image.tmp.jpg", 95);
    image_save_jpeg(img, "test_data/image.tmp.jpg", 95); // existing file

    assert(img.width()==61);
    assert(img.height()==64);
    assert(color_dist(img.get<int>(0,0),  0xFFFFFFFF) < 10);
    assert(color_dist(img.get<int>(7,8),  0xFF000000) < 10);
    assert(color_dist(img.get<int>(7,24), 0xFFFF0000) < 10);
    assert(color_dist(img.get<int>(7,36), 0xFF0000FF) < 10);
    assert(color_dist(img.get<int>(60,63),0xFFFFFFFF) < 10);

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond