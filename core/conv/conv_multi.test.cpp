///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "conv_triv.h"
#include "conv_multi.h"

main(){
  try{

    ConvTriv cnv1, cnv2, cnv3;
    ConvMulti cnv(&cnv1,&cnv2, true, false);
    cnv.push_front(&cnv3, true);

    // cnv3(frw) -> cnv1(frw) -> cnv2(bck)
    cnv3.rescale_src(2);
    cnv1.rescale_src(10);
    cnv2.rescale_src(10);
    cnv.rescale_src(2);
    cnv.rescale_dst(0.5);
    // 2*(2*10/10)*0.5

    dPoint p(10,10);
    cnv.frw(p);  assert(p == dPoint(20,20));
    cnv.bck(p);  assert(p == dPoint(10,10));

    cnv.rescale_src(3);
    cnv.frw(p); assert(p == dPoint(60,60));
    cnv.bck(p); assert(p == dPoint(10,10));

    cnv.rescale_dst(3);
    cnv.frw(p);  assert(p == dPoint(180,180));
    cnv.bck(p);  assert(p == dPoint(10,10));

    cnv.rescale_dst(1/9.0);
    cnv.frw(p);  assert(p == dPoint(20,20));
    cnv.bck(p);  assert(p == dPoint(10,10));

   //some test with non-trivial conversion is needed

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond