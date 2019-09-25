///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "err/assert_err.h"
#include "conv_geo.h"
#include "geo_utils.h"

int
main(){
  try{

    assert(expand_proj_aliases("WGS") ==
      "+datum=WGS84 +proj=lonlat");

    assert(expand_proj_aliases("WEB") ==
      "+proj=webmerc +datum=WGS84");

    assert(expand_proj_aliases("FI") ==
      "+proj=tmerc +lon0=27 +ellps=intl "
      "+towgs84=-90.7,-106.1,-119.2,4.09,0.218,-1.05,1.37");

    assert(expand_proj_aliases("CH") ==
      "+proj=somerc +lat_0=46.95240555555556 +lon_0=7.439583333333333 "
      "+x_0=600000 +y_0=200000 +ellps=bessel "
      "+towgs84=674.374,15.056,405.346,0,0,0,0 +units=m +no_defs");

    assert(expand_proj_aliases("SU-3") ==
      "+ellps=krass +towgs84=+28,-130,-95 "
      "+proj=tmerc +lon_0=-3 +x_0=60500000");

    assert(expand_proj_aliases("SU3") ==
      "+ellps=krass +towgs84=+28,-130,-95 "
      "+proj=tmerc +lon_0=3 +x_0=1500000");

    assert(expand_proj_aliases("SU0") ==
      "+ellps=krass +towgs84=+28,-130,-95 "
      "+proj=tmerc +lon_0=3 +x_0=1500000");


    std::string proj_wgs = "WGS";
    std::string proj_krass = "SU27";

    ConvGeo cnv1(proj_wgs,proj_wgs, false);   // wgs -> wgs, 3D
    ConvGeo cnv2(proj_krass);                 // krass -> wgs, 2D
    ConvGeo cnv3(proj_wgs, proj_krass, true); // wgs -> krass, 2D

    assert(cnv1.is_src_deg() == true);
    assert(cnv1.is_dst_deg() == true);
    assert(cnv2.is_src_deg() == false);
    assert(cnv2.is_dst_deg() == true);
    assert(cnv3.is_src_deg() == true);
    assert(cnv3.is_dst_deg() == false);

    assert(cnv1.get_2d() == false);
    assert(cnv2.get_2d() == true);
    assert(cnv3.get_2d() == true);

    cnv3.set_2d(false);
    assert(cnv3.get_2d() == false);
    cnv3.set_2d();
    assert(cnv3.get_2d() == true);

    dPoint p1(25.651054, 60.976941, 0);
    dPoint p1a(5427091, 6763808, -11);
    dPoint p1z(5427091, 6763808);
    dPoint p1n(5427091, 6763808, nan(""));

    // trivial
    dPoint p2(p1);
    cnv1.frw(p2);
    assert(p1==p2);
    cnv1.bck(p2);
    assert(p1==p2);


    // wgs -> pulkovo -> wgs
    cnv2.set_2d(false);
    cnv3.set_2d(false);
    cnv2.bck(p2);
    assert(iPoint(p2) == p1a);
    cnv2.frw(p2);
    assert(dist(p1,p2) < 2e-7);

    cnv3.frw(p2);
    assert(iPoint(p2) == p1a);
    cnv3.bck(p2);
    assert(dist(p1,p2) < 2e-7);

    // same, 2D
    cnv2.set_2d();
    cnv3.set_2d();
    p2 = p1;
    cnv2.bck(p2);
    assert(iPoint(p2) == p1z);
    cnv2.frw(p2);
    assert(dist(p1,p2) < 2e-7);

    cnv3.frw(p2);
    assert(iPoint(p2) == p1z);
    cnv3.bck(p2);
    assert(dist(p1,p2) < 2e-7);

    // nan altitude (2d conversion)
    cnv2.set_2d(false);
    cnv3.set_2d(false);
    p2 = p1;
    p2.z = nan("");
    cnv2.bck(p2);
    assert(dist2d(p2,p1n) < 1e-1);
    cnv2.frw(p2);
    assert(dist2d(p1,p2) < 2e-7);

    cnv3.frw(p2);
    assert(dist2d(p2,p1n) < 1e-1);
    cnv3.bck(p2);
    assert(dist2d(p1,p2) < 2e-7);

    // line
    dLine l1;
    iLine l1a;
    for (int i=0; i<3; i++) {l1.push_back(p1); l1a.push_back(p1a);}
    cnv3.frw(l1);
    assert(iLine(l1) == l1a);

    // rescale_dst (only x,y is affected!)
    cnv3.rescale_dst(2);
    p2=p1;
    cnv3.frw(p2);
    p2.z*=2;
    assert(dist2d(p2,p1a*2) < 1);
    p2.z/=2;
    cnv3.bck(p2);
    assert(dist2d(p1,p2) < 2e-7);

    // rescale_src
    cnv3.rescale_src(2);
    cnv3.rescale_dst(0.5);
    p2=p1/2; p2.z *= 2;
    cnv3.frw(p2);
    assert(dist2d(p2,p1a) < 1);
    cnv3.bck(p2);
    p2.z/=2;
    assert(dist2d(p1,p2*2) < 2e-7);

    // adding coordinate prefix does not change result
    {
      std::string proj1 = "SU99";
      std::string proj2 = "+ellps=krass +towgs84=+28,-130,-95 +proj=tmerc"
                          " +lon_0=99 +x_0=500000";
;

      ConvGeo cnv1(proj1);
      ConvGeo cnv2(proj2);

      dPoint p1(98.651054, 60.976941), p2(p1), p3(p1);
      cnv1.bck(p1);
      cnv2.bck(p2);
      p2.x += 17000000;
      assert(p1 == p2);
    }

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

    // lonlat = latlon !
    {
      ConvGeo cnv1("+proj=lonlat", "+proj=latlon");
      dPoint p1(25.651054, 60.976941);
      dPoint p2(p1);
      cnv1.frw(p2);
      assert(dist2d(p1,p2) < 1e-7);
      cnv1.bck(p2);
      assert(dist2d(p1,p2) < 1e-7);
    }

    // bad coordinates (without datum conversion)
    {
      std::string proj_ll = "+proj=lonlat";
      std::string proj_tmerc = "+proj=tmerc +x_0=500000 +lon_0=27";

      ConvGeo cnv1(proj_ll, proj_tmerc);
      dPoint p1(25.651054, 160.976941);
      assert_err(cnv1.frw(p1), "Can't convert coordinates: latitude or longitude exceeded limits");

      p1 = dPoint(426963,16763676);
      cnv1.bck(p1);
      assert(p1 == dPoint(27,90)); // strange PROJ feature

      p1 = dPoint(nan(""), 60.976941);
      assert_err(cnv1.frw(p1), "Can't convert coordinates: non-numeric result");
    }

    // bad coordinates (with datum conversion)
    {
      std::string proj_wgs = "WGS";
      std::string proj_krass = "SU27";

      ConvGeo cnv1(proj_wgs, proj_krass);
      dPoint p1(25.651054, 160.976941);
      assert_err(cnv1.frw(p1), "Can't convert coordinates: non-numeric result");

      p1 = dPoint(nan(""), 60.976941);
      assert_err(cnv1.frw(p1), "Can't convert coordinates: non-numeric result");

    }


    // CnvMap
    {
       GeoMap m;
       // ref: image points -> wgs84
       m.add_ref(159.0,386.0,   35.998051,55.999946);
       m.add_ref(2370.0,386.0,  36.248054,55.999950);
       m.add_ref(2371.0,3007.0, 36.248063,55.833280);
       m.add_ref(151.0,3010.0,  35.998059,55.833276);
       m.border = dMultiLine("[[[159.1,386.8],[1264.1,386.4],[2369.9,385.3],"
                        "[2371.2,3007.6],[1260.7,3008.9],[150.9,3009.3],[159.1,386.8]]]");
       m.proj = "+datum=WGS84 +proj=tmerc +lon_0=39 +x_0=500000";
       ConvMap cnv1(m, "SU39");
       dPoint p1(1333, 867, 1100);
       dPoint p2(7321000, 6209000, 1100);
       cnv1.frw(p1);
       assert(dist(p1,p2) < 15); // 15m accuracy (~2px)

       ConvMap cnv2(m, "+ellps=krass +towgs84=28,-130,-95 +proj=latlong");
       dPoint p3(159,386, 2500);
       dPoint p4(36.00,56.00, 2500);
       cnv2.bck(p4);
       assert(dist(p3,p4) < 1); // 1px accuracy

       assert(m.bbox_ref_img() == dRect("[151,386,2220,2624]"));

       dRect r = m.bbox_ref_wgs();
       assert(dist(r.tlc(), dPoint(35.998051,55.833276)) < 1e-6);
       assert(fabs(r.w - 1/4.0) < 1e-4);
       assert(fabs(r.h - 1/6.0) < 1e-4);

       cnv2.rescale_src(2);
       p4 = dPoint(36.00,56.00, 2500/2);
       cnv2.bck(p4);
       assert(dist(p3/2,p4) < 1);

       cnv2.rescale_src(0.5);
       cnv2.rescale_dst(2);
       p4 = dPoint(36.00*2,56.00*2, 2500);
       cnv2.bck(p4);
       assert(dist(p3,p4) < 1);

    }

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
