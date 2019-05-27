#include "image.h"
std::ostream & operator<< (std::ostream & s, const Image & i) {
  if (i.width()==0 || i.height()==0 || i.bpp()==0)
    s << "Image(empty)";
  else
    s << "Image(" << i.width() << "x" << i.height() << "x" << i.bpp() << "bpp)";
  return s;
}

