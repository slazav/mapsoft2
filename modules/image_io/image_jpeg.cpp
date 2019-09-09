#include "image_jpeg.h"
#include <jpeglib.h>
#include <stdio.h>
#include "imgsrc.h"

std::string jpeg_error_message;


// custom error handler
void
my_error_exit (j_common_ptr cinfo) {
  const char buffer[JMSG_LENGTH_MAX] = "";
  (*cinfo->err->format_message) (cinfo, (char *)buffer);
  throw Err() << "JPEG error: " << buffer;
}



class ImageSourceJPEG : ImageSource {
private:
  int line;
  unsigned char *buf;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * infile;

public:
  ImageSourceJPEG(const std::string & file, int denom=1):
       buf(NULL), infile(NULL), line(-1) {

    // check denom value
    if (denom!=1 && denom!=2 && denom!=4 && denom!=8)
      throw Err() << "JPEG error: bad denominator value: " << denom;

    // open file, get image size
    cinfo.err = jpeg_std_error(&jerr);
    jerr.error_exit = my_error_exit;
    jpeg_create_decompress(&cinfo);
    if ((infile = fopen(file.c_str(), "rb")) == NULL)
      throw Err() << "JPEG error: can't open file: " << file;

    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);

    cinfo.out_color_space = JCS_RGB; // always load in RGB mode
    cinfo.scale_denom = denom; // set denominator

  }

  ~ImageSourceJPEG(){
    if (buf) delete[] buf;
    jpeg_abort_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    if (infile) fclose(infile);
  }

  iPoint size() const{ return iPoint(cinfo.image_width,cinfo.image_height); }

  int get_bpp() const  {return 24;}

  int get_line() const{ return line; }

  void goto_line(const int n){
    if (line<0){ // prepare buffers
      jpeg_start_decompress(&cinfo);
      buf  = new unsigned char[(cinfo.image_width+1) * 3];
    }
    if (n>=cinfo.image_height) throw Err() << "JPEG: too large line number";
    if (n<line) throw Err() << "JPEG: can't go back in image lines";
    for (;line<n; ++line)
      jpeg_read_scanlines(&cinfo, (JSAMPLE**)&buf, 1);
  }

  /// Get raw data line
  unsigned char* get_data() const {return buf;}

  /// Get color value (at x coordinate of the current line)
  /// (no range checking!)
  unsigned int get_col(const int x) const {
    return 0xFF000000 + buf[3*x+2] + (buf[3*x+1]<<8) +
           (buf[3*x]<<16);
  }

};


/**********************************************************/

iPoint
image_size_jpeg(const std::string & file){
  ImageSourceJPEG SRC(file, 1);
  return SRC.size();
}


// todo: scale and denominator support!
Image
image_load_jpeg(const std::string & file, const int scale){

  ImageSourceJPEG SRC(file);
  iPoint size = SRC.size();
  Image img(size.x, size.y, IMAGE_32ARGB);

  for (int y=0; y<size.y; ++y){
    SRC.goto_line(y);
    for (int x=0; x<size.x; ++x) img.set32(x,y, SRC.get_col(x));
  }
  return img;
}

/*
int
load(const std::string & file, const int scale){

    int denom=1;
    if (scale <2) denom = 1;
    else if (scale <4) denom = 2;
    else if (scale <8) denom = 4;
    else denom = 8;

    src_rect /= denom;

    try {
      ImageSourceJPEG source(file, denom);
      return source.render_to_image(image, src_rect, dst_rect)? 0:1;
    } catch(int x) {return x;}
}
*/


void
image_save_jpeg(const Image & im, const char *file, const Opt & opt){

  int quality = opt.get("jpeg_quality", 95);

  if ((quality<0)||(quality>100))
      throw Err() << "JPEG error: quality not in range 0..100: " << quality;

  FILE * outfile = NULL;
  unsigned char *buf = NULL;
  std::string msg;

  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);

  try {
    jpeg_create_compress(&cinfo);

    if ((outfile = fopen(file, "wb")) == NULL)
     throw Err() << "JPEG error: can't open file: " << file;

    jpeg_stdio_dest(&cinfo, outfile);
    cinfo.image_width = im.width();
    cinfo.image_height = im.height();
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality (&cinfo, quality, true);
    jpeg_start_compress(&cinfo, TRUE);

    buf = new unsigned char[im.width() * 3];

    for (int y = 0; y < im.height(); y++){
      for (int x = 0; x < im.width(); x++){
        int c = im.get_rgb(x, y);
        buf[3*x+2] = c & 0xFF;
        buf[3*x+1] = (c >> 8) & 0xFF;
        buf[3*x]   = (c >> 16) & 0xFF;
      }
      jpeg_write_scanlines(&cinfo, (JSAMPLE**)&buf, 1);
    }
    jpeg_finish_compress(&cinfo);
  }
  catch (Err e){ msg = e.str(); }

  if (buf) delete [] buf;
  jpeg_destroy_compress(&cinfo);
  if (outfile) fclose(outfile);
  if (msg!="") throw Err() << msg;
}
