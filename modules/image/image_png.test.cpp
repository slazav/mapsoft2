///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "image_png.h"
#include "err/assert_err.h"
#include "image_colors.h"

int
main(){
  try{

    // size
    assert_err(image_size_png("test_png/missing"),
      "image_size_png: can't open file: test_png/missing");
    assert_err(image_size_png("test_png/Readme.md"),
      "image_size_png: not a PNG file: test_png/Readme.md");

    // load
    assert_err(image_load_png("test_png/missing"),
      "image_load_png: can't open file: test_png/missing");
    assert_err(image_load_png("test_png/Readme.md"),
      "image_load_png: not a PNG file: test_png/Readme.md");

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
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFFEFE00);
      assert(I.get_argb(128,0)   == 0x00000000);
      assert(I.get_argb(255,127) == 0xFeFd0000);
      assert(I.get_argb(64,64)   == 0xFF808000);
      assert(I.get_argb(192,64)  == 0x80400000);

      Opt o;
      o.put("png_format", "rgb");
      image_save_png(img, "test_png/img_32_rgb.png", o);
      I = image_load_png("test_png/img_32_rgb.png", 1);
      assert(I.type() == IMAGE_24RGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFFEFE00);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFFFe0000);
      assert(I.get_argb(64,64)   == 0xFF808000);
      assert(I.get_argb(192,64)  == 0xFF800000);

      o.put("png_format", "grey");
      image_save_png(img, "test_png/img_32_grey.png", o);
      I = image_load_png("test_png/img_32_grey.png", 1);
      assert(I.type() == IMAGE_8);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFe1e1e1);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFF4c4c4c);
      assert(I.get_argb(64,64)   == 0xFF717171);
      assert(I.get_argb(192,64)  == 0xFF262626);

      o.put("png_format", "agrey");
      image_save_png(img, "test_png/img_32_agrey.png", o);
      I = image_load_png("test_png/img_32_agrey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFe1e1e1);
      assert(I.get_argb(128,0)   == 0x00000000);
      assert(I.get_argb(255,127) == 0xFE4b4b4b);
      assert(I.get_argb(64,64)   == 0xFF717171);
      assert(I.get_argb(192,64)  == 0x80131313);

      o.put("png_format", "pal");
      o.put("cmap_colors", 120);
      o.put("cmap_alpha", "none");
      image_save_png(img, "test_png/img_32_pal.png", o);
      I = image_load_png("test_png/img_32_pal.png", 1);
      assert(I.type() == IMAGE_8PAL);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xff0a0000);
      assert(I.get_argb(127,127) == 0xfff6f600);
      assert(I.get_argb(128,0)   == 0xffffffff);
      assert(I.get_argb(255,127) == 0xfff50000);
      assert(I.get_argb(64,64)   == 0xFF8C7600);
      assert(I.get_argb(192,64)  == 0xFF8B0000);

      o.put("png_format", "pal");
      o.put("cmap_colors", 120);
      o.put("cmap_alpha", "full");
      image_save_png(img, "test_png/img_32_apal.png", o);
      I = image_load_png("test_png/img_32_apal.png", 1);
      assert(I.type() == IMAGE_8PAL);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xF0080000);
      assert(I.get_argb(127,127) == 0xFFEEEE00);
      assert(I.get_argb(128,0)   == 0x0E050000);
      assert(I.get_argb(255,127) == 0xFEEE0B00);
      assert(I.get_argb(64,64)   == 0xFF8C8C00);
      assert(I.get_argb(192,64)  == 0x87360000);

      o.put("png_format", "pal");
      o.put("cmap_colors", 120);
      o.put("cmap_alpha", "gif");
      image_save_png(img, "test_png/img_32_gpal.png", o);
      I = image_load_png("test_png/img_32_gpal.png", 1);
      assert(I.type() == IMAGE_8PAL);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xff0A0000);
      assert(I.get_argb(127,127) == 0xfff6f600);
      assert(I.get_argb(128,0)   == 0x00000000);
      assert(I.get_argb(255,127) == 0xfff50000);
      assert(I.get_argb(64,64)   == 0xFF8C7600);
      assert(I.get_argb(192,64)  == 0xFF8B0000);

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
          img.set24(x,y, color_rem_transp(img32.get_argb(x,y), false));
        }
      }
      image_save_png(img, "test_png/img_24_def.png");
      Image I = image_load_png("test_png/img_24_def.png", 1);
      assert(I.type() == IMAGE_24RGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0) == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFFEFE00);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFFFE0000);
      assert(I.get_argb(64,64)   == 0xFF808000);
      assert(I.get_argb(192,64)  == 0xFF800000);

      Opt o;
      o.put("png_format", "argb");
      image_save_png(img, "test_png/img_24_argb.png", o);
      I = image_load_png("test_png/img_24_argb.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0) == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFFEFE00);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFFFE0000);
      assert(I.get_argb(64,64)   == 0xFF808000);
      assert(I.get_argb(192,64)  == 0xFF800000);

      o.put("png_format", "grey");
      image_save_png(img, "test_png/img_24_grey.png", o);
      I = image_load_png("test_png/img_24_grey.png", 1);
      assert(I.type() == IMAGE_8);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFe1e1e1);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFF4c4c4c);
      assert(I.get_argb(64,64)   == 0xFF717171);
      assert(I.get_argb(192,64)  == 0xFF262626);

      o.put("png_format", "agrey"); // same as grey
      image_save_png(img, "test_png/img_24_agrey.png", o);
      I = image_load_png("test_png/img_24_agrey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0) == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFe1e1e1);
      assert(I.get_argb(128,0) == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFF4c4c4c);
      assert(I.get_argb(64,64)   == 0xFF717171);
      assert(I.get_argb(192,64)  == 0xFF262626);

      o.put("png_format", "pal");
      o.put("cmap_colors", 120);
      image_save_png(img, "test_png/img_24_pal.png", o);
      I = image_load_png("test_png/img_24_pal.png", 1);
      assert(I.type() == IMAGE_8PAL);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xff0a0000);
      assert(I.get_argb(127,127) == 0xfff6f600);
      assert(I.get_argb(128,0)   == 0xffffffff);
      assert(I.get_argb(255,127) == 0xfff50000);
      assert(I.get_argb(64,64)   == 0xFF8C7600);
      assert(I.get_argb(192,64)  == 0xFF8B0000);
    }

    /*********************************************/
    { // IMAGE_16
      Image img(256,128, IMAGE_16);
      for (int y=0; y<img.height(); ++y){
        for (int x=0; x<img.width(); ++x){
          uint32_t c = color_rem_transp(img32.get_argb(x,y), false);
          img.set16(x,y, color_rgb_to_grey16(c));
        }
      }

      image_save_png(img, "test_png/img_16_def.png");
      Image I = image_load_png("test_png/img_16_def.png", 1);
      assert(I.type() == IMAGE_16);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFe0e0e0);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFF4b4b4b);
      assert(I.get_argb(64,64)   == 0xFF717171);
      assert(I.get_argb(192,64)  == 0xFF262626);
      assert(I.get16(0,0)     == 0x0000);
      assert(I.get16(127,127) == 0xE0EB);
      assert(I.get16(128,0)   == 0xFF00);
      assert(I.get16(255,127) == 0x4BEC);
      assert(I.get16(64,64)   == 0x7158);
      assert(I.get16(192,64)  == 0x2642);

      Opt o;
      o.put("png_format", "argb");
      image_save_png(img, "test_png/img_16_argb.png", o);
      I = image_load_png("test_png/img_16_argb.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFe0e0e0);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFF4b4b4b);
      assert(I.get_argb(64,64)   == 0xFF717171);
      assert(I.get_argb(192,64)  == 0xFF262626);

      o.put("png_format", "rgb");
      image_save_png(img, "test_png/img_16_rgb.png", o);
      I = image_load_png("test_png/img_16_rgb.png", 1);
      assert(I.type() == IMAGE_24RGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFe0e0e0);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFF4b4b4b);
      assert(I.get_argb(64,64)   == 0xFF717171);
      assert(I.get_argb(192,64)  == 0xFF262626);

      o.put("png_format", "grey");
      image_save_png(img, "test_png/img_16_grey.png", o);
      I = image_load_png("test_png/img_16_grey.png", 1);
      assert(I.type() == IMAGE_8);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFe0e0e0);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFF4b4b4b);
      assert(I.get_argb(64,64)   == 0xFF717171);
      assert(I.get_argb(192,64)  == 0xFF262626);

      o.put("png_format", "agrey");
      image_save_png(img, "test_png/img_16_agrey.png", o);
      I = image_load_png("test_png/img_16_agrey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFe0e0e0);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFF4b4b4b);
      assert(I.get_argb(64,64)   == 0xFF717171);
      assert(I.get_argb(192,64)  == 0xFF262626);

      o.put("png_format", "pal");
      o.put("cmap_colors", 32);
      image_save_png(img, "test_png/img_16_pal.png", o);
      I = image_load_png("test_png/img_16_pal.png", 1);
      assert(I.type() == IMAGE_8PAL);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF020202);
      assert(I.get_argb(127,127) == 0xFFDCDCDC);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFF4D4D4D);
      assert(I.get_argb(64,64)   == 0xFF737373);
      assert(I.get_argb(192,64)  == 0xFF232323);

    }

    /*********************************************/

    { // IMAGE_8
      Image img(256,128, IMAGE_8);
      for (int y=0; y<img.height(); ++y){
        for (int x=0; x<img.width(); ++x){
          uint32_t c = color_rem_transp(img32.get_argb(x,y), false);
          img.set8(x,y, color_rgb_to_grey8(c));
        }
      }

      image_save_png(img, "test_png/img_8_def.png");
      Image I = image_load_png("test_png/img_8_def.png", 1);
      assert(I.type() == IMAGE_8);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFE1E1E1);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFF4C4C4C);
      assert(I.get_argb(64,64)   == 0xFF717171);
      assert(I.get_argb(192,64)  == 0xFF262626);

      Opt o;
      o.put("png_format", "argb");
      image_save_png(img, "test_png/img_8_argb.png", o);
      I = image_load_png("test_png/img_8_argb.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFE1E1E1);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFF4C4C4C);
      assert(I.get_argb(64,64)   == 0xFF717171);
      assert(I.get_argb(192,64)  == 0xFF262626);

      o.put("png_format", "rgb");
      image_save_png(img, "test_png/img_8_rgb.png", o);
      I = image_load_png("test_png/img_8_rgb.png", 1);
      assert(I.type() == IMAGE_24RGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFE1E1E1);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFF4C4C4C);
      assert(I.get_argb(64,64)   == 0xFF717171);
      assert(I.get_argb(192,64)  == 0xFF262626);

      o.put("png_format", "grey");
      image_save_png(img, "test_png/img_8_grey.png", o);
      I = image_load_png("test_png/img_8_grey.png", 1);
      assert(I.type() == IMAGE_8);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFe1e1e1);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFF4c4c4c);
      assert(I.get_argb(64,64)   == 0xFF717171);
      assert(I.get_argb(192,64)  == 0xFF262626);

      o.put("png_format", "agrey");
      image_save_png(img, "test_png/img_8_agrey.png", o);
      I = image_load_png("test_png/img_8_agrey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(127,127) == 0xFFe1e1e1);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFF4c4c4c);
      assert(I.get_argb(64,64)   == 0xFF717171);
      assert(I.get_argb(192,64)  == 0xFF262626);

      o.put("png_format", "pal");
      o.put("cmap_colors", 32);
      image_save_png(img, "test_png/img_8_pal.png", o);
      I = image_load_png("test_png/img_8_pal.png", 1);
      assert(I.type() == IMAGE_8PAL);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF020202);
      assert(I.get_argb(127,127) == 0xFFDDDDDD);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFF4F4F4F);
      assert(I.get_argb(64,64)   == 0xFF6e6e6e);
      assert(I.get_argb(192,64)  == 0xFF242424);
    }


    { // IMAGE_8PAL
      std::vector<uint32_t> colors = image_colormap(img32);
      Image img = image_remap(img32, colors);
      image_save_png(img, "test_png/img_8p_def.png");
      Image I = image_load_png("test_png/img_8p_def.png", 1);
      assert(I.type() == IMAGE_8PAL);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xff0a0000);
      assert(I.get_argb(127,127) == 0xfff6f600);
      assert(I.get_argb(128,0)   == 0xffffffff);
      assert(I.get_argb(255,127) == 0xfff50000);
      assert(I.get_argb(64,64)   == 0xff848600);
      assert(I.get_argb(192,64)  == 0xff830000);


      Opt o;
      o.put("png_format", "argb");
      image_save_png(img, "test_png/img_8p_argb.png", o);
      I = image_load_png("test_png/img_8p_argb.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xff0a0000);
      assert(I.get_argb(127,127) == 0xfff6f600);
      assert(I.get_argb(128,0)   == 0xffffffff);
      assert(I.get_argb(255,127) == 0xfff50000);
      assert(I.get_argb(64,64)   == 0xff848600);
      assert(I.get_argb(192,64)  == 0xff830000);

      o.put("png_format", "rgb");
      image_save_png(img, "test_png/img_8p_rgb.png", o);
      I = image_load_png("test_png/img_8p_rgb.png", 1);
      assert(I.type() == IMAGE_24RGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xff0a0000);
      assert(I.get_argb(127,127) == 0xfff6f600);
      assert(I.get_argb(128,0)   == 0xffffffff);
      assert(I.get_argb(255,127) == 0xfff50000);
      assert(I.get_argb(64,64)   == 0xff848600);
      assert(I.get_argb(192,64)  == 0xff830000);

      o.put("png_format", "grey");
      image_save_png(img, "test_png/img_8p_grey.png", o);
      I = image_load_png("test_png/img_8p_grey.png", 1);
      assert(I.type() == IMAGE_8);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF030303);
      assert(I.get_argb(127,127) == 0xFFDADADA);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFF494949);
      assert(I.get_argb(64,64)   == 0xFF767676);
      assert(I.get_argb(192,64)  == 0xFF272727);

      o.put("png_format", "agrey");
      image_save_png(img, "test_png/img_8p_agrey.png", o);
      I = image_load_png("test_png/img_8p_agrey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF030303);
      assert(I.get_argb(127,127) == 0xFFDADADA);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFF494949);
      assert(I.get_argb(64,64)   == 0xFF767676);
      assert(I.get_argb(192,64)  == 0xFF272727);

      o.put("png_format", "pal");
      o.put("cmap_colors", 100);
      image_save_png(img, "test_png/img_8p_pal.png", o);
      I = image_load_png("test_png/img_8p_pal.png", 1);
      assert(I.type() == IMAGE_8PAL);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF0A0000);
      assert(I.get_argb(127,127) == 0xFFEEEE00);
      assert(I.get_argb(128,0)   == 0xFFFFFFFF);
      assert(I.get_argb(255,127) == 0xFFEC0000);
      assert(I.get_argb(64,64)   == 0xFF7C8600);
      assert(I.get_argb(192,64)  == 0xFF830000);

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
      assert(I.type() == IMAGE_8PAL);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(img.get1(0,0)     == 0);
      assert(img.get1(15,45)   == 0);
      assert(img.get1(43,123)  == 1);
      assert(img.get1(203,27)  == 0);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(15,45)   == 0xFF000000);
      assert(I.get_argb(43,123)  == 0xFFFFFFFF);
      assert(I.get_argb(203,27)  == 0xFF000000);

      Opt o;
      o.put("png_format", "argb");
      image_save_png(img, "test_png/img_1_argb.png", o);
      I = image_load_png("test_png/img_1_argb.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(15,45)   == 0xFF000000);
      assert(I.get_argb(43,123)  == 0xFFFFFFFF);
      assert(I.get_argb(203,27)  == 0xFF000000);

      o.put("png_format", "rgb");
      image_save_png(img, "test_png/img_1_rgb.png", o);
      I = image_load_png("test_png/img_1_rgb.png", 1);
      assert(I.type() == IMAGE_24RGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(15,45)   == 0xFF000000);
      assert(I.get_argb(43,123)  == 0xFFFFFFFF);
      assert(I.get_argb(203,27)  == 0xFF000000);

      o.put("png_format", "grey");
      image_save_png(img, "test_png/img_1_grey.png", o);
      I = image_load_png("test_png/img_1_grey.png", 1);
      assert(I.type() == IMAGE_8);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(15,45)   == 0xFF000000);
      assert(I.get_argb(43,123)  == 0xFFFFFFFF);
      assert(I.get_argb(203,27)  == 0xFF000000);

      o.put("png_format", "agrey");
      image_save_png(img, "test_png/img_1_agrey.png", o);
      I = image_load_png("test_png/img_1_agrey.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(15,45)   == 0xFF000000);
      assert(I.get_argb(43,123)  == 0xFFFFFFFF);
      assert(I.get_argb(203,27)  == 0xFF000000);

      o.put("png_format", "pal");
      o.put("cmap_colors", 32);
      image_save_png(img, "test_png/img_1_pal.png", o);
      I = image_load_png("test_png/img_1_pal.png", 1);
      assert(I.type() == IMAGE_8PAL);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get_argb(0,0)     == 0xFF000000);
      assert(I.get_argb(15,45)   == 0xFF000000);
      assert(I.get_argb(43,123)  == 0xFFFFFFFF);
      assert(I.get_argb(203,27)  == 0xFF000000);
    }

    { //scale tests
      Image I0 = image_load_png("test_png/img_32_def.png", 1);
      iPoint pt(101,32);
      for (double sc=1; sc<10; sc+=0.8){
        Image I1 = image_load_png("test_png/img_32_def.png", sc);
        assert(I1.width() == floor((I0.width()-1)/sc+1));
        assert(I1.height() == floor((I0.height()-1)/sc+1));
        iPoint pt1 = (dPoint)pt/sc;
        assert(I1.get_rgb(pt1.x, pt1.y) == I0.get_rgb(rint(pt1.x*sc), rint(pt1.y*sc)));

        pt = iPoint(I0.width()-1, I0.height()-1);
        pt1 = (dPoint)pt/sc;
        assert(pt1.x < I1.width());
        assert(pt1.y < I1.height());
        assert(I1.get_rgb(pt1.x, pt1.y) == I0.get_rgb(rint(pt1.x*sc), rint(pt1.y*sc)));
      }
      assert_err(image_load_png("test_png/img_32_def.png", 0),
        "image_load_png: wrong scale: 0");
    }

/*
std::cerr << std::hex << I.get_argb(0,0) << "\n";
std::cerr << std::hex << I.get_argb(127,127) << "\n";
std::cerr << std::hex << I.get_argb(128,0) << "\n";
std::cerr << std::hex << I.get_argb(255,127) << "\n";
std::cerr << std::hex << I.get_argb(64,64) << "\n";
std::cerr << std::hex << I.get_argb(192,64) << "\n";
*/

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond