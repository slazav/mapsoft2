#include "image_jpeg.h"
#include <jpeglib.h>
#include <stdio.h>

std::string jpeg_error_message;


// custom error handler
void
my_error_exit (j_common_ptr cinfo) {
  const char buffer[JMSG_LENGTH_MAX] = "";
  (*cinfo->err->format_message) (cinfo, (char *)buffer);
  throw Err() << "JPEG error: " << buffer;
}

/**********************************************************/
iPoint
image_size_jpeg(const std::string & file){

  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * infile;

  try {
    // open file, get image size
    cinfo.err = jpeg_std_error(&jerr);
    jerr.error_exit = my_error_exit;
    jpeg_create_decompress(&cinfo);
    if ((infile = fopen(file.c_str(), "rb")) == NULL)
      throw Err() << "JPEG error: can't open file: " << file;

    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);

    throw Err();
  }
  catch (Err e){
    jpeg_destroy_decompress(&cinfo);
    if (infile) fclose(infile);
    if (e.str() != "") throw e;
  }
  return iPoint(cinfo.image_width, cinfo.image_height);
}

/**********************************************************/
// todo: scale and denominator support!
Image
image_load_jpeg(const std::string & file, const int scale){

  unsigned char *buf = NULL;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * infile;
  int denom = 1;
  Image img;

  try {
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

    int w = cinfo.image_width;
    int h = cinfo.image_height;
    img = Image(w,h, IMAGE_32ARGB);

    jpeg_start_decompress(&cinfo);
    buf  = new unsigned char[(cinfo.image_width+1) * 3];

    // main loop

    for (int y=0; y<h; ++y){
      jpeg_read_scanlines(&cinfo, (JSAMPLE**)&buf, 1);
      for (int x=0; x<w; ++x){
        img.set32(x,y,
          0xFF000000 + buf[3*x+2] + (buf[3*x+1]<<8) + (buf[3*x]<<16));
      }
    }

    throw Err();
  }
  catch (Err e){
    if (buf) delete[] buf;
    jpeg_abort_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    if (infile) fclose(infile);
    if (e.str() != "") throw e;
  }
  return img;
}

/**********************************************************/

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

    throw Err();
  }
  catch (Err e){
    if (buf) delete [] buf;
    jpeg_destroy_compress(&cinfo);
    if (outfile) fclose(outfile);
    if (e.str() != "") throw e;
  }
}
