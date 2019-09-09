#include "image_jpeg.h"
#include <jpeglib.h>
#include <stdio.h>
#include <cstring>

std::string jpeg_error_message;


// error prefix
std::string error_pref = "jpeg";
// custom error handler
void
my_error_exit (j_common_ptr cinfo) {
  const char buffer[JMSG_LENGTH_MAX] = "";
  (*cinfo->err->format_message) (cinfo, (char *)buffer);
  throw Err() << error_pref << ": " << buffer;
}

/**********************************************************/
iPoint
image_size_jpeg(const std::string & file){

  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * infile;

  // open file, get image size
  cinfo.err = jpeg_std_error(&jerr);
  jerr.error_exit = my_error_exit;
  error_pref = "image_size_jpeg";
  // note: it is an error to do jpeg_destroy_decompress
  // before jpeg_create_decompress.
  jpeg_create_decompress(&cinfo);

  try {
    if ((infile = fopen(file.c_str(), "rb")) == NULL)
      throw Err() << "image_size_jpeg: can't open file: " << file;

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
image_load_jpeg(const std::string & file, const double scale){

  unsigned char *buf = NULL;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * infile;
  Image img;

  // note: it is an error to do jpeg_destroy_decompress
  // before jpeg_create_decompress.
  cinfo.err = jpeg_std_error(&jerr);
  jerr.error_exit = my_error_exit;
  error_pref = "image_load_jpeg";
  jpeg_create_decompress(&cinfo);

  try {

    if (scale < 1)
      throw Err() << "image_load_jpeg: wrong scale: " << scale;

    // open file, get image size
    if ((infile = fopen(file.c_str(), "rb")) == NULL)
      throw Err() << "image_load_jpeg: can't open file: " << file;

    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);

    int denom;
    if (scale <2) denom = 1;
    else if (scale <4)  denom = 2;
    else if (scale <8)  denom = 4;
    else denom = 8;
    double sc = scale/denom;

    cinfo.out_color_space = JCS_RGB; // always load in RGB mode
    cinfo.scale_denom = denom; // set denominator
    jpeg_start_decompress(&cinfo);

    // scaled image
    int w = cinfo.output_width;
    int h = cinfo.output_height;
    int w1 = floor((cinfo.image_width-1)/scale+1);
    int h1 = floor((cinfo.image_height-1)/scale+1);
    img = Image(w1,h1, IMAGE_24RGB);
    // adjust scale
    sc = std::min((double)(w-1)/(w1-1), (double)(h-1)/(h1-1));


    // main loop

    if (0 && w==w1 && h==h1){
      for (int y=0; y<h; ++y){
        JSAMPLE *sbuf = img.data() + 3*y*w1;
        jpeg_read_scanlines(&cinfo, &sbuf, 1);
      }
    }
    else {
      // memory buffer
      buf  = new unsigned char[(w+1)*3];

      int line = 0;
      for (int y=0; y<h1; ++y){
        while (line<=rint(y*sc) && line<h){
          jpeg_read_scanlines(&cinfo, (JSAMPLE**)&buf, 1);
          line++;
        }
        uint8_t *dst_buf = img.data() + 3*y*w1;
        for (int x=0; x<w1; ++x){
          int xs3 = 3*rint(x*sc);
          memcpy(dst_buf + 3*x, buf + xs3, 3);
        }
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
      throw Err() << "image_save_jpeg: quality not in range 0..100: " << quality;

  FILE * outfile = NULL;
  unsigned char *buf = NULL;
  std::string msg;

  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  error_pref = "image_save_jpeg";
  jpeg_create_compress(&cinfo);

  try {

    if ((outfile = fopen(file, "wb")) == NULL)
     throw Err() << "image_load_jpeg: can't open file: " << file;

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
