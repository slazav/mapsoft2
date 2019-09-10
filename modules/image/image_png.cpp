#include <cstring>
#include <png.h>
#include "image_png.h"
#include "image_colors.h"

/**********************************************************/

iPoint
image_size_png(const std::string & file){

  FILE * infile = NULL;
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL, end_info = NULL;

  png_uint_32 w, height;

  try {

    if ((infile = fopen(file.c_str(), "rb")) == NULL)
      throw Err() << "image_size_png: can't open file: " << file;

    png_byte sign[8];
    const char sign_size = 8;
    if ((fread(sign, 1,sign_size,infile)!=sign_size)||
        (png_sig_cmp(sign, 0, sign_size)!=0))
      throw Err() << "image_size_png: not a PNG file: " << file;

    png_ptr = png_create_read_struct
       (PNG_LIBPNG_VER_STRING, NULL,NULL,NULL);
    if (!png_ptr) throw Err() << "image_size_png: can't make png_read_struct";

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) throw Err() << "image_size_png: can't make png_info_struct";

    end_info = png_create_info_struct(png_ptr);
    if (!end_info) throw Err() << "image_size_png: can't make png_info_struct";

    if (setjmp(png_jmpbuf(png_ptr)))
      throw Err() << "image_size_png: can't do setjmp";

    png_init_io(png_ptr, infile);
    png_set_sig_bytes(png_ptr, sign_size);
    png_read_info(png_ptr, info_ptr);

    int bit_depth, color_type, interlace_type;
    png_get_IHDR(png_ptr, info_ptr, &w, &height,
       &bit_depth, &color_type, &interlace_type ,NULL,NULL);

    throw Err();
  }
  catch(Err e) {
    if (png_ptr) png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    if (infile) fclose(infile);
    if (e.str() != "") throw e;
  }
  return iPoint(w, height);
}




/**********************************************************/

Image
image_load_png(const std::string & file, const double scale){

  FILE * infile = NULL;
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL, end_info = NULL;

  png_bytep row_buf = NULL;
  Image img;

  try {

    if (scale < 1)
      throw Err() << "image_load_png: wrong scale: " << scale;

    if ((infile = fopen(file.c_str(), "rb")) == NULL)
      throw Err() << "image_load_png: can't open file: " << file;

    png_byte sign[8];
    const char sign_size = 8;
    if ((fread(sign, 1,sign_size,infile)!=sign_size)||
        (png_sig_cmp(sign, 0, sign_size)!=0))
      throw Err() << "image_load_png: not a PNG file: " << file;

    png_ptr = png_create_read_struct
       (PNG_LIBPNG_VER_STRING, NULL,NULL,NULL);
    if (!png_ptr) throw Err() << "image_load_png: can't make png_read_struct";

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) throw Err() << "image_load_png: can't make png_info_struct";

    end_info = png_create_info_struct(png_ptr);
    if (!end_info) throw Err() << "image_load_png: can't make png_info_struct";

    if (setjmp(png_jmpbuf(png_ptr)))
      throw Err() << "image_load_png: can't do setjmp";

    png_init_io(png_ptr, infile);
    png_set_sig_bytes(png_ptr, sign_size);
    png_read_info(png_ptr, info_ptr);

    png_uint_32 w=0, h=0;
    int bit_depth, color_type, interlace_type;
    png_get_IHDR(png_ptr, info_ptr, &w, &h,
       &bit_depth, &color_type, &interlace_type ,NULL,NULL);


    // scaled image size
    int w1 = floor((w-1)/scale+1);
    int h1 = floor((h-1)/scale+1);


    // Make image of the correct type
    if (color_type & PNG_COLOR_MASK_PALETTE){

      if (bit_depth == 16)
        png_set_strip_16(png_ptr);

      img = Image(w1,h1, IMAGE_8PAL);
      int cnum = 0;
      png_color *palette;
      png_get_PLTE(png_ptr, info_ptr, &palette, &cnum);

      img.cmap.resize(cnum);
      for (int i=0; i<cnum; i++){
        img.cmap[i] = 0xFF000000 +
          (palette[i].red << 16) +
          (palette[i].green << 8) +
          palette[i].blue;
          //std::cerr << "PAL: " << i << " " << std::hex << img.cmap[i] << "\n";
      }

      // transparent entries for palette
      int num_trans = 0;
      png_byte *trans;
      png_color_16_struct *trans_values;
      png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, &trans_values);
      if (num_trans > cnum)
        throw Err() << "image_load_png: bad tRNS";
      for (int i=0; i<num_trans; i++){
        int a = trans[i];
        int r = (img.cmap[i] >> 16) & 0xFF;
        int g = (img.cmap[i] >> 8) & 0xFF;
        int b = img.cmap[i] & 0xFF;
        img.cmap[i] = color_argb(a,r,g,b);
        //std::cerr << "TRNS: " << i << " " << std::hex << img.cmap[i] << "\n";
      }
    }

    else if (color_type == PNG_COLOR_TYPE_RGB){
      if (bit_depth == 16)
        png_set_strip_16(png_ptr);

      img = Image(w1,h1, IMAGE_24RGB);
    }

    else if (color_type == PNG_COLOR_TYPE_GRAY){
      if (bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);

      if (bit_depth <= 8)
        img = Image(w1,h1, IMAGE_8);
      else
        img = Image(w1,h1, IMAGE_16);
    }

    else {
      img = Image(w1,h1, IMAGE_32ARGB);

      if (bit_depth == 16)
        png_set_strip_16(png_ptr);

      if (color_type == PNG_COLOR_TYPE_GRAY ||
          color_type == PNG_COLOR_TYPE_GRAY_ALPHA){
        if (bit_depth < 8)
          png_set_expand_gray_1_2_4_to_8(png_ptr);
        png_set_gray_to_rgb(png_ptr);
      }

      if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
          png_set_tRNS_to_alpha(png_ptr);

      if (!(color_type & PNG_COLOR_MASK_ALPHA))
        png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);

    }


    png_read_update_info(png_ptr, info_ptr);
    row_buf = (png_bytep)png_malloc(png_ptr,
        png_get_rowbytes(png_ptr, info_ptr));
    if (!row_buf) throw Err() << "PNG: malloc error";


    /// Main loop

    int line = 0;
    for (int y=0; y<h1; ++y){

      while (line<=rint(y*scale)){
        png_read_row(png_ptr, row_buf, NULL);
        line++;
      }

      if (img.type() == IMAGE_8PAL){
        for (int x=0; x<w1; ++x){
          int xs = scale==1.0? x:rint(x*scale);
          img.data()[w1*y+x] = row_buf[xs];
        }
      }

      else if (img.type() == IMAGE_24RGB){
        for (int x=0; x<w1; ++x){
          int xs3 = 3*(scale==1.0? x:rint(x*scale));
          memcpy(img.data()+3*(y*w1+x), row_buf + xs3, 3);
        }
      }

      else if (img.type() == IMAGE_16){
        for (int x=0; x<w1; ++x){
          int xs = scale==1.0? x:rint(x*scale);
          ((uint16_t*)img.data())[w1*y+x] = ((uint16_t*)row_buf)[xs];
        }
      }

      else if (img.type() == IMAGE_8){
        for (int x=0; x<w1; ++x){
          int xs = scale==1.0? x:rint(x*scale);
          img.data()[w1*y+x] = row_buf[xs];
        }
      }

      else {
        for (int x=0; x<w1; ++x){
          int xs4 = 4*(scale==1.0? x:rint(x*scale));
          uint32_t c = color_argb(row_buf[xs4+3], row_buf[xs4],
                                  row_buf[xs4+1], row_buf[xs4+2]);
          img.set32(x,y, c);
        }
      }
    }

    throw Err();
  }
  catch(Err e) {
    if (row_buf) png_free(png_ptr, row_buf);
    if (png_ptr) png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    if (infile) fclose(infile);
    if (e.str() != "") throw e;
  }
  return img;
}

