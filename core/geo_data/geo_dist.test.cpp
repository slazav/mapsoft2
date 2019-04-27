///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "geo_data.h"

main(){
  try{

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


  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond
