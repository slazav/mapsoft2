///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include <sstream>
#include "image.h"

#define assert_err(cmd,ret)\
  try{ cmd; assert(false); } catch (Err e) {\
    if (e.str()!=ret) std::cerr<<e.str()<<"\n";\
    assert(e.str()==(ret));}

int
main(){
  try{

    {
      Image im1;
      assert(im1.width() == 0);
      assert(im1.height() == 0);
      assert(im1.bpp() == 0);
      assert(im1.is_empty());

      Image im2(100,100, 32);

      std::ostringstream s1; s1 << im1;
      assert(s1.str() == "Image(empty)");

      std::ostringstream s2; s2 << im2;
      assert(s2.str() == "Image(100x100x32bpp)");

      assert(im2.width() == 100);
      assert(im2.height() == 100);
      assert(im2.bpp() == 32);
      assert(!im2.is_empty());
    }

    {
      assert_err(Image im2(100,0, 1),    "non-positive image dimension: 100x0");
      assert_err(Image im2(0,1,   1),    "non-positive image dimension: 0x1");
      assert_err(Image im2(100,100, 2),  "unsuported bpp for image: 2");
      assert_err(Image im2(100,100, 0),  "unsuported bpp for image: 0");
      assert_err(Image im2(100,-1, 32),  "can't allocate memory for "
                             "Image(100x18446744073709551615x32bpp): std::bad_alloc");
    }

    { // 1bpp image
      Image im(100,100, 1);
      assert(im.width() == 100);
      assert(im.height() == 100);
      assert(im.bpp() == 1);

      im.set_int(0,9, 1);
      im.set_int(2,9, 5);
      im.set_int(2,0, 2);
      im.set_int(2,1, 0);
      assert(im.get_int<int>(0,9) == 1);
      assert(im.get_int<int>(2,9) == 1);
      assert(im.get_int<int>(2,0) == 1);
      assert(im.get_int<int>(2,1) == 0);

      im.fill_int(0);
      assert(im.get_int<int>(2,9) == 0);

      im.fill_int(1);
      assert(im.get_int<int>(2,9) == 1);
    }

    { // 8bpp image
      Image im(100,100, 8);
      assert(im.width() == 100);
      assert(im.height() == 100);
      assert(im.bpp() == 8);

      // get_int, set_int can work with other types

      im.set_int(0,9, 1);
      im.set_int(2,9, 5);
      im.set_int(2,0, 255);
      im.set_int(2,1, 0x1FF);
      im.set_int(2,2, -1);
      im.set_int(2,3, -2);
      assert(im.get_int<int>(0,9) == 1);
      assert(im.get_int<int>(2,9) == 5);
      assert(im.get_int<int>(2,0) == 255);
      assert(im.get_int<int>(2,1) == 255);
      assert(im.get_int<int>(2,2) == 255);
      assert(im.get_int<int>(2,3) == 254);

      assert(im.get_int<int>(2,2) == 255);

      assert(im.get<int8_t>(0,9) == 1);
      assert(im.get<int8_t>(2,9) == 5);
      assert(im.get<int8_t>(2,0) == -1);
      assert(im.get<int8_t>(2,1) == -1);
      assert(im.get<int8_t>(2,2) == -1);
      assert(im.get<uint8_t>(2,2) == 255);
      assert(im.get<int8_t>(2,3) == -2);
    }

    { // 32bpp image with filling
      Image im(1640,480, 32, 0xFF000010);
      assert(im.width() == 1640);
      assert(im.height() == 480);
      assert(im.bpp() == 32);

      im.set_int(0,9, 1);
      im.set_int(2,9, 5);
      im.set_int(2,0, 2);
      assert(im.get_int<int>(0,9) == 1);
      assert(im.get_int<int>(2,9) == 5);
      assert(im.get_int<int>(2,0) == 2);
      assert(im.get_int<int>(3,0) == 0xFF000010);
    }

    { // 64bpp image
      Image im(100,100, 64);
      assert(im.width() == 100);
      assert(im.height() == 100);
      assert(im.bpp() == 64);

      im.set_int(0,9, 0xFF00FF00FF00FF);
      im.set_int(2,9, (int64_t)-1);
      assert(im.get_int<int64_t>(0,9) == 0xFF00FF00FF00FF);
      assert(im.get_int<int64_t>(2,9) == -1);

      assert(im.get_int<int32_t>(0,9) == 0xFF00FF);
      assert(im.get_int<int32_t>(2,9) == -1);
      assert(im.get_int<uint8_t>(2,9) == 255);
      assert(im.get_int<int8_t>(2,9) == -1);
    }

    { // double image
      Image im(100,100, sizeof(double));
      assert(im.width() == 100);
      assert(im.height() == 100);
      assert(im.bpp() == sizeof(double));

      im.set<double>(0,1, 1);
      im.set<double>(0,2, 1e-8);

      assert(im.get<double>(0,1) == 1);
      assert(im.get<double>(0,2) == 1e-8);
    }


  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
