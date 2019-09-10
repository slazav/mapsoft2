///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "image_io.h"
#include "err/assert_err.h"

int
main(){
  try{

    // size
    assert_err(image_size("test_gif/missing"),
      "image_size: unknown format: test_gif/missing");
    assert_err(image_size("test_gif/missing.gif"),
      "image_size_gif: can't open file: test_gif/missing.gif");
    assert_err(image_size("test_gif/Readme.md"),
      "image_size: unknown format: test_gif/Readme.md");

    // load
    assert_err(image_load("test_gif/missing"),
      "image_load: unknown format: test_gif/missing");
    assert_err(image_load("test_gif/missing.gif"),
      "image_load_gif: can't open file: test_gif/missing.gif");
    assert_err(image_load("test_gif/Readme.md"),
      "image_load: unknown format: test_gif/Readme.md");

    /*********************************************/
    // Original image
    Image img(256,128, IMAGE_32ARGB);
    for (int y=0; y<128; ++y){
      for (int x=0; x<128; ++x){
        img.set32(x,y,     color_argb(0xFF, 2*x, 2*y, 0));
        img.set32(128+x,y, color_argb(2*x,  2*y, 0,   0));
      }
    }
    Image img1;

    image_save(img, "test_data/img.tmp.png");
    img1 = image_load("test_data/img.tmp.png");
    assert(image_size("test_data/img.tmp.png") == img.size());
    assert(img1.size() == img.size());

    image_save(img, "test_data/img.tmp.jpg");
    img1 = image_load("test_data/img.tmp.jpg");
    assert(image_size("test_data/img.tmp.jpg") == img.size());
    assert(img1.size() == img.size());

    image_save(img, "test_data/img.tmp.gif");
    img1 = image_load("test_data/img.tmp.gif");
    assert(image_size("test_data/img.tmp.gif") == img.size());
    assert(img1.size() == img.size());

    image_save(img, "test_data/img.tmp.tiff");
    img1 = image_load("test_data/img.tmp.tiff", 2);
    assert(image_size("test_data/img.tmp.tiff") == img.size());
    assert(img1.size() == img.size()/2);


  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond