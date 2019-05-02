///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "rainbow.h"

#include <iostream>
#include <iomanip>

int
main(){

  // 5.21s / 2e8
  // 4.66s / 2e8 -- m1
  // 3.78s / 2e8 -- m2

  Rainbow R(0, 1, RAINBOW_NORMAL);
  for (int i=0; i<2e8; i++) {
    R.get(1);
  }

  return 0;
}

///\endcond
