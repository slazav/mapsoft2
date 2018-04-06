#include <cassert>
#include "point.h"

main(){
  iPoint p1, p2(1,1), p3(-1,-2);

  assert(p1==iPoint(0,0));
  p1.swap(p2);
  assert(p1==iPoint(1,1));
  p1+=p1;
  assert(p1==iPoint(2,2));
  p1-=p1;
  assert(p1==iPoint(0,0));

  assert(iPoint(1,2) + iPoint(2,3) == iPoint(3,5));
  assert(iPoint(3,4) - iPoint(1,-1) == iPoint(2,5));
  assert(iPoint(3,4)*2 == iPoint(6,8));
  assert(iPoint(3,4)/2 == iPoint(1,2));

  assert(dPoint(1,2) + dPoint(2,3) == dPoint(3,5));
  assert(dPoint(3,4) - dPoint(1,-1) == dPoint(2,5));
  assert(dPoint(3,4)*2 == dPoint(6,8));
  assert(dPoint(3,4)/2 == dPoint(1.5,2));

  assert(-iPoint(1,2) == iPoint(-1,-2));

  assert(iPoint(0,0)==iPoint(0,0));
  assert(iPoint(0,0)!=iPoint(1,1));
}
