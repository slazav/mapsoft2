///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "conv_base.h"
#include "conv_multi.h"
#include "err/assert_err.h"

int
main(){
  try{

    std::shared_ptr<ConvBase> cnv1(new ConvBase());
    std::shared_ptr<ConvBase> cnv2(new ConvBase());
    std::shared_ptr<ConvBase> cnv3(new ConvBase());

    ConvMulti cnv(cnv1, cnv2, true, false);
    cnv.push_front(cnv3, true);

    // cnv3(frw) -> cnv1(frw) -> cnv2(bck)
    cnv3->rescale_src(2);
    cnv1->rescale_src(10);
    cnv2->rescale_src(10);
    cnv.rescale_src(2);
    cnv.rescale_dst(0.5);
    // 2*(2*10/10)*0.5

    dPoint p(10,10);
    cnv.frw(p);  assert_deq(p, dPoint(20,20), 1e-6);
    cnv.bck(p);  assert_deq(p, dPoint(10,10), 1e-6);

    cnv.rescale_src(3);
    cnv.frw(p); assert_deq(p, dPoint(60,60), 1e-6);
    cnv.bck(p); assert_deq(p, dPoint(10,10), 1e-6);

    cnv.rescale_dst(3);
    cnv.frw(p);  assert_deq(p, dPoint(180,180), 1e-6);
    cnv.bck(p);  assert_deq(p, dPoint(10,10), 1e-6);

    cnv.rescale_dst(1/9.0);
    cnv.frw(p);  assert_deq(p, dPoint(20,20), 1e-6);
    cnv.bck(p);  assert_deq(p, dPoint(10,10), 1e-6);

   //some test with non-trivial conversion is needed

   // simplify()

    assert_eq(cnv.size(), 3);

    assert_eq(cnv.simplify(dRect(0,0,10,10), 5), true);
    cnv.frw(p);  assert_deq(p, dPoint(20,20), 1e-6);
    cnv.bck(p);  assert_deq(p, dPoint(10,10), 1e-6);

    assert_eq(cnv.size(), 1);
    cnv.reset();
    assert_eq(cnv.size(), 0);
    assert_eq(cnv.simplify(dRect(0,0,10,10), 5), false);
    assert_eq(cnv.size(), 0);

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond