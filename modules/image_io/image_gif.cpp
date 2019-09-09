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
  void GifErr(){
    //throw Err() << "libgif error: " << GifErrorString(code);
    throw Err() << "GIF error";
  }
#endif
// 4.2 <= v < 5
// GifErrorString()
#if GIFV == 420
  void GifErr(){
    throw Err() << "GIF error: " << GifErrorString();
  }
#endif

// old versions
// GifLastError()
#if GIFV == 0
  void GifErr(){
    throw Err() << "GIF error: " << GifLastError();
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
#if GIFV == 500
    int code
    if (gif) DGifCloseFile(gif, &code);
#else
    if (gif) DGifCloseFile(gif);
#endif
    if (GifLine) delete[] GifLine;
  }

  // Constructor: open file
  ImageSourceGIF(const std::string & file):
       gif(NULL), GifLine(NULL), w(0),h(0),dx(0),dy(0),line(-1){

    // reset color palette
    memset(colors, 0, sizeof(colors));

    // open file and read screen descriptor
#if GIFV == 500
    int code;
    gif = DGifOpenFileName(file.c_str(), &code);
#else
    gif = DGifOpenFileName(file.c_str());
#endif
    if (!gif) throw Err() << "GIF error: can't open file: " << file;

    // Go to the first image, skip all extensions
    GifRecordType RecordType;
    int ExtCode, GifLineLen;
    GifByteType *Extension;
    int trcol = -1; // transparent color index
    do {
      if (DGifGetRecordType(gif, &RecordType) == GIF_ERROR) GifErr();
      if (RecordType == TERMINATE_RECORD_TYPE) GifErr();
      if (RecordType ==  EXTENSION_RECORD_TYPE){
        if (DGifGetExtension(gif, &ExtCode, &Extension) == GIF_ERROR) GifErr();

        // Graphic control extension (5 bytes)
        // byte 0: 4 (data size),
        // byte 1: bits 1-3 - reserved, 4-6 - disposal method,
        //         7 - user input flag, 8 - transparent flag.
        // bytes 2-3:  delay time
        // byte 4: transparent color index
        if (ExtCode == 0xF9 && Extension[0]==4 && (Extension[1]&1))
          trcol = Extension[4];

        // Skip extension tail
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

    // transparent color
    if (trcol > 0 && trcol < 256) colors[trcol] = 0;

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
#if GIFV == 500
    int code;
    GifFileType *gif = DGifOpenFileName(file.c_str(), &code);
    if (!gif) throw Err() << "GIF error: can't open file: " << file;
    iPoint ret(gif->SWidth, gif->SHeight);
    if (gif) DGifCloseFile(gif, &code);
#else
    GifFileType *gif = DGifOpenFileName(file.c_str());
    if (!gif) throw Err() << "GIF error: can't open file: " << file;
    iPoint ret(gif->SWidth, gif->SHeight);
    if (gif) DGifCloseFile(gif);
#endif
    return ret;
}

Image
image_load_gif(const std::string & file, const int scale){

  ImageSourceGIF SRC(file);
  iPoint size = SRC.size();
  Image img(size.x, size.y, IMAGE_32ARGB);

  for (int y=0; y<size.y; ++y){
    SRC.goto_line(y);
    for (int x=0; x<size.x; ++x) img.set32(x,y, SRC.get_col(x));
  }
  return img;
}


void
image_save_gif(const Image & im, const std::string & file, const Opt & opt){

  GifFileType *gif = NULL;
  ColorMapObject *gif_cmap = NULL;
#if GIFV == 500
  int code;
  gif = EGifOpenFileName(file.c_str(), false, &code);
#else
  gif = EGifOpenFileName(file.c_str(), false);
#endif
  if (!gif) throw Err() << "GIF error: can't open file: " << file;

  Opt opt1(opt);
  if (opt1.get("cmap_alpha", "")!="none")
    opt1.put("cmap_alpha", "gif");
  std::vector<uint32_t> colors = image_colormap(im, opt1);
  Image im8 = image_remap(im, colors, opt1);

  if (im8.cmap.size()<2 || im8.cmap.size()>2562) throw Err() <<
    "image_save_gif: bad size of image colormap" << im8.cmap.size();

  // find fully transparent color
  int trcol = -1;
  for (int i=0; i<im8.cmap.size(); i++)
    if (((im8.cmap[i]>>24)&0xFF) == 0) {trcol = i; break;}

  // gif color map should have 2^x size.
  int cnum=2;
  for (int i = 8; i>0; i--){
    if (im8.cmap.size()<=pow(2,i)) cnum = pow(2,i);
    else break;
  }

  try {
//    EGifSetGifVersion(gif, true);
#if GIFV == 500
    gif_cmap = GifMakeMapObject(cnum, NULL);
#else
    gif_cmap = MakeMapObject(cnum, NULL);
#endif

    if (!gif_cmap) throw Err() <<
      "image_save_gif: can't initialize GIF color map";

    for (int i=0; i<im8.cmap.size(); ++i){
      gif_cmap->Colors[i].Blue  = im8.cmap[i] & 0xFF;
      gif_cmap->Colors[i].Green = (im8.cmap[i] >> 8) & 0xFF;
      gif_cmap->Colors[i].Red   = (im8.cmap[i] >> 16) & 0xFF;
    }

    if (EGifPutScreenDesc(gif, im8.width(), im8.height(),
      im8.cmap.size(), 0, gif_cmap)==GIF_ERROR) GifErr();

    // Graphic control extension (0xF9, 4 bytes)
    if (trcol>=0){
      char buf[4] = {1,0xA,0,(char)trcol};
      unsigned int ext = 0x01000000 + trcol;
      if (EGifPutExtension(gif, 0xF9, 4, &buf) == GIF_ERROR)
        GifErr();
    }

    if (EGifPutImageDesc(gif, 0,0, im8.width(), im8.height(),
      false, gif_cmap)==GIF_ERROR) GifErr();

    if (EGifPutLine(gif, im8.data(), im8.width()*im8.height()) ==GIF_ERROR)
      GifErr();

    throw Err();
  }
  catch(Err e){
#if GIFV == 500
    int code;
    if (gif) EGifCloseFile(gif, &code);
    if (cmap) GifFreeMapObject(cmap);
#else
    if (gif) EGifCloseFile(gif);
    if (gif_cmap) FreeMapObject(gif_cmap);
#endif
    if (e.str()!="") throw e;
  }
}
