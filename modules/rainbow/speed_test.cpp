///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "rainbow.h"

#include <iostream>
#include <iomanip>

int
main(){

  // 5.21s / 2e8 -- original code
  // 3.65s / 2e8 -- aac7c74b6e32162b7be66c65a503bd7b07afe75a
  // 3.21s / 2.8 -- move cc,vv,rr,gg,bb,N,dir to constructor
  // 3.08s / 2.8 -- separate setting of limits

  Rainbow R(0, 1, RAINBOW_NORMAL);
  for (int i=0; i<2e8; i++) {
    R.get(1);
  }

  return 0;
}

///\endcond
