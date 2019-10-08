///\cond HIDDEN (do not show this in Doxyden)

#include <unistd.h>
#include <cassert>
#include <iostream>
#include "err/err.h"
#include "mapdb.h"

using namespace std;

int
main(){
  try{

    // MapDBObj structure
    {
      MapDBObj o1,o2;
      // defaults
      assert(o1.cl == MAPDB_POINT);
      assert(o1.type == 0);
      assert(o1.dir == MAPDB_DIR_NO);
      assert(o1.angle == 0);
      assert(o1.name == "");
      assert(o1.comm == "");
      assert(o1.tags.size() == 0);
      assert(o1.size() == 0);

      // <=> operators
      assert(o1 == o2);
      assert(o1 >= o2);
      assert(o1 <= o2);
      o2.cl = MAPDB_LINE;
      assert(o1 != o2);
      assert(o1 < o2);
      assert(o1 <= o2);
      assert(o2 > o1);
      assert(o2 >= o1);

      o2=o1; o2.type = 1;
      assert(o1 != o2);
      assert(o1 < o2);
      assert(o1 <= o2);
      assert(o2 > o1);
      assert(o2 >= o1);

      o2=o1; o2.dir = MAPDB_DIR_BCK;
      assert(o1 != o2);
      assert(o1 < o2);
      assert(o1 <= o2);
      assert(o2 > o1);
      assert(o2 >= o1);

      o2=o1; o2.angle = 10;
      assert(o1 != o2);
      assert(o1 < o2);
      assert(o1 <= o2);
      assert(o2 > o1);
      assert(o2 >= o1);

      o2=o1;
      o1.name = "a";
      o2.name = "b";
      assert(o1 != o2);
      assert(o1 < o2);
      assert(o1 <= o2);
      assert(o2 > o1);
      assert(o2 >= o1);

      o2=o1;
      o1.comm = "a";
      o2.comm = "b";
      assert(o1 != o2);
      assert(o1 < o2);
      assert(o1 <= o2);
      assert(o2 > o1);
      assert(o2 >= o1);

      o2=o1;
      o1.tags.insert("a");
      o2.tags.insert("b");
      assert(o1 != o2);
      assert(o1 < o2);
      assert(o1 <= o2);
      assert(o2 > o1);
      assert(o2 >= o1);

      o2.tags.insert("a");
      o1.tags.insert("b");
      assert(o1 == o2);
      assert(o1 <= o2);
      assert(o2 >= o1);

    }

    // packing and unpacking of MapDBObj
    // for OBJ database
    {
      MapDBObj o1,o2;
      o1.cl = MAPDB_LINE;
      o1.type = 0x2342;
      o1.dir  = MAPDB_DIR_NO;
      o1.angle  = 60;
      o1.name = "object name\nsecond line";
      o1.comm = "object comment\nsecond line";
      o1.tags.insert("object source\nsecond line");
      o1.dMultiLine::operator=(dMultiLine("[[[0,0],[1,1]],[[1,1],[2,2]]]"));
      std::string pack = o1.pack();
      o2.unpack(pack);
      assert(o1==o2);

      o1.cl = MAPDB_POINT;
      o1.type = 0x12;
      o1.dir  = MAPDB_DIR_FRW;
      o1.angle  = 0;
      o1.name = "";
      o1.comm = "";
      o1.tags.clear();
      pack = o1.pack();
      o2.unpack(pack);
      assert(o1==o2);
    }


    // MapDB test
    {
      if (system("rm -rf tmp.db")!=0) throw Err() << "Can't delete tmp.db";
      MapDB m("tmp.db", 1);

      // mapinfo.db test

      // get/set name
      assert(m.get_map_name() == "");
      m.set_map_name("");
      assert(m.get_map_name() == "");
      m.set_map_name("Test");
      assert(m.get_map_name() == "Test");

      // get/set border
      dMultiLine brd("[[[1,1],[2,2],[3,3]],[[4,4],[5,5]]]");
      assert(m.get_map_brd() == dMultiLine());
      m.set_map_brd(brd);
      assert(m.get_map_brd() == brd);

      // get version
      assert(m.get_map_version() == 0);

      // get/set object
      MapDBObj o1;
      o1.cl = MAPDB_LINE;
      o1.type = 0x2342;
      o1.dir  = MAPDB_DIR_NO;
      o1.angle  = 60;
      o1.name = "object name\nsecond line";
      o1.comm = "object comment\nsecond line";
      o1.tags.insert("object source\nsecond line");

      // add/get object
      uint32_t id = m.add(o1);
      assert(id == 0);
      assert(o1 == m.get(id));

      // update object
      o1.type = 0x2342;
      o1.dMultiLine::operator=(dMultiLine("[[[0,0],[1,1]],[[1,1],[2,2]]]"));
      m.put(id,o1);
      assert(o1 == m.get(id));

      // delete object
      m.del(id);
      try {m.get(id); assert(false);} catch (Err e) {
        assert(e.str() == "MapDB::get: no such object: 0");
      }

      // find
      id = m.add(o1);
      assert(o1 == m.get(id));
      assert(m.find(o1.cl, o1.type+1, dRect("[1,1,1,1]")).size() == 0);
      assert(m.find(MAPDB_POINT, o1.type, dRect("[1,1,1,1]")).size() == 0);
      assert(m.find(o1.cl, o1.type, dRect("[10,1,1,1]")).size() == 0);
      std::set<int> ii = m.find(o1.cl, o1.type, dRect("[1,1,1,1]"));
      assert(ii.size()==1);


    }
    if (system("rm -rf tmp.db")!=0) throw Err() << "Can't delete tmp.db";

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
