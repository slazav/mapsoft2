///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "err/err.h"
#include "dbf.h"

int
main(){
  try{
    {
      // create new file
      Dbf db("a", 1);
      assert(db.nfields() == 0);
      assert(db.nrecords() == 0);
      assert(db.field_type(1) == ' '); // no such field

      int fid1 = db.add_str_field("f1", 5);
      int fid2 = db.add_str_field("f2", 10);
      int fid3 = db.add_str_field("f2", 3);
      assert(fid1==0);
      assert(fid2==1);
      assert(fid3==2);

      assert(db.field_find("f1") == 0);
      assert(db.field_find("f2") == 1);
      assert(db.field_find("f3") == -1);

      assert(db.put_str(20, fid1, "val1") == false);
      assert(db.put_str(0, fid1, "val1a") == true);
      assert(db.put_str(1, fid1, "val1b") == true);
      assert(db.put_str(2, fid1, "val1c_") == false);
      assert(db.put_str(130, fid1, "val1") == false);

      assert(db.get_str(0,fid1) == "val1a");
      assert(db.get_str(1,fid1) == "val1b");
      assert(db.get_str(2,fid1) == "val1c");
      assert(db.get_str(130,fid1) == "");

      assert(db.put_str(3, fid2, " a ") == true);
      assert(db.get_str(3,fid2) == "a");

      assert(db.nfields() == 3);
      assert(db.nrecords() == 4);
    }
    {
      // open existing file
      Dbf db("a", 0);
      assert(db.nfields() == 3);
      assert(db.nrecords() == 4);
      assert(db.get_str(0,0) == "val1a");
      assert(db.get_str(1,0) == "val1b");
      assert(db.get_str(2,0) == "val1c");
      assert(db.get_str(130,0) == "");
      assert(db.get_str(2,1) == "");
      assert(db.get_str(3,1) == "a");
    }
  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
