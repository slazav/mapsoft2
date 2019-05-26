///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "err/err.h"
#include "db_geohash.h"

int
main(){
  try{
    {
      // create new file
      DBGeoHash db("a.dbh", 1);

      db.put(1, dRect(-0.01,-0.01, 0.02,0.02));
      db.put(2, dRect(1,-0.01,     0.02,0.02));
      db.put(3, dRect(-0.01,1,     0.02,0.02));
      db.put(4, dRect(1,1,         0.01,0.01));
      db.put(5, dRect(36,57,       0.01,0.01));
      db.put(6, dRect(36,57,       0.11,0.11));
      db.put(7, dRect(35,57,       0.11,0.11));
      db.put(8, dRect(35,57,       0.10,0.10));
      db.put(9, dRect(-100,-50,    220,100));

      std::set<int> v1 = db.get(dRect(-0.01,-0.01, 1.12,1.12));
      assert(v1.size()==5);
      assert(v1.count(1)==1);
      assert(v1.count(2)==1);
      assert(v1.count(3)==1);
      assert(v1.count(4)==1);
      assert(v1.count(9)==1);

      v1 = db.get(dRect(36,57, 0.001,0.001));
      //for (auto i:v1) std::cerr << "> " << i << "\n";
      assert(v1.size()==3);
      assert(v1.count(5)==1);
      assert(v1.count(6)==1);
      assert(v1.count(9)==1);

      v1 = db.get(dRect(-180,-90, 360,180));
      //for (auto i:v1) std::cerr << "> " << i << "\n";
      assert(v1.size()==9);
    }
    {
      // open existing file
      DBGeoHash db("a.dbh", 0);
      std::set<int> v1 = db.get(dRect(36,57, 0.001,0.001));
      //for (auto i:v1) std::cerr << "> " << i << "\n";
      assert(v1.size()==3);
      assert(v1.count(5)==1);
      assert(v1.count(6)==1);
      assert(v1.count(9)==1);

    }
  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
