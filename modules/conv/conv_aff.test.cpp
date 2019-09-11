///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "conv_aff.h"

int
main(){
  try{

    // reference points
    dPoint ps1(0,0), ps2(1,0), ps3(0,1);
    dPoint pc(0.11, 0.12);

    // rotate clockwise by a
    double a=30*M_PI/180;
    dPoint pd1 = rotate2d(ps1, pc, a);
    dPoint pd2 = rotate2d(ps2, pc, a);
    dPoint pd3 = rotate2d(ps3, pc, a);

    std::map<dPoint,dPoint> ref;
    ref[ps1]=pd1;
    ref[ps2]=pd2;
    ref[ps3]=pd3;
    ConvAff2D cnv1(ref);

    dPoint p;
    p=ps1; cnv1.frw(p);
    assert(dist2d(p,pd1) < 1e-8);

    p=dPoint(2,8); cnv1.bck(p);
    assert(dist2d(p,rotate2d(dPoint(2,8), pc, -a)) < 1e-8);
    p=dPoint(2,8); cnv1.frw(p);
    assert(dist2d(p,rotate2d(dPoint(2,8), pc,  a)) < 1e-8);

    // convert angles  a -> a-30deg
    {
      assert(abs(cnv1.frw_ang(dPoint(1,1), 0, 1) + a) <1e-6 );
      assert(abs(cnv1.bck_ang(dPoint(1,1), 0, 1) - a) <1e-6 );
      assert(abs(cnv1.frw_ang(dPoint(1,1), -a, 1) + 2*a) <1e-6 );
      assert(abs(cnv1.bck_ang(dPoint(1,1), +a, 1) - 2*a) <1e-6 );

      assert(abs(cnv1.frw_angd(dPoint(1,1), 0, 1) + 30) <1e-6 );
      assert(abs(cnv1.bck_angd(dPoint(1,1), 0, 1) - 30) <1e-6 );
      assert(abs(cnv1.frw_angd(dPoint(1,1), -30, 1) + 60) <1e-6 );
      assert(abs(cnv1.bck_angd(dPoint(1,1), +30, 1) - 60) <1e-6 );

    }

    // rescale_src, rescale_dst, shift
    {
      // rescale(k)
      dPoint p, pr;
      cnv1.rescale_src(1.234);
      p = dPoint(2,8); cnv1.frw(p); pr = rotate2d(dPoint(2,8)*1.234, pc,  a); assert(dist2d(p,pr) < 1e-8);
      p = dPoint(2,8); cnv1.bck(p); pr = rotate2d(dPoint(2,8), pc, -a)/1.234; assert(dist2d(p,pr) < 1e-8);

      cnv1.rescale_dst(2.345);
      p = dPoint(2,8); cnv1.frw(p); pr = rotate2d(dPoint(2,8)*1.234, pc,  a)*2.345; assert(dist2d(p,pr) < 1e-8);
      p = dPoint(2,8); cnv1.bck(p); pr = rotate2d(dPoint(2,8)/2.345, pc, -a)/1.234; assert(dist2d(p,pr) < 1e-8);

      // rescale(kx,ky)
      cnv1.reset(ref);  // reset
      cnv1.rescale_src(1.234,2.345);
      p = dPoint(2,8); cnv1.frw(p); pr = rotate2d(dPoint(2*1.234,8*2.345), pc,  a); assert(dist2d(p,pr) < 1e-8);
      p = dPoint(2,8); cnv1.bck(p); pr = rotate2d(dPoint(2,8), pc, -a); pr=dPoint(pr.x/1.234,pr.y/2.345);
        assert(dist2d(p,pr) < 1e-8);

      cnv1.reset(ref);
      cnv1.rescale_dst(1.234,2.345);
      p = dPoint(2,8); cnv1.frw(p); pr = rotate2d(dPoint(2,8), pc, a); pr=dPoint(pr.x*1.234,pr.y*2.345);
        assert(dist2d(p,pr) < 1e-8);
      p = dPoint(2,8); cnv1.bck(p); pr = rotate2d(dPoint(2/1.234,8/2.345), pc, -a); assert(dist2d(p,pr) < 1e-8);

      // shift
      cnv1.reset(ref);
      dPoint sh(0.543,0.432);
      cnv1.shift_src(sh);
      p = dPoint(2,8); cnv1.frw(p); pr = rotate2d(dPoint(2,8)+sh, pc,  a); assert(dist2d(p,pr) < 1e-8);
      p = dPoint(2,8); cnv1.bck(p); pr = rotate2d(dPoint(2,8), pc, -a)-sh; assert(dist2d(p,pr) < 1e-8);

      cnv1.reset(ref);
      cnv1.shift_dst(sh);
      p = dPoint(2,8); cnv1.frw(p); pr = rotate2d(dPoint(2,8), pc,  a)+sh; assert(dist2d(p,pr) < 1e-8);
      p = dPoint(2,8); cnv1.bck(p); pr = rotate2d(dPoint(2,8)-sh, pc, -a); assert(dist2d(p,pr) < 1e-8);

      // reset()
      cnv1.reset();
      p = dPoint(2,8); cnv1.frw(p); assert(p == dPoint(2,8));
      p = dPoint(2,8); cnv1.bck(p); assert(p == dPoint(2,8));

    }



    // can't build conversion from two points:
    try {
      ref.erase(ps1);
      ConvAff2D cnv2(ref);
    }
    catch(Err e) {
      assert(e.str() == "ConvAff2D: can't calculate conversion matrix.");
    }


  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond