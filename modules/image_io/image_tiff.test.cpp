///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "image_tiff.h"
#include "err/assert_err.h"

int
main(){
  try{

    // size
    assert_err(image_size_tiff("test_data/missing"),
      "TIFFOpen error: test_data/missing: No such file or directory");
    assert_err(image_size_tiff("test_data/image.gif"),
      "test_data/image.gif error: Not a TIFF or MDI file, bad magic number 18759 (0x4947)");
    assert(image_size_tiff("test_data/image.tif") == iPoint(61,64));

    // load
    assert_err(image_load_tiff("test_data/missing"),
      "TIFFOpen error: test_data/missing: No such file or directory");
    assert_err(image_load_tiff("test_data/image.gif"),
      "test_data/image.gif error: Not a TIFF or MDI file, bad magic number 18759 (0x4947)");
    // 8bit
    Image img1 = image_load_tiff("test_data/image.tif", 1);

    assert(img1.width()==61);
    assert(img1.height()==64);
    assert(img1.get32(0,0)  == 0xFFFFFFFF);
    assert(img1.get32(7,8)  == 0xFF000000);
    assert(img1.get32(7,24) == 0xFFFF0000);
    assert(img1.get32(7,36) == 0xFF0000FF);
//    assert(img1.get32(21,7) == 0x00000000);

    // 32bit, rgba
    Image img2 = image_load_tiff("test_data/image_rgba.tif", 1);

    assert(img2.get32(0,0)  == 0xFFFFFFFF);
    assert(img2.get32(7,8)  == 0xFF000000);
    assert(img2.get32(7,24) == 0xFFFF0000);
    assert(img2.get32(7,36) == 0xFF0000FF);
    assert(img2.get32(21,7) == 0x00000000);
    assert(img2.get32(22,22) == 0x80808080);
    assert(img2.get32(22,36) == 0x7f7f0000);
    assert(img2.get32(38,22) == 0x7f00007f);
    assert(img2.get32(38,36) == 0x7f000000);
    assert(img2.get32(60,63) == 0xFFFFFFFF);

/*
    // save
    assert_err(image_save_tiff(img, "test_data/"),
      "TIFF error: can't open file: test_data/");
    image_save_tiff(img, "test_data/image.tmp.tiff");
    image_save_tiff(img, "test_data/image.tmp.tiff"); // existing file
*/

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond