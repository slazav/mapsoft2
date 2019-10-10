///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "err/assert_err.h"
#include "image_jpeg.h"
#include "image_colors.h"

int
main(){
  try{

    // size
    assert_err(image_size_jpeg("test_jpeg/missing"),
      "image_size_jpeg: can't open file: test_jpeg/missing");
    assert_err(image_size_jpeg("test_jpeg/Readme.md"),
      "image_size_jpeg: Not a JPEG file: starts with 0x45 0x6d");

    // load
    assert_err(image_load_jpeg("test_jpeg/missing"),
      "image_load_jpeg: can't open file: test_jpeg/missing");
    assert_err(image_load_jpeg("test_jpeg/Readme.md"),
      "image_load_jpeg: Not a JPEG file: starts with 0x45 0x6d");

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
    // * Save them with different image_save_jpeg() options.
    // * Read saved file and check result.

    { // IMAGE_32ARGB
      Image img = img32;
      image_save_jpeg(img, "test_jpeg/img_32_def.jpg");
      assert_eq(image_size_jpeg("test_jpeg/img_32_def.jpg"), iPoint(256,128));

      Image I = image_load_jpeg("test_jpeg/img_32_def.jpg", 1);
      image_save_jpeg(I, "test_jpeg/img_32_def1.jpg");
      assert_eq(I.type(), IMAGE_24RGB);
      assert_eq(I.width(), 256);
      assert_eq(I.height(), 128);

      // check far from edges (smaller jpeg artifacts)
      assert(color_dist(I.get_argb(10,10),   0xff141400) < 5);
      assert(color_dist(I.get_argb(117,117), 0xffeaea00) < 5);
      assert(color_dist(I.get_argb(138,10),  0xFF090000) < 5);
      assert(color_dist(I.get_argb(245,117), 0xffE90000) < 5);
      assert(color_dist(I.get_argb(64,64),   0xFF808000) < 5);
      assert(color_dist(I.get_argb(192,64),  0xFF800000) < 5);

      Opt o;
      o.put("jpeg_quality", 120);

      assert_err(image_save_jpeg(img, "test_jpeg/img_32_x.jpg", o),
         "image_save_jpeg: quality not in range 0..100: 120");

      o.put("jpeg_quality", 100);
      image_save_jpeg(img, "test_jpeg/img_32_100.jpg", o);
      I = image_load_jpeg("test_jpeg/img_32_100.jpg", 1);
      assert(color_dist(I.get_argb(10,10),   0xff141400) < 5);
      assert(color_dist(I.get_argb(117,117), 0xffeaea00) < 5);
      assert(color_dist(I.get_argb(138,10),  0xFF090000) < 5);
      assert(color_dist(I.get_argb(245,117), 0xffE90000) < 5);
      assert(color_dist(I.get_argb(64,64),   0xFF808000) < 5);
      assert(color_dist(I.get_argb(192,64),  0xFF800000) < 5);
    }

    /*********************************************/
    { // IMAGE_24RGB
      Image img(256,128, IMAGE_24RGB);
      for (int y=0; y<img.height(); ++y){
        for (int x=0; x<img.width(); ++x){
          img.set24(x,y, color_rem_transp(img32.get32(x,y), false));
        }
      }
      image_save_jpeg(img, "test_jpeg/img_24_def.jpg");
      Image I = image_load_jpeg("test_jpeg/img_24_def.jpg", 1);
      assert_eq(I.type(), IMAGE_24RGB);
      assert_eq(I.width(), 256);
      assert_eq(I.height(), 128);
      assert(color_dist(I.get_argb(10,10),   0xff141400) < 5);
      assert(color_dist(I.get_argb(117,117), 0xffeaea00) < 5);
      assert(color_dist(I.get_argb(138,10),  0xFF090000) < 5);
      assert(color_dist(I.get_argb(245,117), 0xffE90000) < 5);
      assert(color_dist(I.get_argb(64,64),   0xFF808000) < 5);
      assert(color_dist(I.get_argb(192,64),  0xFF800000) < 5);
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

      image_save_jpeg(img, "test_jpeg/img_16_def.jpg");
      Image I = image_load_jpeg("test_jpeg/img_16_def.jpg", 1);
      assert_eq(I.type(), IMAGE_24RGB);
      assert_eq(I.width(), 256);
      assert_eq(I.height(), 128);
      assert(color_dist(I.get_argb(10,10),   0xff111111) < 2);
      assert(color_dist(I.get_argb(117,117), 0xffcfcfcf) < 2);
      assert(color_dist(I.get_argb(138,10),  0xFF030303) < 2);
      assert(color_dist(I.get_argb(245,117), 0xff454545) < 2);
      assert(color_dist(I.get_argb(64,64),   0xFF717171) < 2);
      assert(color_dist(I.get_argb(192,64),  0xFF262626) < 2);
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

      image_save_jpeg(img, "test_jpeg/img_8_def.jpg");
      Image I = image_load_jpeg("test_jpeg/img_8_def.jpg", 1);
      assert_eq(I.type(), IMAGE_24RGB);
      assert_eq(I.width(), 256);
      assert_eq(I.height(), 128);
      assert(color_dist(I.get_argb(10,10),   0xff111111) < 5);
      assert(color_dist(I.get_argb(117,117), 0xffcfcfcf) < 5);
      assert(color_dist(I.get_argb(138,10),  0xFF030303) < 5);
      assert(color_dist(I.get_argb(245,117), 0xff454545) < 5);
      assert(color_dist(I.get_argb(64,64),   0xFF717171) < 5);
      assert(color_dist(I.get_argb(192,64),  0xFF262626) < 5);
    }


    { // IMAGE_8PAL
      std::vector<uint32_t> colors = image_colormap(img32);
      Image img = image_remap(img32, colors);
      image_save_jpeg(img, "test_jpeg/img_8p_def.jpg");
      Image I = image_load_jpeg("test_jpeg/img_8p_def.jpg", 1);
      assert_eq(I.type(), IMAGE_24RGB);
      assert_eq(I.width(), 256);
      assert_eq(I.height(), 128);
      assert(color_dist(I.get_argb(10,10),   0xff101801) < 5);
      assert(color_dist(I.get_argb(117,117), 0xffe6e600) < 5);
      assert(color_dist(I.get_argb(138,10),  0xFF0a0000) < 5);
      assert(color_dist(I.get_argb(245,117), 0xffe60001) < 5);
      assert(color_dist(I.get_argb(64,64),   0xFF858501) < 5);
      assert(color_dist(I.get_argb(192,64),  0xFF820201) < 5);
    }

    { // IMAGE_1
      Image img(256,128, IMAGE_1);
      for (int y=0; y<img.height(); ++y){
        for (int x=0; x<img.width(); ++x){
          img.set1(x,y, (int)(600*sin(2*M_PI*x/255)*sin(2*M_PI*y/255))%2);
        }
      }
      image_save_jpeg(img, "test_jpeg/img_1_def.jpg");
      Image I = image_load_jpeg("test_jpeg/img_1_def.jpg", 1);
      assert_eq(I.type(), IMAGE_24RGB);
      assert_eq(I.width(), 256);
      assert_eq(I.height(), 128);
      assert_eq(img.get1(0,0), 0);
      assert_eq(img.get1(15,45), 0);
      assert_eq(img.get1(43,123), 1);
      assert_eq(img.get1(203,27), 0);
      assert_eq(I.get_argb(0,0), 0xFF000000);
      assert_eq(I.get_argb(15,45), 0xFF000000);
      assert_eq(I.get_argb(43,123), 0xFFFFFFFF);
      assert_eq(I.get_argb(203,27), 0xFF000000);
    }

    { //scale tests
      Image I0 = image_load_jpeg("test_jpeg/img_32_def.jpg", 1);
      iPoint pt(101,32);
      for (double sc=1; sc<10; sc+=0.8){
        Image I1 = image_load_jpeg("test_jpeg/img_32_def.jpg", sc);
        assert_eq(I1.width(), floor((I0.width()-1)/sc+1));
        assert_eq(I1.height(), floor((I0.height()-1)/sc+1));
        iPoint pt1 = (dPoint)pt/sc;
        assert(color_dist(I1.get_rgb(pt1.x, pt1.y),
                          I0.get_rgb(rint(pt1.x*sc), rint(pt1.y*sc))) < 10);

        pt = iPoint(I0.width()-1, I0.height()-1);
        pt1 = (dPoint)pt/sc;
        assert(pt1.x < I1.width());
        assert(pt1.y < I1.height());
        assert(color_dist(I1.get_rgb(pt1.x, pt1.y),
                          I0.get_rgb(rint(pt1.x*sc), rint(pt1.y*sc))) < 20);
      }
      assert_err(image_load_jpeg("test_jpeg/img_32_def.jpg", 0),
        "image_load_jpeg: wrong scale: 0");
    }

/*
std::cerr << std::hex << I.get_argb(10,10) << "\n";
std::cerr << std::hex << I.get_argb(117,117) << "\n";
std::cerr << std::hex << I.get_argb(138,10) << "\n";
std::cerr << std::hex << I.get_argb(245,117) << "\n";
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