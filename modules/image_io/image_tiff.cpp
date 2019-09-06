#include "image_tiff.h"
#include <tiffio.h>
#include <cstring>
#include "imgsrc.h"

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


class ImageSourceTIFF : ImageSource {
private:
  TIFF* tif;
  int line;
  uint32_t w, h;
  int scan, bpp;
  uint8_t *cbuf;
  bool can_skip_lines;
  uint16_t photometric;
  uint32_t colors[256];

public:
  ImageSourceTIFF(const std::string & file):
       tif(NULL), cbuf(NULL), line(-1), w(0), h(0), bpp(0), scan(0),
       photometric(-1), can_skip_lines(false) {

    TIFFSetErrorHandler((TIFFErrorHandler)&my_error_exit);

    tif = TIFFOpen(file.c_str(), "rb");
    if (!tif) throw Err() << "TIFF error: can't open file: " << file;

    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
    scan = TIFFScanlineSize(tif);
    bpp = scan/w;

  }

  ~ImageSourceTIFF(){
    if (cbuf) _TIFFfree(cbuf);
    if (tif) TIFFClose(tif);
  }

  iPoint size() const { return iPoint(w,h); }

  int get_bpp() const  {return 32;}

  int get_line() const{ return line; }

  void prepare_reading(){ // should be done once before reading data
    cbuf = (uint8 *)_TIFFmalloc(scan);
    int compression_type, rows_per_strip;

    // can we do random access to lines?
    TIFFGetField(tif, TIFFTAG_COMPRESSION,  &compression_type);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    if ((compression_type==COMPRESSION_NONE)||(rows_per_strip==1))
      can_skip_lines = true;

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
      throw Err() << "TIFF error: unsupported photometric type: " << photometric;

  }

  void goto_line(const int n){
    if (line<0) prepare_reading();
    if (n>=h) throw Err() << "JPEG: too large line number";
    if (n<line) throw Err() << "JPEG: can't go back in image lines";
    if (can_skip_lines){
      line = n;
      TIFFReadScanline(tif, cbuf, line);
    }
    else {
      for (;line<n; ++line)
        TIFFReadScanline(tif, cbuf, line+1);
    }

  }

  /// Get raw data line
  unsigned char* get_data() const {return cbuf;}

  /// Get color value (at x coordinate of the current line)
  /// (no range checking!)
  unsigned int get_col(const int x) const {

    if (photometric == PHOTOMETRIC_PALETTE)
       return colors[cbuf[x]];

    if (photometric == PHOTOMETRIC_RGB){
       if (bpp==3) // RGB
         return 0xFF000000 + (cbuf[3*x]<<16) + (cbuf[3*x+1]<<8) + cbuf[3*x+2];
       if (bpp==4) // RGBA
         return (cbuf[4*x]<<16) + (cbuf[4*x+1]<<8) + cbuf[4*x+2] + (cbuf[4*x+3]<<24);
       if (bpp==1) // G
         return cbuf[x] + (cbuf[x]<<8) + (cbuf[x]<<16) + (0xFF<<24);
    }
    if (photometric == PHOTOMETRIC_MINISWHITE){
       if (bpp==2) // 16bit
         return 0xFFFFFFFF - (cbuf[2*x+1] + (cbuf[2*x+1]<<8) + (cbuf[2*x+1]<<16));
       if (bpp==1) // 8bit
         return 0xFFFFFFFF - (cbuf[x] + (cbuf[x]<<8) + (cbuf[x]<<16));
    }
    if (photometric == PHOTOMETRIC_MINISBLACK){
      if (bpp==2) // 16bit
         return cbuf[2*x+1] + (cbuf[2*x+1]<<8) + (cbuf[2*x+1]<<16) + (0xFF<<24);
      if (bpp==1) // 8bit
         return cbuf[x] + (cbuf[x]<<8) + (cbuf[x]<<16) + (0xFF<<24);
    }
    throw Err() << "TIFF error: data format: photometric: "
                << photometric << " bpp: " << bpp;
  }

};

// getting file dimensions
iPoint image_size_tiff(const std::string & file){
  ImageSourceTIFF SRC(file);
  return SRC.size();
}

Image
image_load_tiff(const std::string & file, const int scale){

  ImageSourceTIFF SRC(file);
  iPoint size = SRC.size();
  Image img(size.x, size.y, IMAGE_32ARGB);

  for (int y=0; y<size.y; ++y){
    SRC.goto_line(y);
    for (int x=0; x<size.x; ++x) img.set32(x,y, SRC.get_col(x));
  }
  return img;
}