/*
  TODO: Interlaced images support?
  if (interlace_type == PNG_INTERLACE_ADAM7){
    // Interlaced images
    // pass 1: scale 1/8
    // passes 1..3: scale 1/4
    // passes 1..5: scale 1/2

    // PNG_INTERLACE_ADAM7:
    // Xooooooo ooooXooo oooooooo ooXoooXo oooooooo oXoXoXoX oooooooo
    // oooooooo oooooooo oooooooo oooooooo oooooooo oooooooo XXXXXXXX
    // oooooooo oooooooo oooooooo oooooooo XoXoXoXo oXoXoXoX oooooooo
    // oooooooo oooooooo oooooooo oooooooo oooooooo oooooooo XXXXXXXX
    // oooooooo oooooooo XoooXooo ooXoooXo oooooooo oXoXoXoX oooooooo
    // oooooooo oooooooo oooooooo oooooooo oooooooo oooooooo XXXXXXXX
    // oooooooo oooooooo oooooooo oooooooo XoXoXoXo oXoXoXoX oooooooo
    // oooooooo oooooooo oooooooo oooooooo oooooooo oooooooo XXXXXXXX
  }
*/


/**********************************************************/

void
image_save_png(const Image & im, const std::string & file,
               const Opt & opt){

  FILE *outfile = NULL;
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  png_bytep buf = NULL;

  try {

    // Choose default PNG color type
    int color_type;
    int bits = 8; // only 8-bit colors are supported now
    switch (im.type()){
      case IMAGE_32ARGB: color_type = PNG_COLOR_TYPE_RGB_ALPHA; break;
      case IMAGE_24RGB:  color_type = PNG_COLOR_TYPE_RGB;  break;
      case IMAGE_16:     color_type = PNG_COLOR_TYPE_GRAY; bits = 16; break;
      case IMAGE_8:      color_type = PNG_COLOR_TYPE_GRAY; break;
      case IMAGE_1:      color_type = PNG_COLOR_TYPE_PALETTE; break;
      case IMAGE_8PAL:   color_type = PNG_COLOR_TYPE_PALETTE; break;
      case IMAGE_FLOAT:  color_type = PNG_COLOR_TYPE_RGB;  break;
      case IMAGE_DOUBLE: color_type = PNG_COLOR_TYPE_RGB;  break;
      case IMAGE_UNKNOWN: color_type = PNG_COLOR_TYPE_RGB;  break;
    }

    // set PNG color type from options
    std::string str;
    str = opt.get("png_format", "");
    if (str != "") {
      if      (str == "argb")  {color_type = PNG_COLOR_TYPE_RGB_ALPHA;  bits=8;}
      else if (str == "rgb")   {color_type = PNG_COLOR_TYPE_RGB;        bits=8;}
      else if (str == "grey")  {color_type = PNG_COLOR_TYPE_GRAY;       bits=8;}
      else if (str == "agrey") {color_type = PNG_COLOR_TYPE_GRAY_ALPHA; bits=8;}
      else if (str == "pal")   {color_type = PNG_COLOR_TYPE_PALETTE;    bits=8;}
      else throw Err() << "image_save_png: unknown png_format setting: " << str << "\n";
    }

    // png palette
    Image im8 = im;
    if (str == "pal"){
      std::vector<uint32_t> colors = image_colormap(im, opt);
      im8 = image_remap(im, colors, opt);
    }

    // open file
    outfile = fopen(file.c_str(), "wb");
    if (!outfile) throw Err() << "image_save_png: can't open file: " << file;

    png_ptr = png_create_write_struct
      (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
      throw Err() << "image_save_png: can't make png_read_struct";

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
      throw Err() << "image_save_png: can't make png_info_struct";

    if (setjmp(png_jmpbuf(png_ptr)))
      throw Err() << "image_save_png: can't do setjmp";

    png_init_io(png_ptr, outfile);


    png_set_IHDR(png_ptr, info_ptr, im.width(), im.height(),
       bits, color_type, PNG_INTERLACE_NONE,
       PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    if (color_type == PNG_COLOR_TYPE_PALETTE){

      if (im8.type()==IMAGE_1){
        im8.cmap.resize(2);
        im8.cmap[0]=0xFF000000;
        im8.cmap[1]=0xFFFFFFFF;
      }
      else if (im8.type()!=IMAGE_8PAL)
        throw Err() << "image_save_png: inconsistent palette setting";
      int cnum = im8.cmap.size();

      png_color pcolors[cnum];
      for (int i=0; i<cnum; i++){
       uint32_t c = color_rem_transp(im8.cmap[i], false);
        pcolors[i].red   = (c >>16) & 0xFF;
        pcolors[i].green = (c >>8)  & 0xFF;
        pcolors[i].blue  =  c & 0xFF;
      }
      png_set_PLTE(png_ptr, info_ptr, pcolors, cnum);

      //transparent palette colors
      png_byte trans[cnum];
      bool is_transp = false;
      for (int i=0; i<cnum; i++){
        trans[i] = (im8.cmap[i]>>24) & 0xFF;
        if (trans[i] != 0xFF) is_transp = true;
      }
      if (is_transp) png_set_tRNS(png_ptr, info_ptr, trans, cnum, 0);
    }

    png_write_info(png_ptr, info_ptr);
    // for 16-bit RGB this will not be enough!
    png_bytep buf = (png_bytep)png_malloc(png_ptr, im.width()*4);
    if (!buf) throw Err() << "PNG: malloc error";

    for (int y=0; y<im.height(); y++){
      for (int x=0; x<im.width(); x++){
        uint32_t c;
        switch (color_type){
        case PNG_COLOR_TYPE_GRAY:
          if (bits==8)
            buf[x] = im.get_grey8(x, y);
          else
            ((uint16_t*)buf)[x] = im.get_grey16(x, y);
          break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
          ((uint16_t*)buf)[x]  = im.get_agrey8(x, y);
          break;
        case PNG_COLOR_TYPE_RGB:
          c = im.get_rgb(x, y);
          buf[3*x]   = (c >> 16) & 0xFF;
          buf[3*x+1] = (c >> 8) & 0xFF;
          buf[3*x+2] = c & 0xFF;
          break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
          c = im.get_argb(x, y);
          buf[4*x+3] = (c>>24) & 0xFF;
          c = color_rem_transp(c, false); // unscale color!
          buf[4*x]   = (c >> 16) & 0xFF;
          buf[4*x+1] = (c >> 8) & 0xFF;
          buf[4*x+2] = c & 0xFF;
          break;
        case PNG_COLOR_TYPE_PALETTE:
          if (im8.type() == IMAGE_8PAL)
            buf[x] = im8.get8(x,y);
          else if (im8.type() == IMAGE_1)
            buf[x] = (uint8_t)im8.get1(x,y);
          break;
        }
      }
      png_write_row(png_ptr, buf);
    }

    png_write_end(png_ptr, info_ptr);
    throw Err();
  }
  catch (Err e) {
    if (buf)     png_free(png_ptr, buf);
    if (png_ptr) png_destroy_write_struct(&png_ptr, &info_ptr);
    if (outfile) fclose(outfile);
    if (e.str() != "") throw e;
  }
}
