///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "image_png.h"
#include "image/image_colors.h"
#include "err/assert_err.h"

int
main(){
  try{

    /*********************************************/
    // Original image
    Image img32(256,128, IMAGE_32ARGB);

    { // IMAGE_32ARGB
      Image img = img32;
      img.fill32(0xFFFF0000);
      for (int y=0; y<128; ++y){
        for (int x=0; x<128; ++x){
          img.set32(x,y,     color_argb(0xFF, 2*x, 2*y, 0));
          img.set32(128+x,y, color_argb(2*x,  2*y, 0,   0));
        }
      }
      image_save_png(img, "tmp_32_def.png");
      Image I = image_load_png("tmp_32_def.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0) == 0xFF000000);
      assert(I.get32(127,127) == 0xFFFEFE00);
      assert(I.get32(128,0) == 0x00000000);
      assert(I.get32(255,127) == 0xFEFC0000);

      Opt o;
      o.put("png_format", "rgb");
      image_save_png(img, "tmp_32_rgb.png", o);
      I = image_load_png("tmp_32_rgb.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0) == 0xFF000000);
      assert(I.get32(127,127) == 0xFFFEFE00);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFFFd0000);

      o.put("png_format", "grey");
      image_save_png(img, "tmp_32_g.png", o);
      I = image_load_png("tmp_32_g.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0) == 0xFF000000);
      assert(I.get32(127,127) == 0xFFe1e1e1);
      assert(I.get32(128,0) == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4c4c4c);

      o.put("png_format", "agrey");
      image_save_png(img, "tmp_32_ag.png", o);
      I = image_load_png("tmp_32_ag.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0) == 0xFF000000);
      assert(I.get32(127,127) == 0xFFe1e1e1);
      assert(I.get32(128,0) == 0x00000000);
      assert(I.get32(255,127) == 0xFE4b4b4b);

      o.put("png_format", "pal");
      o.put("cmap_colors", 120);
      o.put("cmap_alpha", "none");
      image_save_png(img, "tmp_32_pal.png", o);
      I = image_load_png("tmp_32_pal.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xff0a0000);
      assert(I.get32(127,127) == 0xfff6f600);
      assert(I.get32(128,0)   == 0xffffffff);
      assert(I.get32(255,127) == 0xfff40000);

      o.put("png_format", "pal");
      o.put("cmap_colors", 120);
      o.put("cmap_alpha", "full");
      image_save_png(img, "tmp_32_apal.png", o);
      I = image_load_png("tmp_32_apal.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xF1080000);
      assert(I.get32(127,127) == 0xFFEEEE00);
      assert(I.get32(128,0)   == 0x0E000000);
      assert(I.get32(255,127) == 0xFEEC0B00);

      o.put("png_format", "pal");
      o.put("cmap_colors", 120);
      o.put("cmap_alpha", "gif");
      image_save_png(img, "tmp_32_gpal.png", o);
      I = image_load_png("tmp_32_gpal.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xff0A0000);
      assert(I.get32(127,127) == 0xfff6f600);
      assert(I.get32(128,0)   == 0x00000000);
      assert(I.get32(255,127) == 0xfff40000);
    }

    /*********************************************/
    { // IMAGE_24RGB
      Image img(256,128, IMAGE_24RGB);
      for (int y=0; y<img.height(); ++y){
        for (int x=0; x<img.width(); ++x){
          img.set24(x,y, color_rem_transp(img32.get32(x,y), false));
        }
      }
      image_save_png(img, "tmp_24.png");
      Image I = image_load_png("tmp_24.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0) == 0xFF000000);
      assert(I.get32(127,127) == 0xFFFEFE00);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFFFd0000);

      Opt o;
      o.put("png_format", "argb");
      image_save_png(img, "tmp_24_argb.png", o);
      I = image_load_png("tmp_24_argb.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0) == 0xFF000000);
      assert(I.get32(127,127) == 0xFFFEFE00);
      assert(I.get32(128,0)   == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFFFd0000);

      o.put("png_format", "grey");
      image_save_png(img, "tmp_24_g.png", o);
      I = image_load_png("tmp_24_g.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0) == 0xFF000000);
      assert(I.get32(127,127) == 0xFFe1e1e1);
      assert(I.get32(128,0) == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4c4c4c);

      o.put("png_format", "agrey"); // same as grey
      image_save_png(img, "tmp_24_ag.png", o);
      I = image_load_png("tmp_24_ag.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0) == 0xFF000000);
      assert(I.get32(127,127) == 0xFFe1e1e1);
      assert(I.get32(128,0) == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4c4c4c);

      o.put("png_format", "pal");
      o.put("cmap_colors", 120);
      image_save_png(img, "tmp_24_pal.png", o);
      I = image_load_png("tmp_24_pal.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xff0a0000);
      assert(I.get32(127,127) == 0xfff6f600);
      assert(I.get32(128,0)   == 0xffffffff);
      assert(I.get32(255,127) == 0xfff40000);
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

      image_save_png(img, "tmp_16g.png");
      Image I = image_load_png("tmp_16g.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0) == 0xFF000000);
      assert(I.get32(127,127) == 0xFFe0e0e0);
      assert(I.get32(128,0) == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4b4b4b);
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

      image_save_png(img, "tmp_8g.png");
      Image I = image_load_png("tmp_8g.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0) == 0xFF000000);
      assert(I.get32(127,127) == 0xFFe1e1e1);
      assert(I.get32(128,0) == 0xFFFFFFFF);
      assert(I.get32(255,127) == 0xFF4c4c4c);
    }

    { // IMAGE_8PAL
      std::vector<uint32_t> colors = image_colormap(img32);
      Image img = image_remap(img32, colors);

      image_save_png(img, "tmp_8p.png");
      Image I = image_load_png("tmp_8p.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(I.get32(0,0)     == 0xff0a0000);
      assert(I.get32(127,127) == 0xfff6f600);
      assert(I.get32(128,0)   == 0xffffffff);
      assert(I.get32(255,127) == 0xfff40000);
    }

    { // IMAGE_1
    // IMAGE_1
    Image img1(256,128, IMAGE_1);
    for (int y=0; y<img1.height(); ++y){
      for (int x=0; x<img1.width(); ++x){
        img1.set1(x,y, (int)(600*sin(2*M_PI*x/255)*sin(2*M_PI*y/255))%2);
      }
    }
      image_save_png(img1, "tmp_1.png");
      Image I = image_load_png("tmp_1.png", 1);
      assert(I.type() == IMAGE_32ARGB);
      assert(I.width() == 256);
      assert(I.height() == 128);
      assert(img1.get1(0,0)     == 0);
      assert(img1.get1(0,0)     == 0);
      assert(img1.get1(15,45)   == 0);
      assert(img1.get1(43,123)  == 1);
      assert(img1.get1(203,27)  == 0);
      assert(I.get32(0,0)     == 0xFF000000);
      assert(I.get32(15,45)   == 0xFF000000);
      assert(I.get32(43,123)  == 0xFFFFFFFF);
      assert(I.get32(203,27)  == 0xFF000000);
    }

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond