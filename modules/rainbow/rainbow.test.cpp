///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "rainbow.h"

#include <iostream>
#include <iomanip>

int
main(){

/************************************************/
// user-defined rainbow data
  std::vector<rainbow_data> RD={
    {0.1, 0x000000},
    {0.5, 0xFF0000}, // 0.1 - 0.5 black -> blue
    {0.5, 0xFF00FF}, // - color step
    {0.9, 0x000000}, // 0.5 - 0.9 magenta -> black
  };

  // array size = 0 -> always return 0
  assert(get_rainbow(1.2, RD, 0) == 0);

  assert(get_rainbow(0.0, RD) == 0);
  assert(get_rainbow(0.2, RD) == 0x400000);
  assert(get_rainbow(0.4999, RD) == 0xFF0000);
  assert(get_rainbow(0.5000, RD) == 0xFF0000);
  assert(get_rainbow(0.5001, RD) == 0xFF00FF);
  assert(get_rainbow(0.9, RD) == 0);
  assert(get_rainbow(1.0, RD) == 0);

  assert(get_rainbow(0.0, RD, 0x111111) == 0x111111);
  assert(get_rainbow(1.0, RD, 0x111111) == 0);

  assert(get_rainbow(0.0, RD, 0x111111, 0x222222) == 0x111111);
  assert(get_rainbow(1.0, RD, 0x111111, 0x222222) == 0x222222);

/************************************************/
// same tests but opposite sorting of the data

  std::vector<rainbow_data> RD1={
    {0.9, 0x000000},
    {0.5, 0xFF00FF},
    {0.5, 0xFF0000},
    {0.1, 0x000000},
  };

  // array size = 0 -> always return 0
  assert(get_rainbow(1.2, RD1, 0) == 0);

  assert(get_rainbow(0.0, RD1) == 0);
  assert(get_rainbow(0.2, RD1) == 0x400000);
  assert(get_rainbow(0.4999, RD1) == 0xFF0000);
//step point is different!
  assert(get_rainbow(0.5000, RD1) == 0xFF00FF);
  assert(get_rainbow(0.5001, RD1) == 0xFF00FF);
  assert(get_rainbow(0.9, RD1) == 0);
  assert(get_rainbow(1.0, RD1) == 0);

  assert(get_rainbow(0.0, RD1, 0x111111) == 0x111111);
  assert(get_rainbow(1.0, RD1, 0x111111) == 0);

  assert(get_rainbow(0.0, RD1, 0x111111, 0x222222) == 0x111111);
  assert(get_rainbow(1.0, RD1, 0x111111, 0x222222) == 0x222222);

/************************************************/
// simple_rainbow

  simple_rainbow sr1(1, 2, 0x00, 0x4040);
  assert(sr1.get(0)   == 0x0000);
  assert(sr1.get(1)   == 0x0000);
  assert(sr1.get(1.5) == 0x2020);
  assert(sr1.get(2)   == 0x4040);

  assert(sr1.get(0, 1,2)   == 1);
  assert(sr1.get(1, 1,2)   == 0);
  assert(sr1.get(1.0001, 1,2)   == 0);
  assert(sr1.get(2, 1,2)   == 0x4040);
  assert(sr1.get(2.0001, 1,2)   == 2);
  assert(sr1.get(3, 1,2)   == 2);

  // normal rainbow b-c-g-y-r-m
  simple_rainbow sr2(0, 5, RAINBOW_NORMAL);
  assert(sr2.get(0)   == 0x0000ff);
  assert(sr2.get(0.5) == 0x0080ff);
  assert(sr2.get(1)   == 0x00ffff);
  assert(sr2.get(1.5) == 0x00ff7f);
  assert(sr2.get(2)   == 0x00ff00);
  assert(sr2.get(2.5) == 0x80ff00);
  assert(sr2.get(3)   == 0xffff00);
  assert(sr2.get(3.5) == 0xff7f00);
  assert(sr2.get(4)   == 0xff0000);
  assert(sr2.get(4.5) == 0xff0080);
  assert(sr2.get(5)   == 0xff00ff);

  simple_rainbow sr3(0, 5, RAINBOW_BURNING);
  assert(sr3.get(0)   == 0xffffff);
  assert(sr3.get(1)   == 0xffff00);
  assert(sr3.get(2)   == 0xff0000);
  assert(sr3.get(3)   == 0xff00ff);
  assert(sr3.get(4)   == 0x0000ff);
  assert(sr3.get(5)   == 0x000040);

  simple_rainbow sr4(0, 3, RAINBOW_BURNING1);
  assert(sr4.get(0)   == 0x000000);
  assert(sr4.get(1)   == 0xff0000);
  assert(sr4.get(2)   == 0xffff00);
  assert(sr4.get(3)   == 0xffffff);

  //opposite direction
  simple_rainbow sr5(3, 0, RAINBOW_BURNING1);
  assert(sr5.get(3)   == 0x000000);
  assert(sr5.get(2)   == 0xff0000);
  assert(sr5.get(1)   == 0xffff00);
  assert(sr5.get(0)   == 0xffffff);

  //string
  simple_rainbow sr6(0, 14, "RrGgBbCcMmYyKWw");
  assert(sr6.get(0)   == 0xff0000);
  assert(sr6.get(1)   == 0x400000);
  assert(sr6.get(2)   == 0x00ff00);
  assert(sr6.get(3)   == 0x004000);
  assert(sr6.get(4)   == 0x0000ff);
  assert(sr6.get(5)   == 0x000040);
  assert(sr6.get(6)   == 0x00ffff);
  assert(sr6.get(7)   == 0x004040);
  assert(sr6.get(8)   == 0xff00ff);
  assert(sr6.get(9)   == 0x400040);
  assert(sr6.get(10)  == 0xffff00);
  assert(sr6.get(11)  == 0x404000);
  assert(sr6.get(12)  == 0x000000);
  assert(sr6.get(13)  == 0xffffff);
  assert(sr6.get(14)  == 0x404040);

  assert(color_shade(0xFFFFFF, 0) == 0x000000);
  assert(color_shade(0xFFFFFF, 0.5) == 0x808080);
  assert(color_shade(0xFFFFFF, 1) == 0xffffff);

  return 0;
}

///\endcond
