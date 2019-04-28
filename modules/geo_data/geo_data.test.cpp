///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "geo_data.h"

int
main(){
  try{

    { // GeoWpt

      // constructor, clear/set alt
      GeoWpt p1;
      assert(p1.x == 0);
      assert(p1.y == 0);
      assert(p1.z == UNDEF_ALT);
      assert(p1.have_alt() == false);
      p1.z = 0;
      assert(p1.have_alt() == true);
      p1.clear_alt();
      assert(p1.have_alt() == false);

      p1 = dPoint(10,10);
      assert(p1.x == 10);
      assert(p1.y == 10);
      assert(p1.z == 0);
      p1.opts.put<double>("test", 10.0);
      assert (p1.opts.get<double>("test", 0.0) == 10);

      p1 = GeoWpt(11,11);
      assert(p1.x == 11);
      assert(p1.y == 11);
      assert(p1.z == UNDEF_ALT);
    }

    { // GeoTpt

      // constructor, clear/set alt
      GeoTpt p1;
      assert(p1.x == 0);
      assert(p1.y == 0);
      assert(p1.t == 0);
      assert(p1.d == UNDEF_ALT);
      assert(p1.z == UNDEF_ALT);
      assert(p1.have_alt() == false);
      assert(p1.have_depth() == false);
      p1.z = 0; p1.d = 0;
      assert(p1.have_alt() == true);
      assert(p1.have_depth() == true);
      p1.clear_alt();
      p1.clear_depth();
      assert(p1.have_alt() == false);
      assert(p1.have_depth() == false);

      p1 = dPoint(10,10);
      assert(p1.x == 10);
      assert(p1.y == 10);
      assert(p1.z == 0);
      assert(p1.d == UNDEF_ALT);
    }

    { // GeoWptList
      GeoWptList l1;
      l1.push_back(GeoWpt(37.403169, 55.803693, 210));
      l1.push_back(GeoWpt(24.803224, 60.174925, 20));
      assert(l1.size() == 2);
      assert(iRect(l1.bbox2d()*10) == iRect(248,558,125,43));
      assert(l1[0].z == 210);
      assert(l1[1].z == 20);
      l1.clear_alt();
      assert(l1[0].z == UNDEF_ALT);
      assert(l1[1].z == UNDEF_ALT);
    }

    { // GeoTrk
      GeoTrk l1;
      l1.push_back(GeoTpt(37.403169, 55.803693, 210));
      l1.push_back(GeoTpt(24.803224, 60.174925, 20,0));
      assert(l1.size() == 2);
      assert(abs(l1.length() - 886625) < 1);
      assert(iRect(l1.bbox2d()*10) == iRect(248,558,125,43));
      assert(l1[0].z == 210);
      assert(l1[1].z == 20);
      l1.clear_alt();
      assert(l1[0].z == UNDEF_ALT);
      assert(l1[1].z == UNDEF_ALT);

      assert(l1[0].d == UNDEF_ALT);
      assert(l1[1].d == 0);
      l1.clear_depth();
      assert(l1[0].d == UNDEF_ALT);
      assert(l1[1].d == UNDEF_ALT);
    }

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond