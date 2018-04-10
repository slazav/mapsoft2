///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "conv_triv.h"

main(){
  try{

    ConvTriv cnv;
    dPoint p(10,10);
    cnv.frw(p);  assert(p == dPoint(10,10));
    cnv.bck(p);  assert(p == dPoint(10,10));

    cnv.rescale_src(3);
    cnv.frw(p); assert(p == dPoint(30,30));
    cnv.bck(p); assert(p == dPoint(10,10));

    cnv.rescale_dst(3);
    cnv.frw(p);  assert(p == dPoint(90,90));
    cnv.bck(p);  assert(p == dPoint(10,10));

    cnv.rescale_dst(1/9.0);
    cnv.frw(p);  assert(p == dPoint(10,10));
    cnv.bck(p);  assert(p == dPoint(10,10));

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond