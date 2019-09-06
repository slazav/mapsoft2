///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "image_jpeg.h"
#include "image_png.h"
#include "image_gif.h"
#include "err/assert_err.h"

int
main(){
  try{

    // load
    Image i1 = image_load_jpeg("test_data/test_fullc1.jpg");
    image_save_gif(i1, "test_data/test_fullc1.tmp.gif");

    Image i2 = image_load_jpeg("test_data/test_fullc2.jpg");
    image_save_gif(i2, "test_data/test_fullc2.tmp.gif");

    Image i3 = image_load_png("test_data/image_rgba.png");
    image_save_gif(i3, "test_data/image_rgba.tmp.gif");

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond