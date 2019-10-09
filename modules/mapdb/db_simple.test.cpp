///\cond HIDDEN (do not show this in Doxyden)

#include <unistd.h>
#include <cassert>
#include <iostream>
#include "err/assert_err.h"
#include "db_simple.h"

int
main(){
  try{
    {
      // create new file
      unlink("a.dbp");

      DBSimple db("a.dbp", NULL, 1);
      uint32_t key=1;
      assert_eq(db.get(key), "");
      assert_eq(key, 0xFFFFFFFF);
      key=1;
      assert_eq(db.get_range(key), "");
      assert_eq(key, 0xFFFFFFFF);
      key=1;
      assert_eq(db.get_first(key), "");
      assert_eq(key, 0xFFFFFFFF);
      key=1;
      assert_eq(db.get_next(key), "");
      assert_eq(key, 0xFFFFFFFF);
      key=1;
      assert_eq(db.get_last(key), "");
      assert_eq(key, 0xFFFFFFFF);
      key=1;

      db.put(1, "abc");
      db.put(2, "cde");
      db.put(3, "def");
      db.put(2, "fgh"); // overwrite
      db.put(128, "aaa");
      db.put(257, "bbb");

      assert_eq(db.exists(1), true);
      assert_eq(db.exists(4), false);
      assert_eq(db.exists(257), true);
      assert_eq(db.exists(258), false);

      key=525;
      assert_eq(db.get_range(key), "");
      assert_eq(key, 0xFFFFFFFF);

      key=2;
      assert_eq(db.get_range(key), "fgh");
      assert_eq(key, 2);

      key=525;
      assert_eq(db.get_last(key), "bbb");
      assert_eq(key, 257);

      key=2;
      assert_eq(db.get_last(key), "bbb");
      assert_eq(key, 257);

      key=525;
      assert_eq(db.get_first(key), "abc");
      assert_eq(key, 1);

      key=0;
      assert_eq(db.get_first(key), "abc");
      assert_eq(key, 1);

      key=0xFFFFFFFF;
      assert_eq(db.get_range(key), "");
      assert_eq(key, 0xFFFFFFFF);

      key=0;
      assert_eq(db.get_range(key), "abc");
      assert_eq(key, 1);
      assert_eq(db.get_range(key), "abc");
      assert_eq(key, 1);

      key=2;
      assert_eq(db.get(key), "fgh");
      assert_eq(key, 2);

      assert_eq(db.get_next(key), "def");
      assert_eq(key, 3);
      assert_eq(db.get_next(key), "aaa");
      assert_eq(key, 128);
      assert_eq(db.get_next(key), "bbb");
      assert_eq(key, 257);
      assert_eq(db.get_prev(key), "aaa");
      assert_eq(key, 128);
      assert_eq(db.get_prev(key), "def");
      assert_eq(key, 3);
      assert_eq(db.get_prev(key), "fgh");
      assert_eq(key, 2);

      assert_eq(db.del(128), 1);
      assert_eq(db.del(257), 1);
      assert_eq(db.del(257), 0);
      key=257;
      assert_eq(db.get(key), "");

    }
    {
      // open existing file
      DBSimple db("a.dbp", NULL, 0);
      uint32_t key = 2;
      assert_eq(db.get(key), "fgh");
      assert_eq(db.get_next(key), "def");
    }
    unlink("a.dbp");

    {
      // duplicate keys
      DBSimple db1("a.dbp", NULL, 1, 1);
      db1.put(1, "abc1");
      db1.put(1, "abc2");
      db1.put(2, "cde1");
      db1.put(2, "cde2");

      uint32_t key;
      key = 1;
      assert_eq(db1.get(key), "abc1");      assert_eq(key,1);
      assert_eq(db1.get_next(key), "abc2"); assert_eq(key,1);
      assert_eq(db1.get_next(key), "cde1"); assert_eq(key,2);
      assert_eq(db1.get_next(key), "cde2"); assert_eq(key,2);

      assert_eq(db1.get_range(key), "cde1");
      assert_eq(db1.get_next(key), "cde2"); assert_eq(key,2);

      assert_eq(db1.del(2), 2);
      assert_eq(db1.del(2), 0);
      assert_eq(db1.get(key), "");

    }
    unlink("a.dbp");


    {
      // open two databases in one file
      DBSimple db1("a.dbp", "db1", 1);
      db1.put(1, "abc");
      db1.put(2, "cde");

      DBSimple db2("a.dbp", "db2", 1);
      db2.put(1, "abc2");
      db2.put(2, "cde2");

      uint32_t key;
      key = 1;
      assert_eq(db1.get(key), "abc");
      assert_eq(db1.get_next(key), "cde");

      key = 1;
      assert_eq(db2.get(key), "abc2");
      assert_eq(db2.get_next(key), "cde2");
    }
    unlink("a.dbp");


  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
