///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "conv_geo.h"

int
main(){
  try{
    std::string proj_wgs = "+datum=WGS84 +proj=lonlat";
    std::string proj_krass = "+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=500000 +lon_0=27";

    ConvGeo cnv1(proj_wgs,proj_wgs);    // wgs -> wgs
    ConvGeo cnv2(proj_krass);           // to krass -> wgs
    ConvGeo cnv3(proj_wgs, proj_krass); // wgs -> krass

    dPoint p1(25.651054, 60.976941, 0);
    iPoint p1a(427091, 6763808, -11);

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

    // z<=-1e7 -- no altitude conversions
    p1 = dPoint(25.651054, 60.976941, -1e8);
    p1a = iPoint (427091, 6763808, -1e8);
    p2=p1;
    cnv2.bck(p2);
    assert(iPoint(p2) == p1a);
    cnv2.frw(p2);
    assert(dist(p1,p2) < 2e-7);

    // rescale_dst (only x,y is affected!)
    cnv3.rescale_dst(2);
    p2=p1;
    cnv3.frw(p2);
    p2.z*=2;
    assert(iPoint(p2) == p1a*2);
    p2.z/=2;
    cnv3.bck(p2);
    assert(dist(p1,p2) < 2e-7);

    // rescale_src
    cnv3.rescale_src(2);
    cnv3.rescale_dst(0.5);
    p2=p1/2; p2.z *= 2;
    cnv3.frw(p2);
    assert(iPoint(p2) == p1a);
    cnv3.bck(p2);
    p2.z/=2;
    assert(dist(p1/2,p2) < 2e-7);


  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
