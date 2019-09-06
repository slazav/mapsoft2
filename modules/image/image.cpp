#include "image.h"

// distance between two colors
double
color_dist(const uint32_t c1, const uint32_t c2){
  return sqrt(
    pow( (int)((c1>>24)&0xFF)-(int)((c2>>24)&0xFF), 2) +
    pow( (int)((c1>>16)&0xFF)-(int)((c2>>16)&0xFF), 2) +
    pow( (int)((c1>> 8)&0xFF)-(int)((c2>>8)&0xFF), 2) +
    pow( (int)(c1&0xFF) - (int)(c2&0xFF), 2));
}

// Assemble 32-bit color from a,r,g,b components.
// Prescaled semi-transparent colors are used
uint32_t color_argb(const uint8_t a, const uint8_t r,
                    const uint8_t g, const uint8_t b){
  if (a==0) return 0;
  if (a==0xFF) return 0xFF000000 + ((uint32_t)r<<16) + ((uint32_t)g<<8) + b;
  return ((uint32_t)a<<24) + ((uint32_t)r*a/256<<16) +
         ((uint32_t)g*a/256<<8) + ((uint32_t)b*a/256);
}

// remove transparency (for scaled colors)
uint32_t color_rem_transp(const uint32_t c, const bool gifmode){
  int a = (c>>24)&0xFF;
  if (a==0) return gifmode? 0 : c|0xFFFFFFFF;
  if (a==255) return c;

  int r = (c>>16)&0xFF;
  int g = (c>> 8)&0xFF;
  int b = c&0xFF;
  if (a>0 && a<0xFF){
    r = (r*256)/a;
    g = (g*256)/a;
    b = (b*256)/a;
    if (r>0xFF) r=0xFF;
    if (g>0xFF) g=0xFF;
    if (b>0xFF) b=0xFF;
  }
  return (0xFF<<24)+(r<<16)+(g<<8)+b;
}

// Convert RGB color to 8-bit greyscale
uint8_t color_rgb_to_grey8(const uint32_t c){
  return rint(
    COLOR_LUMINR*((c>>16)&0xFF) +
    COLOR_LUMING*((c>>8)&0xFF) +
    COLOR_LUMINB*(c&0xFF) );
}

// Convert RGB color to 16-bit greyscale
uint16_t color_rgb_to_grey16(const uint32_t c){
  return rint(
    COLOR_LUMINR*((c>>8)&0xFF00) +
    COLOR_LUMING*(c&0xFF00) +
    COLOR_LUMINB*((c<<8)&0xFF00) );
}


// image << operator
std::ostream & operator<< (std::ostream & s, const Image & i) {
  if (i.width()==0 || i.height()==0) {
    s << "Image(empty)";
  }
  else {
    s << "Image(" << i.width() << "x" << i.height() << ", ";

    switch (i.type()){
      case IMAGE_32ARGB:  s << "ARGB, 32bpp"; break;
      case IMAGE_24RGB:   s << "RGB, 24bpp"; break;
      case IMAGE_16:      s << "Grey, 16bpp"; break;
      case IMAGE_8:       s << "Grey, 8bpp"; break;
      case IMAGE_8PAL:    s << "Palette, 8bpp"; break;
      case IMAGE_1:       s << "B/W, 1bpp"; break;
      case IMAGE_UNKNOWN: s << "Unknown data format"; break;
    }
    s << ")";
  }
  return s;
}

