///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "conv_geo.h"

main(){
  try{
    std::string proj_wgs = "+datum=WGS84 +proj=lonlat";
    std::string proj_krass = "+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=500000 +lon_0=27";

    ConvGeo cnv1(proj_wgs,proj_wgs);

    dPoint p1(25.651054, 60.976941, 0);
    iPoint p1a(427091, 6763808, -11);

    dPoint p2(p1);
    cnv1.frw(p2);
    assert(p1==p2);
    cnv1.bck(p2);
    assert(p1==p2);

    ConvGeo cnv2(proj_krass); // to krass->wgs84

    cnv2.bck(p2);
    assert(iPoint(p2) == p1a);
    cnv2.frw(p2);
    assert(dist(p1,p2) < 2e-7);

    // wgs->krass
    ConvGeo cnv3(proj_wgs, proj_krass);

    cnv3.frw(p2);
    assert(iPoint(p2) == p1a);
    cnv3.bck(p2);
    assert(dist(p1,p2) < 2e-7);

    dLine l1;
    iLine l1a;
    for (int i=0; i<3; i++) {l1.push_back(p1); l1a.push_back(p1a);}
    cnv3.frw(l1);
    assert(iLine(l1) == l1a);


  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond
