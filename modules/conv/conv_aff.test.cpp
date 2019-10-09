///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "conv_aff.h"
#include "err/assert_err.h"

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
    // for 3 points conversion error is 0
    assert(cnv1.get_src_err() < 1e-15);
    assert(cnv1.get_dst_err() < 1e-15);

    dPoint p;
    p=ps1; cnv1.frw(p);
    assert(dist2d(p,pd1) < 1e-8);

    p=dPoint(2,8); cnv1.bck(p);
    assert(dist2d(p,rotate2d(dPoint(2,8), pc, -a)) < 1e-8);
    p=dPoint(2,8); cnv1.frw(p);
    assert(dist2d(p,rotate2d(dPoint(2,8), pc,  a)) < 1e-8);

    // convert angles  a -> a-30deg
    {
      assert(fabs(cnv1.frw_ang(dPoint(1,1), 0, 1) + a) <1e-6 );
      assert(fabs(cnv1.bck_ang(dPoint(1,1), 0, 1) - a) <1e-6 );
      assert(fabs(cnv1.frw_ang(dPoint(1,1), -a, 1) + 2*a) <1e-6 );
      assert(fabs(cnv1.bck_ang(dPoint(1,1), +a, 1) - 2*a) <1e-6 );

      assert(fabs(cnv1.frw_angd(dPoint(1,1), 0, 1) + 30) <1e-6 );
      assert(fabs(cnv1.bck_angd(dPoint(1,1), 0, 1) - 30) <1e-6 );
      assert(fabs(cnv1.frw_angd(dPoint(1,1), -30, 1) + 60) <1e-6 );
      assert(fabs(cnv1.bck_angd(dPoint(1,1), +30, 1) - 60) <1e-6 );

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
      p = dPoint(2,8); cnv1.frw(p); assert_eq(p, dPoint(2,8));
      p = dPoint(2,8); cnv1.bck(p); assert_eq(p, dPoint(2,8));

    }

    // can't build conversion from two points:
    try {
      ref.erase(ps1);
      ConvAff2D cnv2(ref);
    }
    catch(Err e) {
      assert_eq(e.str(), "ConvAff2D: can't calculate conversion matrix.");
    }

    // error test
    {
      // make NxN point array in 1x1 square
      // conversion: rotation by `ang` around `cnt`, scaling by `sc`.
      int N = 10;
      dLine pts1, pts2;
      std::map<dPoint, dPoint> ptmap;

      double ang=30*M_PI/180;
      double sc=5.6;
      dPoint cnt(0.2,0.2);

      dPoint dp(0.1,0);
      for (int i=0; i<N; ++i) for (int j=0; j<N; ++j){
         dPoint p1(1.0*i/N, 1.0*j/N);
         dPoint p2 = rotate2d(p1, cnt, ang) * sc;
         pts1.push_back(p1);
         pts2.push_back(p2);

         // modify first point:
         if (i==0 && j==0) p1+=dp;
         ptmap.emplace(p1,p2);
      }

      ConvAff2D cnv(ptmap);
      // src error is roughly len2d(dp)/N (less then it)
      double e1 = len2d(dp)/N;
      assert(cnv.get_src_err() < e1);
      assert(e1 - cnv.get_src_err() < 0.1*e1);

      // src error is sc times more
      double e2 = e1*sc;
      assert(cnv.get_dst_err() < e2);
      assert(e2 - cnv.get_dst_err() < 0.1*e2);

      e1 = cnv.get_src_err();
      e2 = cnv.get_dst_err();

      cnv.rescale_src(2);
      assert(fabs(cnv.get_src_err() - e1/2) < 1e-10);
      assert(fabs(cnv.get_dst_err() - e2) < 1e-10);

      cnv.rescale_dst(3);
      assert(fabs(cnv.get_src_err() - e1/2) < 1e-10);
      assert(fabs(cnv.get_dst_err() - e2*3) < 1e-10);

      // just in case check conversion
      dPoint p1(0.345,0.443);
      dPoint p2(p1);
      cnv.frw(p2);
      dPoint p2c = rotate2d(p1*2, cnt, ang)*sc*3;
      assert(dist2d(p2,p2c) < 0.05);

    }


  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond