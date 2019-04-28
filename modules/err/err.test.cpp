///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "err.h"

int
main(){
  try {
    throw Err() << "text " << 123;
  }
  catch (Err E){
    assert (E.str() == "text 123");
    assert (E.code() == -1);
  }

  try {
    throw Err(3) << "text " << 123;
  }
  catch (Err E){
    assert (E.str()  == "text 123");
    assert (E.code() == 3);
  }
  return 0;
}

///\endcond