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
    VMapObj o;
    assert(o.cl == POINT);
    assert(o.type == 0);
    assert(o.dir == FRW);


    VMap map;
//    assert(map.bbox2d() == dRect());
//    map.push_back(o);
//    assert(map.bbox2d() == dRect());
//    o.dMultiLine::operator=(dMultiLine("[[[0,0],[1,1]]]"));
//    map.push_back(o);
//    assert(map.bbox2d() == dRect(0,0,1,1));


  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
