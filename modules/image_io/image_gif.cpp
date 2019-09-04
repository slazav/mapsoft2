#include <cstring>
#include <gif_lib.h>
#include "err/err.h"
#include "image/image_colors.h"
#include "image_gif.h"
#include "imgsrc.h"

/**********************************************************/
// Version-specific GifOpen/GifClose wrappers

#if defined(GIFLIB_MAJOR) && defined(GIFLIB_MINOR)
#if GIFLIB_MAJOR == 4 && GIFLIB_MINOR >= 2
#define GIFV 420
#endif
#if GIFLIB_MAJOR >= 5
#define GIFV 500
#endif
#endif
#ifndef GIFV
#define GIFV 0
#endif


// GIFLIB_MAJOR and GIFLIB_MINOR defined in 4.1.6 and later
// GifErrorString(code)
#if GIFV == 500
  GifFileType* GifOpen(const char *file){
    int code;
    GifFileType *gif = DGifOpenFileName(file, &code);
    if (!gif) throw Err() << "GIF error: can't open file: " << file;
    return gif;
  }
  void GifClose(GifFileType *gif){
    int code;
    DGifCloseFile(gif, &code);
    // do not throw error because GifClose will be use in destructors
  }
  void GifErr(){
    //throw Err() << "libgif error: " << GifErrorString(code);
    throw Err() << "libgif error";
  }
#endif
// 4.2 <= v < 5
// GifErrorString()
#if GIFV == 420
  GifFileType* GifOpen(const char *file){
    GifFileType *gif = DGifOpenFileName(file);
    if (!gif) throw Err() << "GIF error: can't open file: " << file;
    return gif;
  }
  void GifClose(GifFileType *gif){
    DGifCloseFile(gif);
  }
  void GifErr(){
    throw Err() << "libgif error: " << GifErrorString();
  }
#endif

// old versions
// GifLastError()
#if GIFV == 0
  GifFileType* GifOpen(const char *file){
    GifFileType *gif = DGifOpenFileName(file);
    if (!gif) throw Err() << "GIF error: can't open file: " << file;
    return gif;
  }
  void GifClose(GifFileType *gif){
    DGifCloseFile(gif);
  }
  void GifErr(){
    throw Err() << "libgif error: " << GifLastError();
  }
#endif
/**********************************************************/


/**********************************************************/
/// ImageSource for gif files

#include "imgsrc.h"

class ImageSourceGIF : ImageSource{
private:
  GifFileType *gif; // gif handler
  GifByteType *GifLine;
  int w, h, dx, dy;
  unsigned int colors[256]; // color palette
  int line;

public:

  // Destructor
  ~ImageSourceGIF(){
    if (gif) GifClose(gif);
    if (GifLine) delete[] GifLine;
  }

  // Constructor: open file
  ImageSourceGIF(const std::string & fname):
       gif(NULL), GifLine(NULL), w(0),h(0),dx(0),dy(0),line(-1){

    // reset color palette
    memset(colors, 0, sizeof(colors));

    // open file and read screen descriptor
    gif = GifOpen(fname.c_str());

    // Go to the first image, skip all extensions
    GifRecordType RecordType;
    int ExtCode, GifLineLen;
    GifByteType *Extension;
    do {
      if (DGifGetRecordType(gif, &RecordType) == GIF_ERROR) GifErr();
      if (RecordType == TERMINATE_RECORD_TYPE) GifErr();
      if (RecordType ==  EXTENSION_RECORD_TYPE){
        if (DGifGetExtension(gif, &ExtCode, &Extension) == GIF_ERROR) GifErr();
        while (Extension != NULL){
          if (DGifGetExtensionNext(gif, &Extension) == GIF_ERROR) GifErr();
        }
      }
    }
    while (RecordType != IMAGE_DESC_RECORD_TYPE);

    // read image description
    if (DGifGetImageDesc(gif) == GIF_ERROR) GifErr();

    //width = gif->SWidth;
    //height = gif->SHeight;

    w  = gif->Image.Width;
    h  = gif->Image.Height;
    dx = gif->Image.Left;
    dy = gif->Image.Top;

    if (w != gif->SWidth ||
        h != gif->SHeight) throw Err() 
          << "gif: image size differs from gif page size: "
          << w << "x" << h << " vs. "
          << gif->Image.Width << "x" << gif->Image.Height << "\n";

    for (int i=0; i<gif->SColorMap->ColorCount; i++){
      colors[i] = gif->SColorMap->Colors[i].Blue +
                  (gif->SColorMap->Colors[i].Green << 8) +
                  (gif->SColorMap->Colors[i].Red << 16) + (0xFF<<24);
      //std::cerr << "PALETTE: " << i << ": " << std::hex << colors[i] << "\n";
    }
    int bgcolor = colors[gif->SBackGroundColor];
    //std::cerr << "BGCOLOR: " << gif->SBackGroundColor << ": " <<  bgcolor << "\n";

    // get transparent color (LIBGIF>5.0)
    /*
    struct GraphicsControlBlock cblock;
    int image_index=0;
    if (DGifSavedExtensionToGCB(gif, image_index, cblock) != GIF_ERROR){
      if (cblock->TransparentColor >=0){
        colors[cblock->TransparentColor] = 0x00000000;
      }
    }
    */

    // allocate memory for one data line
    GifLine = new GifByteType[w];
    if (!GifLine) throw Err() << "gif: can't allocate memory";
  }

