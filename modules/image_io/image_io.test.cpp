///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "image_png.h"
#include "image/image_colors.h"
#include "err/assert_err.h"

int
main(){
  try{

    // IMAGE_32ARGB
    Image img32(256,128, IMAGE_32ARGB);
    img32.fill32(0xFFFF0000);
    for (int y=0; y<128; ++y){
      for (int x=0; x<128; ++x){
        img32.set32(x,y,     color_argb(0xFF, 2*x, 2*y, 0));
        img32.set32(128+x,y, color_argb(2*x,  2*y, 0,   0));
      }
    }

    // IMAGE_24RGB
    Image img24(256,128, IMAGE_24RGB);
    for (int y=0; y<img24.height(); ++y){
      for (int x=0; x<img24.width(); ++x){
        img24.set24(x,y, color_rem_transp(img32.get32(x,y), false));
      }
    }

    // IMAGE_16
    Image img16g(256,128, IMAGE_16);
    for (int y=0; y<img16g.height(); ++y){
      for (int x=0; x<img16g.width(); ++x){
        uint32_t c = color_rem_transp(img32.get32(x,y), false);
        img16g.set16(x,y, color_rgb_to_grey16(c));
      }
    }

    // IMAGE_8
    Image img8g(256,128, IMAGE_8);
    for (int y=0; y<img8g.height(); ++y){
      for (int x=0; x<img8g.width(); ++x){
        uint32_t c = color_rem_transp(img32.get32(x,y), false);
        img8g.set8(x,y, color_rgb_to_grey8(c));
      }
    }

    // IMAGE_8PAL
    std::vector<uint32_t> colors = image_colormap(img32);
    Image img8p = image_remap(img32, colors);

    // IMAGE_1
    Image img1(256,128, IMAGE_1);
    for (int y=0; y<img1.height(); ++y){
      for (int x=0; x<img1.width(); ++x){
        img1.set1(x,y, (int)(600*sin(2*M_PI*x/255)*sin(2*M_PI*y/255))%2);
      }
    }


    { // IMAGE_32ARGB
      image_save_png(img32, "tmp_32.png");
      Image img = image_load_png("tmp_32.png", 1);
      assert(img.type() == IMAGE_32ARGB);
      assert(img.width() == 256);
      assert(img.height() == 128);
      assert(img.get32(0,0) == 0xFF000000);
      assert(img.get32(127,127) == 0xFFFEFE00);
      assert(img.get32(128,0) == 0x00000000);
      assert(img.get32(255,127) == 0xFEFC0000);
    }

    { // IMAGE_24RGB
      image_save_png(img24, "tmp_24.png");
      Image img = image_load_png("tmp_24.png", 1);
      assert(img.type() == IMAGE_32ARGB);
      assert(img.width() == 256);
      assert(img.height() == 128);
      assert(img.get32(0,0) == 0xFF000000);
      assert(img.get32(127,127) == 0xFFFEFE00);
      assert(img.get32(128,0)   == 0xFFFFFFFF);
      assert(img.get32(255,127) == 0xFFFd0000);
    }

    { // IMAGE_16
      image_save_png(img16g, "tmp_16g.png");
      Image img = image_load_png("tmp_16g.png", 1);
      assert(img.type() == IMAGE_32ARGB);
      assert(img.width() == 256);
      assert(img.height() == 128);
      assert(img.get32(0,0) == 0xFF000000);
      assert(img.get32(127,127) == 0xFFe0e0e0);
      assert(img.get32(128,0) == 0xFFFFFFFF);
      assert(img.get32(255,127) == 0xFF4b4b4b);
    }

    { // IMAGE_8
      image_save_png(img8g, "tmp_8g.png");
      Image img = image_load_png("tmp_8g.png", 1);
      assert(img.type() == IMAGE_32ARGB);
      assert(img.width() == 256);
      assert(img.height() == 128);
      assert(img.get32(0,0) == 0xFF000000);
      assert(img.get32(127,127) == 0xFFe1e1e1);
      assert(img.get32(128,0) == 0xFFFFFFFF);
      assert(img.get32(255,127) == 0xFF4c4c4c);
    }

    { // IMAGE_8PAL
      image_save_png(img8p, "tmp_8p.png");
      Image img = image_load_png("tmp_8p.png", 1);
      assert(img.type() == IMAGE_32ARGB);
      assert(img.width() == 256);
      assert(img.height() == 128);
      assert(img.get32(0,0)     == 0xff0a0000);
      assert(img.get32(127,127) == 0xfff6f600);
      assert(img.get32(128,0)   == 0xffffffff);
      assert(img.get32(255,127) == 0xfff40000);
    }

    { // IMAGE_1
      image_save_png(img1, "tmp_1.png");
      Image img = image_load_png("tmp_1.png", 1);
      assert(img.type() == IMAGE_32ARGB);
      assert(img.width() == 256);
      assert(img.height() == 128);
      assert(img1.get1(0,0)     == 0);
      assert(img1.get1(0,0)     == 0);
      assert(img1.get1(15,45)   == 0);
      assert(img1.get1(43,123)  == 1);
      assert(img1.get1(203,27)  == 0);
      assert(img.get32(0,0)     == 0xFF000000);
      assert(img.get32(15,45)   == 0xFF000000);
      assert(img.get32(43,123)  == 0xFFFFFFFF);
      assert(img.get32(203,27)  == 0xFF000000);
    }

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond