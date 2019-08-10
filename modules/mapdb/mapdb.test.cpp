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
      assert(o1.src == "");

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
      o1.src = "a";
      o2.src = "b";
      assert(o1 != o2);
      assert(o1 < o2);
      assert(o1 <= o2);
      assert(o2 > o1);
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
      o1.src = "object source\nsecond line";
      std::string pack = o1.pack();
      o2.unpack(pack);
      assert(o1==o2);

      o1.cl = MAPDB_POINT;
      o1.type = 0x12;
      o1.dir  = MAPDB_DIR_FRW;
      o1.angle  = 0;
      o1.name = "";
      o1.comm = "";
      o1.src = "";
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

      // get/set object coordinates and bboxes
      assert(m.get_map_bbox() == dRect());
      MapDBObj o1;
      o1.cl = MAPDB_LINE;
      o1.type = 0x2342;
      o1.dir  = MAPDB_DIR_NO;
      o1.angle  = 60;
      o1.name = "object name\nsecond line";
      o1.comm = "object comment\nsecond line";
      o1.src = "object source\nsecond line";

      // put object, check bbox and coords
      uint32_t id = m.add(o1);
      assert(m.get_bbox(id) == dRect());
      assert(m.get_coord(id) == dMultiLine());
      assert(o1 == m.get(id));

      m.set_coord(id, dMultiLine("[[[1,2],[3,3]]]"));
      assert(m.get_bbox(id) == dRect(dPoint(1,2), dPoint(3,3)));
      assert(m.get_coord(id) == dMultiLine("[[[1,2],[3,3]]]"));
      assert(o1 == m.get(id)); // no change in the object

      assert(m.get_map_bbox() == dRect(dPoint(1,2), dPoint(3,3))); // map bbox

      m.set_coord(id, dMultiLine("[[[0,0],[5,5]]]"));
      assert(m.get_map_bbox() == dRect(dPoint(0,0), dPoint(5,5)));

      m.set_coord(id, dMultiLine());
      assert(m.get_bbox(id) == dRect());
      assert(m.get_coord(id) == dMultiLine());
      assert(o1 == m.get(id));

      // todo: shrinking of the map bbox -- not implemented

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
