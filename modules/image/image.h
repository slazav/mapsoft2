#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

#include <vector>
#include <memory>
#include <err/err.h>
#include <geom/rect.h>

/*
An image: 2d array of arbitrary data.
*/

// ImageDataType shows data type of the image.
// One shoukd not directly access data if type
// is IMAGE_UNKNOWN.
enum ImageDataType {
  IMAGE_32ARGB, // 4 bytes per pixel data: ARGB colors
  IMAGE_24RGB,  // 3 bytes per pixel data: RGB colors
  IMAGE_16,     // 2 bytes per pixel, gryscale
  IMAGE_8,      // 1 byte per pixel, gryscale
  IMAGE_8PAL,   // 1 byte per pixel + color palette
  IMAGE_1,      // 1 bit per pixel, black and white
  IMAGE_FLOAT,  // float-value pixel
  IMAGE_DOUBLE, // double-value pixel
  IMAGE_UNKNOWN,// unknown data format
};

/*********************************************************************/
// Luminance (from ITU-R BT.601.5)
#define COLOR_LUMINR (0.2989)
#define COLOR_LUMING (0.5866)
#define COLOR_LUMINB (0.1145)

/*********************************************************************/
// a few color-handling functions

// distance between two colors
double color_dist(const uint32_t c1, const uint32_t c2);

// Assemble 32-bit color from a,r,g,b components.
// Prescaled semi-transparent colors are used
uint32_t color_argb(const uint8_t a, const uint8_t r,
                    const uint8_t g, const uint8_t b);

// Remove transparency (with color scaling).
// if gifmode = true, then keep fully transparent colors.
uint32_t color_rem_transp(const uint32_t c, const bool gifmode);

// Convert RGB color to 8-bit greyscale
uint8_t color_rgb_to_grey8(const uint32_t c);

// Convert RGB color to 16-bit greyscale
uint16_t color_rgb_to_grey16(const uint32_t c);

/*********************************************************************/
// base class
class Image {
  private:
    std::shared_ptr<uint8_t> data_;
    size_t w,h; // width, height
    ImageDataType t;

  public:

    std::vector<uint32_t> cmap;

    ImageDataType type() const   {return t;}
    size_t width() const  {return w;}
    size_t height() const {return h;}
    iRect bbox() const    {return iRect(0,0,w,h);}
    iPoint size() const   {return iPoint(w,h);}
    unsigned char *data() const {return data_.get();}

    bool is_empty() const {return w==0 || h==0;}
    operator bool() const{ return w!=0 && h!=0; }

    // get data size
    size_t dsize() const {
      switch (t){
        case IMAGE_32ARGB: return w*h*4;
        case IMAGE_24RGB:  return w*h*3;
        case IMAGE_16:     return w*h*2;
        case IMAGE_8:      return w*h;
        case IMAGE_8PAL:   return w*h;
        case IMAGE_1:      return (w*h-1)/8+1;
        case IMAGE_FLOAT:  return (w*h)*sizeof(float);
        case IMAGE_DOUBLE: return (w*h)*sizeof(double);
        case IMAGE_UNKNOWN: return 0;
      }
      return 0;
    }

    // constructor -- create an empty image
    Image(): w(0), h(0), t(IMAGE_UNKNOWN) {}

    // constructor -- create WxH non-initialized image
    Image(const size_t W, const size_t H,
          const ImageDataType type): w(W), h(H), t(type){

      if (w<=0 || h<=0)
        throw Err() << "non-positive image dimension: " << w << "x" << h;

      try{
        data_ = std::shared_ptr<unsigned char>(new uint8_t[dsize()]);
      }
      catch (const std::bad_alloc & e) {
        throw Err() << "Image: can't allocate memory for "
                    << *this << ": " << e.what();
      }
    }


    /******************************************************/
    // Fast get functions for different image types.
    // Image type and coordinate range should be checked before.

    // Fast get function for image type IMAGE_32ARGB.
    uint32_t get32(const size_t x, const size_t y) const{
      return ((uint32_t*)data_.get())[w*y+x]; }

