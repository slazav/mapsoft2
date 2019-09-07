#include "image_png.h"
#include "image/image_colors.h"
#include <png.h>
#include <cstring>
#include "imgsrc.h"

class ImageSourcePNG : ImageSource {
private:
  FILE * infile;
  png_structp png_ptr;
  png_infop info_ptr, end_info;

  png_uint_32 width, height;
  int bit_depth, color_type, interlace_type;
  png_bytep row_buf;

  int line;

public:

  // Constructor. Read PNG file until image size will be available
  ImageSourcePNG(const std::string & file):
        infile(NULL), row_buf(NULL),
        png_ptr(NULL), info_ptr(NULL), end_info(NULL),
        line(-1), width(0), height(0),
        bit_depth(0), color_type(0), interlace_type(0){

    if ((infile = fopen(file.c_str(), "rb")) == NULL)
      throw Err() << "PNG error: can't open file: " << file;

    png_byte sign[8];
    const char sign_size = 8;
    if ((fread(sign, 1,sign_size,infile)!=sign_size)||
        (png_sig_cmp(sign, 0, sign_size)!=0))
      throw Err() << "PNG error: not a PNG file: " << file;

    png_ptr = png_create_read_struct
       (PNG_LIBPNG_VER_STRING, NULL,NULL,NULL);
    if (!png_ptr)
      throw Err() << "PNG error: can't make png_read_struct";

    info_ptr = png_create_info_struct(png_ptr);
    end_info = png_create_info_struct(png_ptr);
    if ((!info_ptr) || (!end_info))
      throw Err() << "PNG error: can't make png_info_struct";

    if (setjmp(png_jmpbuf(png_ptr)))
      throw Err() << "PNG error: can't do setjmp";

    png_init_io(png_ptr, infile);
    png_set_sig_bytes(png_ptr, sign_size);
    png_read_info(png_ptr, info_ptr);

    png_get_IHDR(png_ptr, info_ptr, &width, &height,
       &bit_depth, &color_type, &interlace_type ,NULL,NULL);
  }

  // destructor
  ~ImageSourcePNG(){
    if (row_buf) png_free(png_ptr, row_buf);
    if (png_ptr) png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    if (infile) fclose(infile);
  }

  /// Image dimensions
  iPoint size() const{ return iPoint(width, height); }

  /// Get bits per pixed for data
  int get_bpp() const  {return 32;}

  /// Get data line number
  int get_line() const{ return line; }

  /// Goto line n (n should be >= get_line())
  void goto_line(const int n){

    if (line<0){ // prepare buffers
      // color transformation
      if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);

      if (color_type == PNG_COLOR_TYPE_GRAY ||
          color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);

      if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

      if (!(color_type & PNG_COLOR_MASK_ALPHA))
        png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);

      if (bit_depth == 16)
        png_set_strip_16(png_ptr);
      //
      png_read_update_info(png_ptr, info_ptr);
      row_buf = (png_bytep)png_malloc(png_ptr,
        png_get_rowbytes(png_ptr, info_ptr));

      if (!row_buf) throw Err() << "PNG: malloc error";

    }
    if (n>=height) throw Err() << "PNG: too large line number";
    if (n<line) throw Err() << "PNG: can't go back in image lines";
    for (;line<n; ++line)
      png_read_row(png_ptr, row_buf, NULL);
  }

  /// Get raw data line
  unsigned char* get_data() const {return (unsigned char*)row_buf;}

  /// Get color value (at x coordinate of the current line)
  /// (no range checking!)
  unsigned int get_col(const int x) const {
    int r = row_buf[4*x];
    int g = row_buf[4*x+1];
    int b = row_buf[4*x+2];
    int a = row_buf[4*x+3];
    if (a<255){
      r = (r*a)/255;
      g = (g*a)/255;
      b = (b*a)/255;
    }
    return (a<<24) + (r<<16) + (g<<8) + b;
  }

};

/**********************************************************/

iPoint
image_size_png(const std::string & file){
  ImageSourcePNG SRC(file);
  return SRC.size();
}

Image
image_load_png(const std::string & file, const int scale){

  ImageSourcePNG SRC(file);
  iPoint size = SRC.size();
  Image img(size.x, size.y, IMAGE_32ARGB);

  for (int y=0; y<size.y; ++y){
    SRC.goto_line(y);
    for (int x=0; x<size.x; ++x) img.set32(x,y, SRC.get_col(x));
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



void
image_save_png(const Image & im, const std::string & file,
               const Opt & opt){

  FILE *outfile = NULL;
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  png_bytep buf = NULL;

  try {

    outfile = fopen(file.c_str(), "wb");
    if (!outfile) throw Err() << "PNG error: can't open file: " << file;

    png_ptr = png_create_write_struct
      (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
      throw Err() << "PNG error: can't make png_read_struct";

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
      throw Err() << "PNG error: can't make png_info_struct";

    if (setjmp(png_jmpbuf(png_ptr)))
      throw Err() << "PNG error: can't do setjmp";

    png_init_io(png_ptr, outfile);


    // Choose default PNG color type
    int color_type;
    int bits = 8; // only 8-bit colors are supported now
    switch (im.type()){
      case IMAGE_32ARGB: color_type = PNG_COLOR_TYPE_RGB_ALPHA; break;
      case IMAGE_24RGB:  color_type = PNG_COLOR_TYPE_RGB;  break;
      case IMAGE_16:     color_type = PNG_COLOR_TYPE_GRAY; break;
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
      if      (str == "argb")  color_type = PNG_COLOR_TYPE_RGB_ALPHA;
      else if (str == "rgb")   color_type = PNG_COLOR_TYPE_RGB;
      else if (str == "grey")  color_type = PNG_COLOR_TYPE_GRAY;
      else if (str == "agrey") color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
      else if (str == "pal")   color_type = PNG_COLOR_TYPE_PALETTE;
      else throw Err() << "image_save_png: unknown png_format setting: " << str << "\n";
    }

    png_set_IHDR(png_ptr, info_ptr, im.width(), im.height(),
       bits, color_type, PNG_INTERLACE_NONE,
       PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // png palette
    Image im8 = im;
    if (color_type == PNG_COLOR_TYPE_PALETTE){

      int maxcol = 256;
      if (im8.type()==IMAGE_1){
        maxcol = 2;
        im8.cmap[0]=0xFF000000;
        im8.cmap[1]=0xFFFFFFFF;
      }
      else if (im8.type()!=IMAGE_8PAL){
        std::vector<uint32_t> colors = image_colormap(im, opt);
        im8 = image_remap(im, colors, opt);
        maxcol = colors.size();
      }

      png_color pcolors[maxcol];
      for (int i=0; i<maxcol; i++){
        pcolors[i].red   = (im8.cmap[i] >>16) & 0xFF;
        pcolors[i].green = (im8.cmap[i] >>8)  & 0xFF;
        pcolors[i].blue  =  im8.cmap[i] & 0xFF;
      }
      png_set_PLTE(png_ptr, info_ptr, pcolors, maxcol);

      //transparent palette colors
      png_byte trans[maxcol];
      bool is_transp = false;
      for (int i=0; i<maxcol; i++){
        trans[i] = (im8.cmap[i]>>24) & 0xFF;
        if (trans[i] != 0xFF) is_transp = true;
      }
      if (is_transp) png_set_tRNS(png_ptr, info_ptr, trans, maxcol, 0);
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
          buf[x] = im.get_grey8(x, y);
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