  /// Image range
  iPoint size() const {return iPoint(w,h);};

  /// Get bits per pixed for data
  int get_bpp() const  {return 8;}

  /// Get line number
  int get_line() const  {return line;};

  /// Goto line n (n should be >= get_line())
  void goto_line(const int n){
    if (n>=h)   throw Err() << "gif: too large line number";
    if (n<line) throw Err() << "gif: can't go back in image lines";
    for (;line<n; ++line)
      if (DGifGetLine(gif, GifLine, w) == GIF_ERROR) GifErr();
  }

  /// Get raw data line
  unsigned char* get_data() const {return GifLine;}

  /// Get color value (at x coordinate of the current line)
  /// (no range checking!)
  unsigned int get_col(const int x) const {
     return colors[GifLine[x]]; }

};

iPoint
image_size_gif(const std::string & file){
    GifFileType *gif = GifOpen(file.c_str());
    iPoint ret(gif->SWidth, gif->SHeight);
    GifClose(gif);
    return ret;
}

Image
image_load_gif(const std::string & file, const int scale){

  ImageSourceGIF SRC(file);
  iPoint size = SRC.size();
  Image img(size.x, size.y, 32);

  for (int y=0; y<size.x; ++y){
    SRC.goto_line(y);
    for (int x=0; x<size.y; ++x) img.set(x,y, SRC.get_col(x));
  }
  return img;
}


void
image_save_gif(const Image & im, const std::string & file){

  // reduce nu,ber of colors to 256
  int clen = 256;
  uint32_t colors[clen];
  image_color_mkpal(im, colors, clen);
  Image im8 = image_color_reduce(im, colors, clen);
  GifFileType *gif = NULL;
  ColorMapObject *cmap = NULL;
#if GIFV == 500
  int code;
  gif = EGifOpenFileName(file.c_str(), false, &code);
#else
  gif = EGifOpenFileName(file.c_str(), false);
#endif
  if (!gif) throw Err() << "GIF error: can't open file: " << file;

  try {
//    EGifSetGifVersion(gif, true);

#if GIFV == 500
    cmap = GifMakeMapObject(clen, NULL);
#else
    cmap = MakeMapObject(clen, NULL);
#endif
    for (int i=0; i<clen; i++){
      cmap->Colors[i].Blue  = colors[i] & 0xFF;
      cmap->Colors[i].Green = (colors[i] >> 8) & 0xFF;
      cmap->Colors[i].Red   = (colors[i] >> 16) & 0xFF;
    }

    if (EGifPutScreenDesc(gif, im8.width(), im8.height(),
      clen, 0, cmap)==GIF_ERROR) GifErr();

    if (EGifPutImageDesc(gif, 0,0, im8.width(), im8.height(),
      false, cmap)==GIF_ERROR) GifErr();

    if (EGifPutLine(gif, im8.data(), im8.width()*im8.height()) ==GIF_ERROR)
      GifErr();

    throw Err();
  }
  catch(Err e){
    if (gif) EGifCloseFile(gif);
#if GIFV == 500
    if (cmap) GifFreeMapObject(cmap);
#else
    if (cmap) FreeMapObject(cmap);
#endif
    if (e.str()!="") throw e;
  }
}