    // Fast get function for image type IMAGE_24RGB
    uint32_t get24(const size_t x, const size_t y) const{
      int i0 = 3*(w*y+x);
      return 0xFF000000 +
             (data_.get()[i0]<<16) +
             (data_.get()[i0+1]<<8) +
              data_.get()[i0+2];
    }

    // Fast get function for image type IMAGE_16
    uint16_t get16(const size_t x, const size_t y) const{
      return ((uint16_t*)data_.get())[w*y+x]; }

    // Fast color get function for image type IMAGE_16.
    // To convert 16-bit data into color the less sagnificant byte is skipped.
    uint32_t get16col(const size_t x, const size_t y) const{
      uint16_t c = (((uint16_t*)data_.get())[w*y+x] >> 8) & 0xFF;
      return 0xFF000000 + (c<<16) + (c<<8) + c;
    }

    // Fast get function for image types IMAGE_8 and IMAGE_8PAL
    uint8_t get8(const size_t x, const size_t y) const{
      return data_.get()[w*y+x]; }

    // Fast color get function for image type IMAGE_8
    // Image type and coordinate range should be checked before.
    uint32_t get8col(const size_t x, const size_t y) const{
      uint8_t c = data_.get()[w*y+x];
      return 0xFF000000 + (c<<16) + (c<<8) + c;
    }

    // Fast color get function for image type IMAGE_8PAL
    uint32_t get8pal(const size_t x, const size_t y) const{
      return cmap[data_.get()[w*y+x]];
    }

    // Fast get function for image type IMAGE_1.
    bool get1(const size_t x, const size_t y) const{
      size_t b = (w*y+x)/8; // byte
      size_t o = (w*y+x)%8; // offset
      uint8_t v = data_.get()[b];
      return (v >> o) & 1;
    }

    // Fast color get function for image type IMAGE_1.
    uint32_t get1col(const size_t x, const size_t y) const{
      return get1(x,y) ? 0xFFFFFFFF:0xFF000000;
    }

    // Fast get function for image type IMAGE_FLOAT
    float getF(const size_t x, const size_t y) const{
      return ((float *)data_.get())[w*y+x]; }

    // Fast get function for image type IMAGE_DOUBLE
    double getD(const size_t x, const size_t y) const{
      return ((double *)data_.get())[w*y+x]; }

    // Color get function for IMAGE_UNKNOWN.
    // To be redefined in non-standard image classes.
    virtual uint32_t getUcol(const size_t x, const size_t y) const{
      return 0;
    }

    /******************************************************/
    // universal get functions, should work for any image type

    // Get ARGB (prescaled) color for any image type.
    uint32_t get_argb(const size_t x, const size_t y) const{
      switch (t){
        case IMAGE_32ARGB: return get32(x,y);
        case IMAGE_24RGB:  return get24(x,y);
        case IMAGE_16:     return get16col(x,y);
        case IMAGE_8:      return get8col(x,y);
        case IMAGE_8PAL:   return get8pal(x,y);
        case IMAGE_1:      return get1col(x,y);
        case IMAGE_FLOAT:  return 0; // todo: rainbow calculation?
        case IMAGE_DOUBLE: return 0; // todo: rainbow calculation?
        case IMAGE_UNKNOWN: return getUcol(x,y);
      }
      return 0;
    }

    // Get RGB color for any image type.
    uint32_t get_rgb(const size_t x, const size_t y) const{
      if (t==IMAGE_24RGB) return get24(x,y);
      if (t==IMAGE_16)    return get16col(x,y);
      if (t==IMAGE_8)     return get8col(x,y);
      if (t==IMAGE_1)     return get1col(x,y);
      return color_rem_transp(get_argb(x,y),false);
    }

    // Get 8-bit grey color for any image type.
    uint8_t get_grey8(const size_t x, const size_t y) const{
      if (t==IMAGE_8)  return get8(x,y);
      if (t==IMAGE_16) return get16(x,y)>>8;
      return color_rgb_to_grey8(get_rgb(x,y));
    }

