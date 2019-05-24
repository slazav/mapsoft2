///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "conv_geo.h"

int
main(){
  try{
    std::string proj_wgs = "+datum=WGS84 +proj=lonlat";
    std::string proj_krass = "+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=500000 +lon_0=27";

    ConvGeo cnv1(proj_wgs,proj_wgs);    // wgs -> wgs
    ConvGeo cnv2(proj_krass);           // krass -> wgs
    ConvGeo cnv3(proj_wgs, proj_krass); // wgs -> krass

    dPoint p1(25.651054, 60.976941, 0);
    dPoint p1a(427091, 6763808, -11);

    // trivial
    dPoint p2(p1);
    cnv1.frw(p2);
    assert(p1==p2);
    cnv1.bck(p2);
    assert(p1==p2);

    cnv2.bck(p2);
    assert(iPoint(p2) == p1a);
    cnv2.frw(p2);
    assert(dist(p1,p2) < 2e-7);

    cnv3.frw(p2);
    assert(iPoint(p2) == p1a);
    cnv3.bck(p2);
    assert(dist(p1,p2) < 2e-7);

    // line
    dLine l1;
    iLine l1a;
    for (int i=0; i<3; i++) {l1.push_back(p1); l1a.push_back(p1a);}
    cnv3.frw(l1);
    assert(iLine(l1) == l1a);

    // z==nan -- no altitude conversions
    p1 = dPoint(25.651054, 60.976941, nan(""));
    p1a = dPoint (427091.023, 6763808.07, nan(""));
    p2=p1;
    cnv2.bck(p2);
    assert(dist2d(p2,p1a) < 1e-2);
    cnv2.frw(p2);
    assert(dist2d(p1,p2) < 2e-7);

    // rescale_dst (only x,y is affected!)
    cnv3.rescale_dst(2);
    p2=p1;
    cnv3.frw(p2);
    p2.z*=2;
    assert(dist2d(p2,p1a*2) < 1e-2);
    p2.z/=2;
    cnv3.bck(p2);
    assert(dist2d(p1,p2) < 2e-7);

    // rescale_src
    cnv3.rescale_src(2);
    cnv3.rescale_dst(0.5);
    p2=p1/2; p2.z *= 2;
    cnv3.frw(p2);
    assert(dist2d(p2,p1a) < 1e-2);
    cnv3.bck(p2);
    p2.z/=2;
    assert(dist2d(p1/2,p2) < 2e-7);

    // no datum
    {
      std::string proj_ll = "+ellps=krass +proj=lonlat";
      std::string proj_tmerc = "+ellps=krass +proj=tmerc +x_0=500000 +lon_0=27";

      ConvGeo cnv1(proj_ll, proj_tmerc);
      dPoint p1(25.651054, 60.976941, nan("")), p2(p1);
      dPoint p1a(426961.39, 6763794.09, nan(""));
      cnv1.frw(p2);
      assert(dist2d(p1a,p2) <1e-2);
      cnv1.bck(p2);
      assert(dist2d(p1,p2) < 1e-7);
    }

    // no datum, no ellipsoid
    {
      std::string proj_ll = "+proj=lonlat";
      std::string proj_tmerc = "+proj=tmerc +x_0=500000 +lon_0=27";

      ConvGeo cnv1(proj_ll, proj_tmerc);
      dPoint p1(25.651054, 60.976941, nan("")), p2(p1);
      dPoint p1a(426962.60, 6763675.97, nan(""));
      cnv1.frw(p2);
      assert(dist2d(p1a,p2) <1e-2);
      cnv1.bck(p2);
      assert(dist2d(p1,p2) < 1e-7);
    }

    // CnvMap
    {
       GeoMap m;
       // ref: image points -> wgs84
       m.add_ref(159.0,386.0,   35.998051,55.999946);
       m.add_ref(2370.0,386.0,  36.248054,55.999950);
       m.add_ref(2371.0,3007.0, 36.248063,55.833280);
       m.add_ref(151.0,3010.0,  35.998059,55.833276);
       m.border = dLine("[[159.1,386.8],[1264.1,386.4],[2369.9,385.3],"
                        "[2371.2,3007.6],[1260.7,3008.9],[150.9,3009.3],[159.1,386.8]]");
       m.proj = "+datum=WGS84 +proj=tmerc +lon_0=39 +x_0=500000";
       ConvMap cnv1(m, "+ellps=krass +towgs84=28,-130,-95 +x_0=500000 +proj=tmerc +lon_0=39");
       dPoint p1(1333, 867);
       dPoint p2(321000, 6209000);
       cnv1.frw(p1);
       assert(dist2d(p1,p2) < 15); // 15m accuracy (~2px)

       ConvMap cnv2(m, "+ellps=krass +towgs84=28,-130,-95 +proj=latlong");
       dPoint p3(159,386);
       dPoint p4(36.00,56.00);
       cnv2.bck(p4);
       assert(dist2d(p3,p4) < 1); // 1px accuracy

       assert(m.bbox2d_ref_img() == dRect("[151,386,2220,2624]"));

       dRect r = m.bbox2d_ref_wgs();
       assert(dist2d(r.tlc(), dPoint(35.998051,55.833276)) < 1e-6);
       assert(abs(r.w - 1/4.0) < 1e-4);
       assert(abs(r.h - 1/6.0) < 1e-4);
    }

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
