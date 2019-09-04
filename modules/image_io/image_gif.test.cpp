///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "image_gif.h"
#include "err/assert_err.h"

int
main(){
  try{

    // size
    assert_err(image_size_gif("test_data/missing"),
      "GIF error: can't open file: test_data/missing");
    assert_err(image_size_gif("image_gif.test.cpp"),
      "GIF error: can't open file: image_gif.test.cpp");
    assert(image_size_gif("test_data/image.gif") == iPoint(61,64));

    // load
    assert_err(image_load_gif("test_data/missing"),
      "GIF error: can't open file: test_data/missing");
    assert_err(image_load_gif("image_gif.test.cpp"),
      "GIF error: can't open file: image_gif.test.cpp");
    Image img = image_load_gif("test_data/image.gif", 1);

    // save
    assert_err(image_save_gif(img, "test_data/"),
      "GIF error: can't open file: test_data/");
    image_save_gif(img, "test_data/image.tmp.gif");
    image_save_gif(img, "test_data/image.tmp.gif"); // existing file

    assert(img.width()==61);
    assert(img.height()==64);
    assert(img.get<int>(0,0)  == 0xFFFFFFFF);
    assert(img.get<int>(7,8)  == 0xFF000000);
    assert(img.get<int>(7,24) == 0xFFFF0000);
    assert(img.get<int>(7,36) == 0xFF0000FF);
    assert(img.get<int>(21,8) == 0x00000000);

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond