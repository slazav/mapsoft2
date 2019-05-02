///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "rainbow.h"

#include <iostream>
#include <iomanip>

int
main(){

/************************************************/
// user-defined rainbow data


  {
    // empty data -> always return 0
    std::vector<rainbow_data> RD;
    Rainbow R(RD);
    assert(R.get(1.2) == 0);
    assert(R.get(0.5) == 0);
  }

  {
    std::vector<rainbow_data> RD ={
      {0.1, 0x000000},
      {0.5, 0xFF0000}, // 0.1 - 0.5 black -> blue
      {0.5, 0xFF00FF}, // - color step
      {0.9, 0x000000} // 0.5 - 0.9 magenta -> black
    };
    Rainbow R(RD);

    assert(R.get(0.0) == 0);
    assert(R.get(0.2) == 0x400000);
    assert(R.get(0.4999) == 0xFF0000);
    assert(R.get(0.5000) == 0xFF0000);
    assert(R.get(0.5001) == 0xFF00FF);
    assert(R.get(0.9) == 0);
    assert(R.get(1.0) == 0);

    R.set_limits(0x111111, -1);
    assert(R.get(0.0) == 0x111111);
    assert(R.get(1.0) == 0);

    R.set_limits(0x111111, 0x222222);
    assert(R.get(0.0) == 0x111111);
    assert(R.get(1.0) == 0x222222);
  }

/************************************************/
// same tests but opposite sorting of the data
  {
    std::vector<rainbow_data> RD = {
      {0.9, 0x000000},
      {0.5, 0xFF00FF},
      {0.5, 0xFF0000},
      {0.1, 0x000000}
    };

    Rainbow R(RD);

    assert(R.get(0.0) == 0);
    assert(R.get(0.2) == 0x400000);
    assert(R.get(0.4999) == 0xFF0000);
    // the step point is different!
    assert(R.get(0.5000) == 0xFF00FF);
    assert(R.get(0.5001) == 0xFF00FF);
    assert(R.get(0.9) == 0);
    assert(R.get(1.0) == 0);

    R.set_limits(0x111111, -1);
    assert(R.get(0.0) == 0x111111);
    assert(R.get(1.0) == 0);

    R.set_limits(0x111111, 0x222222);
    assert(R.get(0.0) == 0x111111);
    assert(R.get(1.0) == 0x222222);
  }

/************************************************/
// two-color gradient and limits
  {
    Rainbow R(1, 2, 0x00, 0x4040);
    assert(R.get(0.5) == 0x0000);
    assert(R.get(1)   == 0x0000);
    assert(R.get(1.5) == 0x2020);
    assert(R.get(2)   == 0x4040);
    assert(R.get(2.5) == 0x4040);

    R.set_limits(1, 2);
    assert(R.get(0.5)   == 1);
    assert(R.get(1)     == 0);
    assert(R.get(1.5)   == 0x2020);
    assert(R.get(2)     == 0x4040);
    assert(R.get(2.5)   == 2);
  }

  // same, inversed order
  {
    Rainbow R(2, 1, 0x4040, 0x00);
    assert(R.get(0.5) == 0x0000);
    assert(R.get(1)   == 0x0000);
    assert(R.get(1.5) == 0x2020);
    assert(R.get(2)   == 0x4040);
    assert(R.get(2.5) == 0x4040);

    R.set_limits(1, 2);
    assert(R.get(0.5)   == 1);
    assert(R.get(1)     == 0);
    assert(R.get(1.5)   == 0x2020);
    assert(R.get(2)     == 0x4040);
    assert(R.get(2.5)   == 2);
  }

  // normal rainbow b-c-g-y-r-m
  {
    Rainbow R(0, 5, RAINBOW_NORMAL);
    assert(R.get(0)   == 0x0000ff);
    assert(R.get(0.5) == 0x0080ff);
    assert(R.get(1)   == 0x00ffff);
    assert(R.get(1.5) == 0x00ff7f);
    assert(R.get(2)   == 0x00ff00);
    assert(R.get(2.5) == 0x80ff00);
    assert(R.get(3)   == 0xffff00);
    assert(R.get(3.5) == 0xff7f00);
    assert(R.get(4)   == 0xff0000);
    assert(R.get(4.5) == 0xff0080);
    assert(R.get(5)   == 0xff00ff);
  }

  {
    Rainbow R(0, 5, RAINBOW_BURNING);
    assert(R.get(0)   == 0xffffff);
    assert(R.get(1)   == 0xffff00);
    assert(R.get(2)   == 0xff0000);
    assert(R.get(3)   == 0xff00ff);
    assert(R.get(4)   == 0x0000ff);
    assert(R.get(5)   == 0x000040);
  }

  {
    Rainbow R(0, 3, RAINBOW_BURNING1);
    assert(R.get(0)   == 0x000000);
    assert(R.get(1)   == 0xff0000);
    assert(R.get(2)   == 0xffff00);
    assert(R.get(3)   == 0xffffff);
  }

  {
    //opposite direction
    Rainbow R(3, 0, RAINBOW_BURNING1);
    assert(R.get(3)   == 0x000000);
    assert(R.get(2)   == 0xff0000);
    assert(R.get(1)   == 0xffff00);
    assert(R.get(0)   == 0xffffff);
  }

  {
    //string
    Rainbow R(0, 14, "RrGgBbCcMmYyKWw");
    assert(R.get(0)   == 0xff0000);
    assert(R.get(1)   == 0x400000);
    assert(R.get(2)   == 0x00ff00);
    assert(R.get(3)   == 0x004000);
    assert(R.get(4)   == 0x0000ff);
    assert(R.get(5)   == 0x000040);
    assert(R.get(6)   == 0x00ffff);
    assert(R.get(7)   == 0x004040);
    assert(R.get(8)   == 0xff00ff);
    assert(R.get(9)   == 0x400040);
    assert(R.get(10)  == 0xffff00);
    assert(R.get(11)  == 0x404000);
    assert(R.get(12)  == 0x000000);
    assert(R.get(13)  == 0xffffff);
    assert(R.get(14)  == 0x404040);
  }

  // color shade
  {
    assert(color_shade(0xFFFFFF, 0) == 0x000000);
    assert(color_shade(0xFFFFFF, 0.5) == 0x808080);
    assert(color_shade(0xFFFFFF, 1) == 0xffffff);
  }

  return 0;
}

///\endcond
