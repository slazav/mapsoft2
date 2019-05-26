///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "err/err.h"
#include "db_simple.h"

int
main(){
  try{
    {
      // create new file
      DBSimple db("a.dbp", 1);

      db.put(1, "abc");
      db.put(2, "cde");
      db.put(3, "def");
      db.put(2, "fgh"); // overwrite

      assert(db.get(3) == "def");
      assert(db.get(2) == "fgh");
      assert(db.get(1) == "abc");
      try {
        db.get(4);
        assert(false);
      }
      catch(Err e) {
        assert(e.str() == "db_simple: DB_NOTFOUND: No matching key/data pair found");
      }

    }
    {
      // open existing file
      DBSimple db("a.dbp", 0);
      assert(db.get(3) == "def");
      assert(db.get(2) == "fgh");
    }
  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
