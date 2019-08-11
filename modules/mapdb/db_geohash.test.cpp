///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <unistd.h>
#include <iostream>
#include "err/err.h"
#include "db_geohash.h"

int
main(){
  try{
    {
      // create new file
      GeoHashDB db("a.dbh", NULL, 1);

      db.put(1, 0, dRect(-0.01,-0.01, 0.02,0.02));
      db.put(2, 0, dRect(1,-0.01,     0.02,0.02));
      db.put(3, 0, dRect(-0.01,1,     0.02,0.02));
      db.put(4, 0, dRect(1,1,         0.01,0.01));
      db.put(5, 0, dRect(36,57,       0.01,0.01));
      db.put(6, 0, dRect(36,57,       0.11,0.11));
      db.put(7, 0, dRect(35,57,       0.11,0.11));
      db.put(8, 0, dRect(35,57,       0.10,0.10));
      db.put(9, 0, dRect(-100,-50,    220,100));
      db.put(10, 0, dRect()); // empty range - do nothing

      std::set<int> v1 = db.get(0, dRect());
      assert(v1.size()==0);

      v1 = db.get(0, dRect(-0.01,-0.01, 1.12,1.12));
      //for (auto i:v1) std::cerr << "> " << i << "\n";
      assert(v1.size()==5);
      assert(v1.count(1)==1);
      assert(v1.count(2)==1);
      assert(v1.count(3)==1);
      assert(v1.count(4)==1);
      assert(v1.count(9)==1);

      v1 = db.get(1, dRect(-0.01,-0.01, 1.12,1.12));
      assert(v1.size()==0);

      v1 = db.get(0, dRect(36,57, 0.001,0.001));
      //for (auto i:v1) std::cerr << "> " << i << "\n";
      assert(v1.size()==3);
      assert(v1.count(5)==1);
      assert(v1.count(6)==1);
      assert(v1.count(9)==1);

      v1 = db.get(0, dRect(-180,-90, 360,180));
      //for (auto i:v1) std::cerr << "> " << i << "\n";
      assert(v1.size()==9);
    }
    {
      // open existing file
      GeoHashDB db("a.dbh", NULL, 0);
      std::set<int> v1 = db.get(0, dRect(36,57, 0.001,0.001));
      //for (auto i:v1) std::cerr << "> " << i << "\n";
      assert(v1.size()==3);
      assert(v1.count(5)==1);
      assert(v1.count(6)==1);
      assert(v1.count(9)==1);

      db.del(9, 0, dRect(-100,-50, 220,100));
      db.del(5, 1, dRect(-100,-50, 220,100));
      db.put(9, 1, dRect(-100,-50, 220,100));
      //for (auto i:v1) std::cerr << "> " << i << "\n";
      v1 = db.get(0, dRect(36,57, 0.001,0.001));
      assert(v1.size()==2);
      assert(v1.count(5)==1);
      assert(v1.count(6)==1);

      v1 = db.get(1, dRect(36,57, 0.001,0.001));
      assert(v1.size()==1);
      assert(v1.count(9)==1);

      db.del(5, 0, dRect(36,57,0.01,0.01));
      v1 = db.get(0, dRect(36,57, 0.001,0.001));
      assert(v1.size()==1);
      assert(v1.count(6)==1);

      // non-existing id-type-range combinations!
      db.del(6, 0, dRect(36,57,0.01,0.01));
      db.del(6, 1, dRect(36,57,0.11,0.11));
      v1 = db.get(0, dRect(36,57, 0.001,0.001));
      assert(v1.size()==1);
      assert(v1.count(6)==1);


    }
    unlink("a.dbh");

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
