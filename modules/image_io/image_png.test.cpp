///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "image_png.h"
#include "image/image_colors.h"
#include "err/assert_err.h"

int
main(){
  try{

    // size
    assert_err(image_size_png("test_png/missing"),
      "PNG error: can't open file: test_png/missing");
    assert_err(image_size_png("test_png/Readme.md"),
      "PNG error: not a PNG file: test_png/Readme.md");

    // load
    assert_err(image_load_png("test_png/missing"),
      "PNG error: can't open file: test_png/missing");
    assert_err(image_load_png("test_png/Readme.md"),
      "PNG error: not a PNG file: test_png/Readme.md");

    /*********************************************/
    // Original image
    Image img32(256,128, IMAGE_32ARGB);
    for (int y=0; y<128; ++y){
      for (int x=0; x<128; ++x){
        img32.set32(x,y,     color_argb(0xFF, 2*x, 2*y, 0));
        img32.set32(128+x,y, color_argb(2*x,  2*y, 0,   0));
      }
    }

    // * Create all types of images (32ARGB, 24RGB, 16, 8, 1, PAL).
    // * Save them with different image_save_png() options.
    // * Read saved file and check result.

    { // IMAGE_32ARGB
      Image img = img32;
      image_save_png(img, "test_png/img_32_def.png");
      assert(image_size_png("test_png/img_32_def.png") == iPoint(256,128));

      Image I = image_load_png("test_png/img_32_def.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(127,127) == 0xFFFEFE00);
      assert(I.get32(128,0)   == 0x00000000);
      assert(I.get32(255,127) == 0xFEFC0000);
      assert(I.get32(64,64)   == 0xFF808000);
      assert(I.get32(192,64)  == 0x80400000);

      Opt o;
      o.put("png_format", "rgb");
      image_save_png(img, "test_png/img_32_rgb.png", o);
      I = image_load_png("test_png/img_32_rgb.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(127,127) == 0xFFFEFE00);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFFFd0000);
      assert(I.get32(64,64)   == 0xFF808000);
      assert(I.get32(192,64)  == 0xFF800000);

      o.put("png_format", "grey");
      image_save_png(img, "test_png/img_32_grey.png", o);
      I = image_load_png("test_png/img_32_grey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(127,127) == 0xFFe1e1e1);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4c4c4c);
      assert(I.get32(64,64)   == 0xFF717171);
      assert(I.get32(192,64)  == 0xFF262626);

      o.put("png_format", "agrey");
      image_save_png(img, "test_png/img_32_agrey.png", o);
      I = image_load_png("test_png/img_32_agrey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(127,127) == 0xFFe1e1e1);
      assert(I.get32(128,0)   == 0x00000000);
      assert(I.get32(255,127) == 0xFE4b4b4b);
      assert(I.get32(64,64)   == 0xFF717171);
      assert(I.get32(192,64)  == 0x80131313);

      o.put("png_format", "pal");
      o.put("cmap_colors", 120);
      o.put("cmap_alpha", "none");
      image_save_png(img, "test_png/img_32_pal.png", o);
      I = image_load_png("test_png/img_32_pal.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xff0a0000);
      assert(I.get32(127,127) == 0xfff6f600);
      assert(I.get32(128,0)   == 0xffffffff);
      assert(I.get32(255,127) == 0xfff40000);
      assert(I.get32(64,64)   == 0xFF8C7600);
      assert(I.get32(192,64)  == 0xFF8A0000);

      o.put("png_format", "pal");
      o.put("cmap_colors", 120);
      o.put("cmap_alpha", "full");
      image_save_png(img, "test_png/img_32_apal.png", o);
      I = image_load_png("test_png/img_32_apal.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xF1080000);
      assert(I.get32(127,127) == 0xFFEEEE00);
      assert(I.get32(128,0)   == 0x0E000000);
      assert(I.get32(255,127) == 0xFEEC0B00);
      assert(I.get32(64,64)   == 0xFF8C8C00);
      assert(I.get32(192,64)  == 0x871C0000);

      o.put("png_format", "pal");
      o.put("cmap_colors", 120);
      o.put("cmap_alpha", "gif");
      image_save_png(img, "test_png/img_32_gpal.png", o);
      I = image_load_png("test_png/img_32_gpal.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xff0A0000);
      assert(I.get32(127,127) == 0xfff6f600);
      assert(I.get32(128,0)   == 0x00000000);
      assert(I.get32(255,127) == 0xfff40000);
      assert(I.get32(64,64)   == 0xFF8C7600);
      assert(I.get32(192,64)  == 0xFF8A0000);

      o.put("png_format", "pal");
      o.put("cmap_colors", 300); // too many colors
      assert_err( image_save_png(img, "test_png/img_32_xpal.png", o),
        "image_remap: palette length is out of range");

    }

    /*********************************************/
    { // IMAGE_24RGB
      Image img(256,128, IMAGE_24RGB);
      for (int y=0; y<img.height(); ++y){
        for (int x=0; x<img.width(); ++x){
          img.set24(x,y, color_rem_transp(img32.get32(x,y), false));
        }
      }
      image_save_png(img, "test_png/img_24_def.png");
      Image I = image_load_png("test_png/img_24_def.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0) == 0xFF000000);
      assert(I.get32(127,127) == 0xFFFEFE00);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFFFd0000);
      assert(I.get32(64,64)   == 0xFF808000);
      assert(I.get32(192,64)  == 0xFF800000);

      Opt o;
      o.put("png_format", "argb");
      image_save_png(img, "test_png/img_24_argb.png", o);
      I = image_load_png("test_png/img_24_argb.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0) == 0xFF000000);
      assert(I.get32(127,127) == 0xFFFEFE00);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFFFd0000);
      assert(I.get32(64,64)   == 0xFF808000);
      assert(I.get32(192,64)  == 0xFF800000);

      o.put("png_format", "grey");
      image_save_png(img, "test_png/img_24_grey.png", o);
      I = image_load_png("test_png/img_24_grey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(127,127) == 0xFFe1e1e1);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4c4c4c);
      assert(I.get32(64,64)   == 0xFF717171);
      assert(I.get32(192,64)  == 0xFF262626);

      o.put("png_format", "agrey"); // same as grey
      image_save_png(img, "test_png/img_24_agrey.png", o);
      I = image_load_png("test_png/img_24_agrey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0) == 0xFF000000);
      assert(I.get32(127,127) == 0xFFe1e1e1);
      assert(I.get32(128,0) == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4c4c4c);
      assert(I.get32(64,64)   == 0xFF717171);
      assert(I.get32(192,64)  == 0xFF262626);

      o.put("png_format", "pal");
      o.put("cmap_colors", 120);
      image_save_png(img, "test_png/img_24_pal.png", o);
      I = image_load_png("test_png/img_24_pal.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xff0a0000);
      assert(I.get32(127,127) == 0xfff6f600);
      assert(I.get32(128,0)   == 0xffffffff);
      assert(I.get32(255,127) == 0xfff40000);
      assert(I.get32(64,64)   == 0xFF8C7600);
      assert(I.get32(192,64)  == 0xFF8A0000);
    }

    /*********************************************/
    { // IMAGE_16
      Image img(256,128, IMAGE_16);
      for (int y=0; y<img.height(); ++y){
        for (int x=0; x<img.width(); ++x){
          uint32_t c = color_rem_transp(img32.get32(x,y), false);
          img.set16(x,y, color_rgb_to_grey16(c));
        }
      }

      image_save_png(img, "test_png/img_16_def.png");
      Image I = image_load_png("test_png/img_16_def.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(127,127) == 0xFFe0e0e0);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4b4b4b);
      assert(I.get32(64,64)   == 0xFF717171);
      assert(I.get32(192,64)  == 0xFF262626);

      Opt o;
      o.put("png_format", "argb");
      image_save_png(img, "test_png/img_16_argb.png", o);
      I = image_load_png("test_png/img_16_argb.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(127,127) == 0xFFe0e0e0);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4b4b4b);
      assert(I.get32(64,64)   == 0xFF717171);
      assert(I.get32(192,64)  == 0xFF262626);

      o.put("png_format", "rgb");
      image_save_png(img, "test_png/img_16_rgb.png", o);
      I = image_load_png("test_png/img_16_rgb.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(127,127) == 0xFFe0e0e0);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4b4b4b);
      assert(I.get32(64,64)   == 0xFF717171);
      assert(I.get32(192,64)  == 0xFF262626);

      o.put("png_format", "grey");
      image_save_png(img, "test_png/img_16_grey.png", o);
      I = image_load_png("test_png/img_16_grey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(127,127) == 0xFFe0e0e0);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4b4b4b);
      assert(I.get32(64,64)   == 0xFF717171);
      assert(I.get32(192,64)  == 0xFF262626);

      o.put("png_format", "agrey");
      image_save_png(img, "test_png/img_16_agrey.png", o);
      I = image_load_png("test_png/img_16_agrey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(127,127) == 0xFFe0e0e0);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4b4b4b);
      assert(I.get32(64,64)   == 0xFF717171);
      assert(I.get32(192,64)  == 0xFF262626);

      o.put("png_format", "pal");
      o.put("cmap_colors", 32);
      image_save_png(img, "test_png/img_16_pal.png", o);
      I = image_load_png("test_png/img_16_pal.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF020202);
      assert(I.get32(127,127) == 0xFFDCDCDC);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4D4D4D);
      assert(I.get32(64,64)   == 0xFF737373);
      assert(I.get32(192,64)  == 0xFF232323);

    }

    /*********************************************/

    { // IMAGE_8
      Image img(256,128, IMAGE_8);
      for (int y=0; y<img.height(); ++y){
        for (int x=0; x<img.width(); ++x){
          uint32_t c = color_rem_transp(img32.get32(x,y), false);
          img.set8(x,y, color_rgb_to_grey8(c));
        }
      }

      image_save_png(img, "test_png/img_8_def.png");
      Image I = image_load_png("test_png/img_8_def.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(127,127) == 0xFFE1E1E1);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4C4C4C);
      assert(I.get32(64,64)   == 0xFF717171);
      assert(I.get32(192,64)  == 0xFF262626);

      Opt o;
      o.put("png_format", "argb");
      image_save_png(img, "test_png/img_8_argb.png", o);
      I = image_load_png("test_png/img_8_argb.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(127,127) == 0xFFE1E1E1);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4C4C4C);
      assert(I.get32(64,64)   == 0xFF717171);
      assert(I.get32(192,64)  == 0xFF262626);

      o.put("png_format", "rgb");
      image_save_png(img, "test_png/img_8_rgb.png", o);
      I = image_load_png("test_png/img_8_rgb.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(127,127) == 0xFFE1E1E1);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4C4C4C);
      assert(I.get32(64,64)   == 0xFF717171);
      assert(I.get32(192,64)  == 0xFF262626);

      o.put("png_format", "grey");
      image_save_png(img, "test_png/img_8_grey.png", o);
      I = image_load_png("test_png/img_8_grey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(127,127) == 0xFFe1e1e1);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4c4c4c);
      assert(I.get32(64,64)   == 0xFF717171);
      assert(I.get32(192,64)  == 0xFF262626);

      o.put("png_format", "agrey");
      image_save_png(img, "test_png/img_8_agrey.png", o);
      I = image_load_png("test_png/img_8_agrey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(127,127) == 0xFFe1e1e1);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4c4c4c);
      assert(I.get32(64,64)   == 0xFF717171);
      assert(I.get32(192,64)  == 0xFF262626);

      o.put("png_format", "pal");
      o.put("cmap_colors", 32);
      image_save_png(img, "test_png/img_8_pal.png", o);
      I = image_load_png("test_png/img_8_pal.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF020202);
      assert(I.get32(127,127) == 0xFFDDDDDD);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4D4D4D);
      assert(I.get32(64,64)   == 0xFF737373);
      assert(I.get32(192,64)  == 0xFF242424);
    }


    { // IMAGE_8PAL
      std::vector<uint32_t> colors = image_colormap(img32);
      Image img = image_remap(img32, colors);
      image_save_png(img, "test_png/img_8p_def.png");
      Image I = image_load_png("test_png/img_8p_def.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xff0a0000);
      assert(I.get32(127,127) == 0xfff6f600);
      assert(I.get32(128,0)   == 0xffffffff);
      assert(I.get32(255,127) == 0xfff40000);
      assert(I.get32(64,64)   == 0xff848600);
      assert(I.get32(192,64)  == 0xff830000);


      Opt o;
      o.put("png_format", "argb");
      image_save_png(img, "test_png/img_8p_argb.png", o);
      I = image_load_png("test_png/img_8p_argb.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xff0a0000);
      assert(I.get32(127,127) == 0xfff6f600);
      assert(I.get32(128,0)   == 0xffffffff);
      assert(I.get32(255,127) == 0xfff40000);
      assert(I.get32(64,64)   == 0xff848600);
      assert(I.get32(192,64)  == 0xff830000);

      o.put("png_format", "rgb");
      image_save_png(img, "test_png/img_8p_rgb.png", o);
      I = image_load_png("test_png/img_8p_rgb.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xff0a0000);
      assert(I.get32(127,127) == 0xfff6f600);
      assert(I.get32(128,0)   == 0xffffffff);
      assert(I.get32(255,127) == 0xfff40000);
      assert(I.get32(64,64)   == 0xff848600);
      assert(I.get32(192,64)  == 0xff830000);

      o.put("png_format", "grey");
      image_save_png(img, "test_png/img_8p_grey.png", o);
      I = image_load_png("test_png/img_8p_grey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF030303);
      assert(I.get32(127,127) == 0xFFDADADA);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF494949);
      assert(I.get32(64,64)   == 0xFF767676);
      assert(I.get32(192,64)  == 0xFF272727);

      o.put("png_format", "agrey");
      image_save_png(img, "test_png/img_8p_agrey.png", o);
      I = image_load_png("test_png/img_8p_agrey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF030303);
      assert(I.get32(127,127) == 0xFFDADADA);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF494949);
      assert(I.get32(64,64)   == 0xFF767676);
      assert(I.get32(192,64)  == 0xFF272727);

      o.put("png_format", "pal");
      o.put("cmap_colors", 100);
      image_save_png(img, "test_png/img_8p_pal.png", o);
      I = image_load_png("test_png/img_8p_pal.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF0A0000);
      assert(I.get32(127,127) == 0xFFEEEE00);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFFEB0000);
      assert(I.get32(64,64)   == 0xFF7C8600);
      assert(I.get32(192,64)  == 0xFF8A0000);

    }


    { // IMAGE_1
      Image img(256,128, IMAGE_1);
      for (int y=0; y<img.height(); ++y){
        for (int x=0; x<img.width(); ++x){
          img.set1(x,y, (int)(600*sin(2*M_PI*x/255)*sin(2*M_PI*y/255))%2);
        }
      }
      image_save_png(img, "test_png/img_1_def.png");
      Image I = image_load_png("test_png/img_1_def.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(img.get1(0,0)     == 0);
      assert(img.get1(15,45)   == 0);
      assert(img.get1(43,123)  == 1);
      assert(img.get1(203,27)  == 0);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(15,45)   == 0xFF000000);
      assert(I.get32(43,123)  == 0xFFFFFFFF);
      assert(I.get32(203,27)  == 0xFF000000);

      Opt o;
      o.put("png_format", "argb");
      image_save_png(img, "test_png/img_1_argb.png", o);
      I = image_load_png("test_png/img_1_argb.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(15,45)   == 0xFF000000);
      assert(I.get32(43,123)  == 0xFFFFFFFF);
      assert(I.get32(203,27)  == 0xFF000000);

      o.put("png_format", "rgb");
      image_save_png(img, "test_png/img_1_rgb.png", o);
      I = image_load_png("test_png/img_1_rgb.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(15,45)   == 0xFF000000);
      assert(I.get32(43,123)  == 0xFFFFFFFF);
      assert(I.get32(203,27)  == 0xFF000000);

      o.put("png_format", "grey");
      image_save_png(img, "test_png/img_1_grey.png", o);
      I = image_load_png("test_png/img_1_grey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(15,45)   == 0xFF000000);
      assert(I.get32(43,123)  == 0xFFFFFFFF);
      assert(I.get32(203,27)  == 0xFF000000);

      o.put("png_format", "agrey");
      image_save_png(img, "test_png/img_1_agrey.png", o);
      I = image_load_png("test_png/img_1_agrey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(15,45)   == 0xFF000000);
      assert(I.get32(43,123)  == 0xFFFFFFFF);
      assert(I.get32(203,27)  == 0xFF000000);

      o.put("png_format", "pal");
      o.put("cmap_colors", 32);
      image_save_png(img, "test_png/img_1_pal.png", o);
      I = image_load_png("test_png/img_1_pal.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(15,45)   == 0xFF000000);
      assert(I.get32(43,123)  == 0xFFFFFFFF);
      assert(I.get32(203,27)  == 0xFF000000);
    }

/*
std::cerr << std::hex << I.get32(0,0) << "\n";
std::cerr << std::hex << I.get32(127,127) << "\n";
std::cerr << std::hex << I.get32(128,0) << "\n";
std::cerr << std::hex << I.get32(255,127) << "\n";
std::cerr << std::hex << I.get32(64,64) << "\n";
std::cerr << std::hex << I.get32(192,64) << "\n";
*/

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond