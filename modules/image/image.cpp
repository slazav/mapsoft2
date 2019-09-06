#include "image.h"
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

