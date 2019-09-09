#include <tiffio.h>
#include <cstring>
#include "image_tiff.h"
#include "image/image_colors.h"

// custom error handler
#include <cstdarg>
void
my_error_exit (const char* module, const char* fmt, const char *x, ...) {
  char buf[1024];
  va_list args;
  va_start (args, x); // strange extra parameter???
  vsnprintf(buf, 1024, fmt, args);
  va_end (args);
  throw Err() << module << " error: " << buf;
}


/**********************************************************/

// getting file dimensions
iPoint image_size_tiff(const std::string & file){
  TIFF* tif = NULL;
  uint32_t w, h;

  try {
    TIFFSetErrorHandler((TIFFErrorHandler)&my_error_exit);

    tif = TIFFOpen(file.c_str(), "rb");
    if (!tif) throw Err() << "image_size_tiff: can't open file: " << file;

    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);

    throw Err();
  }
  catch (Err e){
    if (tif) TIFFClose(tif);
    if (e.str()!="") throw e;
  }
  return iPoint(w,h);
}

/**********************************************************/

Image
image_load_tiff(const std::string & file, const double scale){
  TIFF* tif = NULL;
  uint8_t *cbuf = NULL;
  Image img;

  try {

    if (scale < 1)
      throw Err() << "image_load_tiff: wrong scale: " << scale;

    TIFFSetErrorHandler((TIFFErrorHandler)&my_error_exit);

    tif = TIFFOpen(file.c_str(), "rb");
    if (!tif) throw Err() << "image_load_tiff: can't open file: " << file;

    uint32_t w, h;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
    int scan = TIFFScanlineSize(tif);
    int bpp = scan/w;


    // can we do random access to lines?
    int compression_type, rows_per_strip;
    TIFFGetField(tif, TIFFTAG_COMPRESSION,  &compression_type);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    bool can_skip_lines =
      (compression_type==COMPRESSION_NONE)||(rows_per_strip==1);

    uint16_t photometric;
    uint32_t colors[256];
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);

    if(photometric == PHOTOMETRIC_RGB ||
       photometric == PHOTOMETRIC_MINISWHITE ||
       photometric == PHOTOMETRIC_MINISBLACK){
    }
    else if (photometric == PHOTOMETRIC_PALETTE){
      uint16 *cmap[3];
      TIFFGetField(tif, TIFFTAG_COLORMAP, cmap, cmap+1, cmap+2);
      for (int i=0; i<256; i++){
        colors[i] =
          (((uint32)cmap[0][i] & 0xFF00)<<8) +
           ((uint32)cmap[1][i] & 0xFF00) +
          (((uint32)cmap[2][i] & 0xFF00)>>8) +
          (0xFF<<24);
      }
    }
    else
      throw Err() << "image_load_tiff: unsupported photometric type: " << photometric;

    // allocate buffer
    cbuf = (uint8 *)_TIFFmalloc(scan);

    // scaled image
    int w1 = floor((w-1)/scale+1);
    int h1 = floor((h-1)/scale+1);
    img = Image(w1,h1, IMAGE_32ARGB);

    // Main loop

    int line = 0;
    for (int y=0; y<h1; ++y){

      if (can_skip_lines) line = y*scale;

      while (line<=rint(y*scale)){
        TIFFReadScanline(tif, cbuf, line);
        line++;
      }

      for (int x=0; x<w1; ++x){
        uint32_t c;
        int xs = scale==1.0? x:rint(x*scale);
        switch (photometric){

          case PHOTOMETRIC_PALETTE:
            c = colors[cbuf[xs]]; break;

          case PHOTOMETRIC_RGB:
            if (bpp==3){ // RGB
              c = 0xFF000000 + (cbuf[3*xs]<<16) + (cbuf[3*xs+1]<<8) + cbuf[3*xs+2];
              break;
            }
            if (bpp==4){ // RGBA
              c = (cbuf[4*xs]<<16) + (cbuf[4*xs+1]<<8) + cbuf[4*xs+2] + (cbuf[4*xs+3]<<24);
              break;
            }
            if (bpp==1){ // G
              c = cbuf[xs] + (cbuf[xs]<<8) + (cbuf[xs]<<16) + 0xFF000000;
              break;
            }
            throw Err() << "image_load_tiff: unsupported format: PHOTOMETRIC_RGB, bpp: " << bpp;

          case PHOTOMETRIC_MINISWHITE:
            if (bpp==2){ // 16bit
              c = 0xFFFFFFFF - (cbuf[2*xs+1] + (cbuf[2*xs+1]<<8) + (cbuf[2*xs+1]<<16));
              break;
            }
            if (bpp==1){ // 8bit
              c = 0xFFFFFFFF - (cbuf[xs] + (cbuf[xs]<<8) + (cbuf[xs]<<16));
              break;
            }
            throw Err() << "image_load_tiff: unsupported format: PHOTOMETRIC_MINISWHITE, bpp: " << bpp;

          case PHOTOMETRIC_MINISBLACK:
            if (bpp==2){ // 16bit
              c = cbuf[2*xs+1] + (cbuf[2*xs+1]<<8) + (cbuf[2*xs+1]<<16) + 0xFF000000;
              break;
            }
            if (bpp==1){ // 8bit
              c = cbuf[xs] + (cbuf[xs]<<8) + (cbuf[xs]<<16) + 0xFF000000;
              break;
            }
            throw Err() << "image_load_tiff: unsupported format: PHOTOMETRIC_MINISBLACK, bpp: " << bpp;

          default: throw Err() << "image_load_tiff: unsupported data format: photometric: "
                               << photometric << " bpp: " << bpp;
        }
        img.set32(x,y,c);
      }
    }
    throw Err();
  }
  catch (Err e) {
    if (cbuf) _TIFFfree(cbuf);
    if (tif) TIFFClose(tif);
    if (e.str() != "") throw e;
  }
  return img;
}

/**********************************************************/

void image_save_tiff(const Image & im, const std::string & file, const Opt & opt){

  TIFF *tif = NULL;
  tdata_t buf = NULL;

  try {
    int samples = 3; // samples per pixel
    int bps = 8;     // bits per sample
    bool use_cmap = false;
    switch (im.type()){
      case IMAGE_32ARGB:  samples=4; break;
      case IMAGE_24RGB:   samples=3; break;
      case IMAGE_16:      samples=1; bps=16; break;
      case IMAGE_8:       samples=1; break;
      case IMAGE_1:       samples=1; break;
      case IMAGE_8PAL:    samples=1; use_cmap=1; break;
      case IMAGE_FLOAT:   samples=3; break;
      case IMAGE_DOUBLE:  samples=3; break;
      case IMAGE_UNKNOWN: samples=4; break;
    }

    // set TIFF type from options
    std::string str;
    str = opt.get("tiff_format", "");
    if (str != "") {
      if      (str == "argb")  {bps=8; samples = 4; use_cmap = 0; }
      else if (str == "rgb")   {bps=8; samples = 3; use_cmap = 0; }
      else if (str == "grey")  {bps=8; samples = 1; use_cmap = 0; }
      else if (str == "pal")   {bps=8; samples = 1; use_cmap = 1;}
      else throw Err() << "image_save_tiff: unknown tiff_format setting: " << str << "\n";
    }

    // palette
    Image im8 = im;
    if (str == "pal"){
      Opt opt1(opt);
      opt1.put("cmap_alpha", "none");
      std::vector<uint32_t> colors = image_colormap(im, opt1);
      im8 = image_remap(im, colors, opt1);
    }

    tif = TIFFOpen(file.c_str(), "wb");
    if (!tif) throw Err() << "TIFF error: can't open file: " << file;

    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, im.width());
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, im.height());
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE,   bps);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG,    1);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP,    1);
    TIFFSetField(tif, TIFFTAG_COMPRESSION,     COMPRESSION_LZW);

    if (samples == 3 || samples == 4){
      TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    }

    if (samples == 4){
      int type=EXTRASAMPLE_UNASSALPHA;
      TIFFSetField(tif, TIFFTAG_EXTRASAMPLES,  1, &type);
    }

    if (samples == 1 && !use_cmap ){
      if (opt.get("tiff_minwhite", false))
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);
      else
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    }

    uint16 cmap[3][256];
    if (use_cmap){
      for (int i=0; i<im8.cmap.size(); i++){
        cmap[0][i] = (im8.cmap[i]>>8)&0xFF00;
        cmap[1][i] =  im8.cmap[i]    &0xFF00;
        cmap[2][i] = (im8.cmap[i]<<8)&0xFF00;
      }
      TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_PALETTE);
      TIFFSetField(tif, TIFFTAG_COLORMAP, cmap, cmap+1, cmap+2);
    }

    // note: only for bps>=8!
    int scan = samples*bps*im.width()/8;

    buf = _TIFFmalloc(scan);
    uint8 *cbuf = (uint8 *)buf;

    for (int y=0; y<im.height(); y++){
      for (int x=0; x<im.width(); x++){
        uint32_t c;
        //uint16_t c16;
        switch (samples*bps){
          case 32:
            c = im.get_argb(x, y);
            cbuf[4*x]   = (c >> 16) & 0xFF;
            cbuf[4*x+1] = (c >> 8)  & 0xFF;
            cbuf[4*x+2] = c & 0xFF;
            cbuf[4*x+3] = (c >> 24) & 0xFF;
            break;
          case 24:
            c = im.get_rgb(x, y);
            cbuf[3*x]   = (c >> 16) & 0xFF;
            cbuf[3*x+1] = (c >> 8)  & 0xFF;
            cbuf[3*x+2] = c & 0xFF;
            break;
          case 16:
            //c16 = im.get_grey16(x, y);
            //cbuf[2*x]   = (c16>>8) & 0xFF;
            //cbuf[2*x+1] = c16 & 0xFF;

            ((uint16_t *)cbuf)[x] = im.get_grey16(x, y);

            break;
          case 8:
          if (use_cmap)
            cbuf[x] = im8.get8(x,y);
          else
            cbuf[x] = im.get_grey8(x,y);
          break;
        }
      }
      TIFFWriteScanline(tif, buf, y);
    }
    throw Err();
  }
  catch (Err e) {
    if (buf) _TIFFfree(buf);
    if (tif) TIFFClose(tif);
    if (e.str() != "") throw e;
  }
}