/*

// save part of image
void image_save_tiff(const Image & im, const std::string & file){

  if (im.type() != IMAGE_32ARGB)
    throw Err() << "GIF error: only 32-bpp images are supported";

  TIFF* tif = TIFFOpen(file, "wb");

    if (!tif){
      cerr << "image_tiff: can't write " << file << endl;
      return 1;
    }

    // scan image for colors and alpha
    bool alpha = false;
    bool fulla = false;
    bool fullc = false;
    bool color = false;
    uint32 colors[256], mc=0;
    memset(colors, 0, 256*sizeof(int));
    for (int y = src_rect.y; y < src_rect.y+src_rect.h; y++){
      if ((y<0)||(y>=im.h)) continue;
      for (int x = 0; x < src_rect.w; x++){
        if ((x+src_rect.x < 0) || (x+src_rect.x>=im.w)) continue;
        unsigned int c = im.get(x+src_rect.x, y);

        if (!alpha){
          int a = (c >> 24) & 0xFF;
          if (a<255) alpha=true;
        }

        if (!fulla){
          int a = (c >> 24) & 0xFF;
          if (a>0 && a<255) fulla=true;
        }

        if (!color){
          int r = (c >> 16) & 0xFF;
          int g = (c >> 8) & 0xFF;
          int b = c & 0xFF;
          if (r!=g || r!=b) color=true;
        }

        if (!fullc){
          bool found=false;
          for (int i=0; i<mc; i++)
            if (c==colors[i]){ found=true; break;}
          if (!found){
            if (mc==256) fullc=true;
            else colors[mc++] = c;
          }
        }
      }
    }

    int bpp = 3;
    if (!color || !fullc) {bpp=1;}
    if (alpha)  {bpp=4; fullc=true;}
    int scan = bpp*src_rect.w;

    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, src_rect.w);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, src_rect.h);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, bpp);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE,   8);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG,    1);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP,    1);
    TIFFSetField(tif, TIFFTAG_COMPRESSION,     COMPRESSION_LZW);

    uint16 cmap[3][256];
    if (fullc){
      TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    }
    else{
      if (color){
        for (int i=0; i<256; i++){
          cmap[0][i] = (colors[i]<<8)&0xFF00;
          cmap[1][i] =  colors[i]    &0xFF00;
          cmap[2][i] = (colors[i]>>8)&0xFF00;
        }
      }
      else{
        for (uint16 i=0; i<256; i++){
          cmap[0][i] = cmap[1][i] = cmap[2][i] = i<<8;
          colors[i] = i;
        }
      }
      TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_PALETTE);
      TIFFSetField(tif, TIFFTAG_COLORMAP, cmap, cmap+1, cmap+2);
    }

    if (bpp==4){
      int type=EXTRASAMPLE_UNASSALPHA;
      TIFFSetField(tif, TIFFTAG_EXTRASAMPLES,  1, &type);
    }

    tdata_t buf = _TIFFmalloc(scan);
    uint8 *cbuf = (uint8 *)buf;

    for (int row = 0; row < src_rect.h; row++){
      if ((row+src_rect.y <0)||(row+src_rect.y>=im.h)){
	for (int col = 0; col < src_rect.w*bpp; col++) cbuf[col] = 0;
      } else {
        for (int col = 0; col < src_rect.w; col++){
	  int c = 0;
          if ((col+src_rect.x >=0)&&(col+src_rect.x<im.w))
             c = im.get(src_rect.x+col,src_rect.y+row);
	  if (bpp==3){ // RGB
    	      cbuf[3*col]   = c & 0xFF;
    	      cbuf[3*col+1] = (c >> 8)  & 0xFF;
    	      cbuf[3*col+2] = (c >> 16) & 0xFF;
          }
	  else if (bpp==4){ // RGBA
    	      cbuf[4*col]   = c & 0xFF;
    	      cbuf[4*col+1] = (c >> 8)  & 0xFF;
    	      cbuf[4*col+2] = (c >> 16) & 0xFF;
    	      cbuf[4*col+3] = (c >> 24) & 0xFF;
          }
          else if (bpp==1){
            if (color){
              for (int i=0; i<mc; i++)
                if (colors[i] == c) {cbuf[col] = (unsigned char)i; break;}
            }
            else{
              cbuf[col] = c;
            }
          }
        }
      }
      TIFFWriteScanline(tif, buf, row);
    }
    _TIFFfree(buf);
    TIFFClose(tif);
    return 0;
}

Image load(const std::string & file, const int scale){
  iPoint s = size(file);
  Image ret(s.x/scale,s.y/scale);
  if (s.x*s.y==0) return ret;
  load(file, iRect(0,0,s.x,s.y), ret, iRect(0,0,s.x/scale,s.y/scale));
  return ret;
}

// save the whole image
int save(const Image & im, const std::string & file){
  return save(im, im.range(), file);
}
*/