    // Get alpha channel + 8-bit grey color for any image type.
    uint16_t get_agrey8(const size_t x, const size_t y) const{
      if (t==IMAGE_8)  return 0xFF00 + get8(x,y);
      if (t==IMAGE_16) return 0xFF00 + (get16(x,y)>>8);
      uint32_t c = get_argb(x,y);
      return ((c>>16) & 0xFF00) +
             color_rgb_to_grey8(color_rem_transp(c,false));
    }

    // Get 16-bit grey color for any image type.
    uint16_t get_grey16(const size_t x, const size_t y) const{
      if (t==IMAGE_8)  return get8(x,y)<<8;
      if (t==IMAGE_16) return get16(x,y);
      return color_rgb_to_grey16(get_rgb(x,y));
    }

    /******************************************************/
    // Fast set functions for different image types.
    // Image type and coordinate range should be checked before.

    // Fast set function for image type IMAGE_32ARGB.
    void set32(const size_t x, const size_t y, const uint32_t v) const{
      ((uint32_t*)data_.get())[w*y+x] = v; }

    // Fast set function for image type IMAGE_24RGB
    void set24(const size_t x, const size_t y, const uint32_t v) const{
      data_.get()[3*(w*y+x)]   = (v>>16) & 0xFF;
      data_.get()[3*(w*y+x)+1] = (v>>8)  & 0xFF;
      data_.get()[3*(w*y+x)+2] = v & 0xFF;
    }

    // Fast set function for image type IMAGE_16
    void set16(const size_t x, const size_t y, const uint16_t v) const{
      ((uint16_t *)data_.get())[w*y+x] = v; }

    // Fast set function for image types IMAGE_8 and IMAGE_8PAL
    void set8(const size_t x, const size_t y, const uint8_t v) const{
      data_.get()[w*y+x] = v; }

    // Fast set function for image type IMAGE_1.
    void set1(const size_t x, const size_t y, const bool v) const{
      size_t b = (w*y+x)/8; // byte
      size_t o = (w*y+x)%8; // offset
      uint8_t old = data_.get()[b];
      data_.get()[b] = v? old|(1<<o) : old&~(1<<o);
    }

    // Fast set function for image type IMAGE_FLOAT
    void setF(const size_t x, const size_t y, const float v) const{
      ((float*)data_.get())[w*y+x] = v; }

    // Fast set function for image type IMAGE_DOUBLE
    void setD(const size_t x, const size_t y, const double v) const{
      ((double*)data_.get())[w*y+x] = v; }

    // Color set function for IMAGE_UNKNOWN.
    // To be redefined in non-standard image classes.
    virtual void setUcol(const size_t x, const size_t y) const{
    }

    /******************************************************/
    // Fill functions for different image types.
    // Image type should be checked before.

    // Fill function for image type IMAGE_32ARGB.
    void fill32(const uint32_t v) const{
      for (int i=0; i<w*h; i++)
        ((uint32_t*)data_.get())[i] = v;
    }

    // Fill function for image type IMAGE_24RGB
    void fill24(const uint32_t v) const{
      for (int i=0; i<w*h; i++) {
        data_.get()[3*i]   = (v>>16) & 0xFF;
        data_.get()[3*i+1] = (v>>8)  & 0xFF;
        data_.get()[3*i+2] = v & 0xFF;
      }
    }

    // Fill function for image type IMAGE_16
    void fill16(const uint16_t v) const{
      for (int i=0; i<w*h; i++)
        ((uint16_t *)data_.get())[i] = v;
    }

    // Fill function for image types IMAGE_8 and IMAGE_8PAL
    void fill8(const uint8_t v) const{
      for (int i=0; i<w*h; i++)
        data_.get()[i] = v;
    }

    // Fill function for image type IMAGE_1.
    void fill1(const bool v) const{
      for (int i=0; i<dsize(); i++)
        data_.get()[i] = v? 0xFF:0x00;
    }

    // Fill function for image type IMAGE_FLOAT
    void fillF(const float v) const{
      for (int i=0; i<w*h; i++)
        ((float*)data_.get())[i] = v;
    }


    // Fill function for image type IMAGE_DOUBLE
    void fillD(const double v) const{
      for (int i=0; i<w*h; i++)
        ((double*)data_.get())[i] = v;
    }

};


std::ostream & operator<< (std::ostream & s, const Image & i);

#endif
