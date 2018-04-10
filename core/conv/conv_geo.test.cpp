///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "conv_geo.h"

main(){
  try{
    ConvGeo cnv1("+datum=WGS84 +proj=lonlat","+datum=WGS84 +proj=lonlat");

    dPoint p1(25.651054, 60.976941);
    dPoint p2(p1);
    cnv1.frw(p2);
    assert(p1==p2);
    cnv1.bck(p2);
    assert(p1==p2);

    std::string proj1="+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=500000 +lon_0=27";

    ConvGeo cnv2(proj1); // to wgs84

    cnv2.bck(p2);
    assert(iPoint(p2) == iPoint(427091, 6763808));
    cnv2.frw(p2);
    assert(iPoint(p2*1e6) == iPoint(p1*1e6));

    ConvGeo cnv3("+datum=WGS84 +proj=lonlat", proj1);

    cnv3.frw(p2);
    assert(iPoint(p2) == iPoint(427091, 6763808));
    cnv3.bck(p2);
    assert(iPoint(p2*1e6) == iPoint(p1*1e6));

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond
