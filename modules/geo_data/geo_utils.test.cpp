///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "err/assert_err.h"
#include "geo_utils.h"

int
main(){
  try {

    {  //geo_dist_2d

      // large distance:
      // nakarte: 885370
      // formula: 886625
      double d1 = 886625;
      assert ( abs(geo_dist_2d(
        dPoint(37.403169, 55.803693),
        dPoint(24.803224, 60.174925))-d1) < 1.0 );

      // nakarte: 7960
      // map:     8000
      // formula: 7973.47
      double d2 = 7973.47;
      assert( abs(geo_dist_2d(
        dPoint(37.340126, 55.803018),
        dPoint(37.467499, 55.804658))-d2) < 1.0);

      // nakarte: 63870
      // map:     64000
      // formula: 63961.6
      double d3 = 63961.6;
      assert( abs(geo_dist_2d(
        dPoint(37.589979, 55.932279),
        dPoint(37.568607, 56.506565))-d3) <1.0);

      // pole to pole
      double d4 = 6380e3*M_PI;
      assert( abs(geo_dist_2d(
        dPoint(12, -90),
        dPoint(22,  90))-d4) <1.0);

      // pole to equator
      double d5 = 6380e3*M_PI/2;
      assert( abs(geo_dist_2d(
        dPoint(12, -90),
        dPoint(22,   0))-d5) <1.0);
    }

  /****************************/
  // lon2lon0
  assert(lon2lon0(0.1) == 3);
  assert(lon2lon0(-0.1) == -3);
  assert(lon2lon0(179) == 177);
  assert(lon2lon0(-179) == -177);
  assert(lon2lon0(37+360) == 39);
  assert(lon2lon0(37-360) == 39);

  // boundary cases
  assert(lon2lon0(6-1e-10) == 3);
  assert(lon2lon0(6) == 9);
  assert(lon2lon0(0-1e-10) == -3);
  assert(lon2lon0(0) == 3);

  // lon2pref
  assert(lon2pref(37) == 7);
  assert(lon2pref(34) == 6);
  assert(lon2pref(-73.5) == 48);
  assert(lon2pref(34+720) == 6);
  assert(lon2pref(34-720) == 6);

  // boundary cases
  assert(lon2pref(6-1e-10) == 1);
  assert(lon2pref(6) == 2);
  assert(lon2pref(0-1e-10) == 60);
  assert(lon2pref(0) == 1);

  // crdx2lon0
  assert(crdx2lon0(7800000) == 39);
  assert(crdx2lon0(48800000) == -75);
  assert(crdx2lon0(1800000) == 3);

  assert_err(crdx2lon0(800000), "zero coordinate prefix");

  // crdx2nonpref
  assert(crdx2nonpref(7800000) == 800000);
  assert(crdx2nonpref(800000) == 800000);
  assert(crdx2nonpref(60800000) == 800000);

  // no prefix
  assert(crdx2nonpref(800000) == 800000);

  assert(GEO_PROJ_SU(31) ==
  "+ellps=krass +towgs84=+28,-130,-95 +proj=tmerc"
  " +lon_0=33 +x_0=6500000");
  assert(GEO_PROJ_SU(30) ==
  "+ellps=krass +towgs84=+28,-130,-95 +proj=tmerc"
  " +lon_0=33 +x_0=6500000");
  assert(GEO_PROJ_SU(5) ==
  "+ellps=krass +towgs84=+28,-130,-95 +proj=tmerc"
  " +lon_0=3 +x_0=1500000");
  assert(GEO_PROJ_SU(-10) ==
  "+ellps=krass +towgs84=+28,-130,-95 +proj=tmerc"
  " +lon_0=-9 +x_0=59500000");

  }
  catch (Err E){
    std::cerr << "Error: " << E.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
