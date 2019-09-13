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
      assert(p1.t == 0);
      assert(std::isnan(p1.z));
      assert(std::isnan(p1.z));
      assert(p1.have_alt() == false);
      p1.z = 0;
      assert(!std::isnan(p1.z));
      assert(p1.have_alt() == true);
      p1.clear_alt();
      assert(p1.have_alt() == false);

      p1 = dPoint(10,10,0); // z is undef!
      assert(p1.x == 10);
      assert(p1.y == 10);
      assert(p1.t == 0);
      assert(p1.have_alt() == false);
      p1.opts.put<double>("test", 10.0);
      assert (p1.opts.get<double>("test", 0.0) == 10);

      p1 = GeoWpt(11,11);
      assert(p1.x == 11);
      assert(p1.y == 11);
      assert(p1.t == 0);
      assert(p1.have_alt() == false);
    }

    { // GeoTpt

      // constructor, clear/set alt
      GeoTpt p1;
      assert(p1.x == 0);
      assert(p1.y == 0);
      assert(p1.t == 0);
      assert(p1.start == false);
      assert(std::isnan(p1.z));
      assert(p1.have_alt() == false);
      p1.z = 0;
      assert(p1.have_alt() == true);
      assert(p1.z == 0);
      p1.clear_alt();
      assert(p1.have_alt() == false);

      p1 = dPoint(10,10,0);
      assert(p1.x == 10);
      assert(p1.y == 10);
      assert(p1.t == 0);
      assert(p1.have_alt() == false);
    }

    { // GeoWptList
      GeoWptList l1;
      l1.push_back(GeoWpt(37.403169, 55.803693, 210));
      l1.push_back(GeoWpt(24.803224, 60.174925, 20));
      assert(l1.size() == 2);
      assert(iRect(l1.bbox()*10.0) == iRect(248,558,125,43));
      assert(l1[0].z == 210);
      assert(l1[1].z == 20);
      l1.clear_alt();
      assert(std::isnan(l1[0].z));
      assert(std::isnan(l1[1].z));
    }

    { // GeoTrk
      GeoTrk l1;
      l1.push_back(GeoTpt(37.403169, 55.803693, 210));
      l1.push_back(GeoTpt(24.803224, 60.174925, 20,0));
      assert(l1.size() == 2);
      assert(abs(l1.length() - 886625) < 1);
      assert(iRect(l1.bbox()*10.0) == iRect(248,558,125,43));
      assert(l1[0].z == 210);
      assert(l1[1].z == 20);
      l1.clear_alt();
      assert(std::isnan(l1[0].z));
      assert(std::isnan(l1[1].z));
    }

    { // GeoMap
      GeoMap m1,m2;
      assert(m1==m2);
      assert(m1<=m2);
      assert(m1>=m2);

      assert(m1.image_dpi == 300);
      assert(m1.tile_size == 256);
      assert(m1.tile_yswap == false);

      assert(m1.ref.size()==0);
      m1.add_ref(1,2,30,40);
      m1.add_ref(dPoint("[2,3]"),dPoint("[41,51]"));
      m1.add_ref(dLine("[[3,4],[4,5]]"),dLine("[[52,62],[63,73]]"));

      std::ostringstream ss;
      for (auto & r:m1.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[1,2] [30,40]"
        "[2,3] [41,51]"
        "[3,4] [52,62]"
        "[4,5] [63,73]");

    }

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
