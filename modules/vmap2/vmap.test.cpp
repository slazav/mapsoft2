///\cond HIDDEN (do not show this in Doxyden)

#include <unistd.h>
#include <cassert>
#include <iostream>
#include "err/err.h"
#include "vmap.h"

using namespace std;

int
main(){
  try{
    VMapObj o1,o2;
    // defaults
    assert(o1.cl == POINT);
    assert(o1.type == 0);
    assert(o1.dir == FRW);
    assert(o1.name == "");
    assert(o1.comm == "");
    assert(o1.src == "");
    assert(o1.size() == 0);

    // <=> operators
    assert(o1 == o2);
    assert(o1 >= o2);
    assert(o1 <= o2);
    o2.cl = LINE;
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

    o2=o1; o2.dir = BCK;
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

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
