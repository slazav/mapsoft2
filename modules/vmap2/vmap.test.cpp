///\cond HIDDEN (do not show this in Doxyden)

#include <unistd.h>
#include <cassert>
#include <iostream>
#include "err/err.h"
#include "vmap.h"

using namespace std;

/* tests for VMAP objects */

int
main(){
  try{
    VMapObj o1,o2;
    // defaults
    assert(o1.cl == VMAP_POINT);
    assert(o1.type == 0);
    assert(o1.dir == VMAP_DIR_NO);
    assert(o1.angle == 0);
    assert(o1.name == "");
    assert(o1.comm == "");
    assert(o1.src == "");
    assert(o1.size() == 0);

    // <=> operators
    assert(o1 == o2);
    assert(o1 >= o2);
    assert(o1 <= o2);
    o2.cl = VMAP_LINE;
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

    o2=o1; o2.dir = VMAP_DIR_BCK;
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

    o2=o1;
    o1.push_back(dLine("[[1,2],[3,3]]"));
    o2.push_back(dLine("[[1,2],[3,4]]"));
    assert(o1 != o2);
    assert(o1 < o2);
    assert(o1 <= o2);
    assert(o2 > o1);
    assert(o2 >= o1);

    o2=o1;
    o1.push_back(dLine("[[1,2]]"));
    o2.push_back(dLine("[[1,2],[3,4]]"));
    assert(o1 != o2);
    assert(o1 < o2);
    assert(o1 <= o2);
    assert(o2 > o1);
    assert(o2 >= o1);

    {
      unlink("tmp.db");
      VMap m("tmp.db", 1);

      // get/set name
      assert(m.get_name() == "");
      m.set_name("");
      assert(m.get_name() == "");
      m.set_name("Test");
      assert(m.get_name() == "Test");

      // get/set border
      dMultiLine brd("[[[1,1],[2,2],[3,3]],[[4,4],[5,5]]]");
      assert(m.get_brd() == dMultiLine());
      m.set_brd(brd);
      assert(m.get_brd() == brd);

      // get/set bbox
      assert(m.get_bbox() == dRect());
      VMapObj o1;
      o1.push_back(dLine("[[1,2],[3,3]]"));
      m.add(o1);
      assert(m.get_bbox() == dRect(dPoint(1,2), dPoint(3,3)));
      o1.clear();
      o1.push_back(dLine("[[0,0]]"));
      m.add(o1);
      assert(m.get_bbox() == dRect(dPoint(0,0), dPoint(3,3)));

    }
    unlink("tmp.db");

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
