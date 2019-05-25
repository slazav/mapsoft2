///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "err/err.h"
#include "shape.h"

int
main(){
  try{
    Shape M("a", 1, MAP_LINE);
    assert(M.get_type() == MAP_LINE);
    assert(M.get_num() == 0);

    dMultiLine l1("[[[0,1],[1,2],[3,4]],[[0,0],[1,1]]]");
    dMultiLine l2("[[[1,2]]]");
    int id1 = M.put(l1);
    int id2 = M.put(l2);

    dMultiLine l1a = M.get(id1);
    dMultiLine l2a = M.get(id2);

    assert(l1==l1a);
    assert(l2==l2a);
    assert(id1==0);
    assert(id2==1);
  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond