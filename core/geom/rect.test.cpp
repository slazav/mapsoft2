///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "rect.h"
#include "opt/opt.h"

int
main(){
  try{

  // constructors, empty, zsize, ==, !=
  iRect r1;
  assert (r1.empty());
  assert (!r1.zsize());

  iRect r2(iPoint(0,0), iPoint(1,0));
  assert (!r2.empty());
  assert (r2.zsize());

  assert (iRect(0,0,0,0) != iRect());
  assert (iRect(iPoint(),iPoint()) == iRect(0,0,0,0));

  iRect r3(iPoint(0,0), iPoint(1,2));
  assert (!r3.empty());
  assert (!r3.zsize());

  assert (iRect(iPoint(1,2),iPoint(3,4)) == iRect(1,2,2,2));
  assert (iRect(iPoint(3,4),iPoint(1,2)) == iRect(1,2,2,2));
  assert (iRect(iPoint(3,2),iPoint(1,4)) == iRect(1,2,2,2));
  assert (iRect(0,0,-1,-1) == iRect(-1,-1,1,1));

  assert (iRect(1,2,3,4) == iRect("[1,2,3,4]"));
  assert (iRect(0,0,0,0) == iRect("[0,0,0,0]"));
  assert (iRect() == iRect("[]"));

  // swap
  r1.swap(r2);
  assert(r2.empty());
  assert(!r1.empty());

  // corners
  r1=iRect(1,2,3,4);
  assert (r1.tlc() == iPoint(1,2));
  assert (r1.trc() == iPoint(4,2));
  assert (r1.blc() == iPoint(1,6));
  assert (r1.cnt() == iPoint(2,4));
  assert (dRect(r1).cnt() == dPoint(2.5,4));

  assert (iRect(r1.tlc(),r1.brc()) == r1);
  assert (iRect(r1.trc(),r1.blc()) == r1);
  assert (iRect(r1.brc(),r1.tlc()) == r1);
  assert (iRect(r1.blc(),r1.trc()) == r1);

  r1=iRect();
  try { r1.tlc(); } catch (Err e) { assert(e.str() == "Empty rectangle in tlc call"); }
  try { r1.trc(); } catch (Err e) { assert(e.str() == "Empty rectangle in trc call"); }
  try { r1.blc(); } catch (Err e) { assert(e.str() == "Empty rectangle in blc call"); }
  try { r1.brc(); } catch (Err e) { assert(e.str() == "Empty rectangle in brc call"); }
  try { r1.cnt(); } catch (Err e) { assert(e.str() == "Empty rectangle in cnt call"); }

  // +,-,*,-
  r1=iRect(10,10, 20,20);
  assert(r1+iPoint(2,2) == iRect(12,12,20,20));
  assert(iPoint(2,2)+r1 == iRect(12,12,20,20));
  assert(r1-iPoint(2,2) == iRect(8,8,20,20));
  assert(r1/5 == iRect(2,2,4,4));
  assert(r1*2 == iRect(20,20,40,40));
  assert(2*r1 == iRect(20,20,40,40));

  assert((r1+=iPoint(2,2)) == iRect(12,12,20,20));
  assert((r1-=iPoint(2,2)) == iRect(10,10,20,20));
  assert((r1/=5) == iRect(2,2,4,4));
  assert((r1*=2) == iRect(4,4,8,8));
  assert(-r1 == iRect(-12,-12,8,8));

  r1=iRect();
  try { r1+=iPoint(); } catch (Err e) { assert(e.str() == "Empty rectangle in operator+"); }
  try { r1-=iPoint(); } catch (Err e) { assert(e.str() == "Empty rectangle in operator-"); }
  try { r1/=5; } catch (Err e) { assert(e.str() == "Empty rectangle in operator/"); }
  try { r1*=5; } catch (Err e) { assert(e.str() == "Empty rectangle in operator*"); }
  try { -r1;   } catch (Err e) { assert(e.str() == "Empty rectangle in operator-"); }


  // <=>
  r1=iRect();
  r2=iRect(0,0,0,0);
  r3=iRect(1,1,0,0);
  iRect r4(1,1,1,1);
  iRect r5(0,0,1,1);
  iRect r6(0,0,2,2);

  assert(r1 == r1);
  assert(r1 <= r1);
  assert(r1 >= r1);
  assert(!(r1 != r1));
  assert(!(r1 > r1));
  assert(!(r1 < r1));

  assert(r2 == r2);
  assert(r2 <= r2);
  assert(r2 >= r2);
  assert(!(r2 != r2));
  assert(!(r2 > r2));
  assert(!(r2 < r2));

  assert(r1 != r2);
  assert(r2 != r3);
  assert(r3 != r4);
  assert(r4 != r5);

  assert(r1 < r2);
  assert(r1 < r3);
  assert(r2 < r3);
  assert(r3 < r4);
  assert(r4 > r5);
  assert(r5 < r6);

  // cast to iRect, dRect
  assert(dRect(iRect(5,6,7,8)) == dRect(5,6,7,8));
  assert(dRect(iRect()) == dRect());
  assert(iRect(dRect(5.8,6.8,7.2,8.8)) == iRect(5,6,7,8));
  assert(iRect(dRect()) == iRect());

  // rint, floor, ceil
  dRect dr1 = dRect(dPoint(5.8,6.8),dPoint(7.2,8.8));
  dRect dr2 = dRect(-dPoint(5.8,6.8),-dPoint(7.2,8.8));
  assert(dr1.rint()  == iRect(iPoint(6,7), iPoint(7,9)));
  assert(dr1.floor() == iRect(iPoint(6,7), iPoint(7,8)));
  assert(dr1.ceil()  == iRect(iPoint(5,6), iPoint(8,9)));

  assert(dr2.rint()  == iRect(-iPoint(6,7), -iPoint(7,9)));
  assert(dr2.floor() == iRect(-iPoint(6,7), -iPoint(7,8)));
  assert(dr2.ceil()  == iRect(-iPoint(5,6), -iPoint(8,9)));

  //pump, expand, intersect, contains
  {
    assert (iRect(1,1,2,2).pump(1)    == iRect(0,0,4,4));
    assert (iRect(1,1,2,2).pump(-1)   == iRect(2,2,0,0));
    assert (iRect(1,1,2,2).pump(-2)   == iRect());
    assert (iRect(1,1,2,2).pump(1,0)  == iRect(0,1,4,2));
    assert (iRect(1,1,2,2).pump(-1,0) == iRect(2,1,0,2));
    assert (iRect(1,1,2,2).pump(-2,0) == iRect());
    assert (iRect(1,1,2,2).pump(0,1)  == iRect(1,0,2,4));
    assert (iRect(1,1,2,2).pump(0,-1) == iRect(1,2,2,0));
    assert (iRect(1,1,2,2).pump(0,-2) == iRect());
    try { iRect().pump(1); } catch (Err e) { assert(e.str() == "Empty rectangle in pump()"); }
    try { iRect().pump(1,2); } catch (Err e) { assert(e.str() == "Empty rectangle in pump()"); }

    iPoint p0(0,0), p1(1,2), p2(2,3);
    assert(iRect().expand(p1) == iRect(p1,p1));
    assert(iRect(p0,p0).expand(p1) == iRect(p0,p1));
    assert(iRect(p1,p2).expand(p0) == iRect(p0,p2));

    assert(iRect().expand(iRect(p0,p0)) == iRect(p0,p0));
    assert(iRect(p0,p0).expand(iRect()) == iRect(p0,p0));
    assert(iRect(p0,p1).expand(iRect(p0,p0)) == iRect(p0,p1));
    assert(iRect(p0,p1).expand(iRect(p1,p2)) == iRect(p0,p2));
    assert(iRect(p1,p2).expand(iRect(p0,p1)) == iRect(p0,p2));
    assert(iRect(p1,p2).expand(iRect(p0,p2)) == iRect(p0,p2));

    assert(iRect().intersect(iRect(p0,p0)) == iRect());
    assert(iRect(p1,p2).intersect(iRect()) == iRect());
    assert(iRect(p1,p2).intersect(iRect(p0,p0)) == iRect());
    assert(iRect(p1,p2).intersect(iRect(p0,p1)) == iRect(p1,p1));
    assert(iRect(p0,p1).intersect(iRect(p1,p2)) == iRect(p1,p1));
    assert(iRect(p0,p2).intersect(iRect(p1,p2)) == iRect(p1,p2));

    assert(!iRect().contains(p0));
    assert(iRect(p0,p2).contains(p0));
    assert(iRect(p0,p2).contains(p2));
    assert(iRect(p0,p2).contains(iRect(p0,p0)));
    assert(iRect(p0,p2).contains(iRect(p2,p2)));
    assert(iRect(p0,p2).contains(iRect(p1,p1)));
    assert(iRect(p0,p2).contains(iRect(p1,p2)));
    assert(iRect(p0,p2).contains(iRect(p0,p2)));
    assert(!iRect(p0,p1).contains(iRect(p0,p2)));
    assert(!iRect(p0,p1).contains(iRect(p2,p2)));
    assert(!iRect(p0,p1).contains(iRect()));
  }

  //same with separate functions
  {
    assert (pump(iRect(1,1,2,2),1)    == iRect(0,0,4,4));
    assert (pump(iRect(1,1,2,2),-1)   == iRect(2,2,0,0));
    assert (pump(iRect(1,1,2,2),-2)   == iRect());
    assert (pump(iRect(1,1,2,2),1,0)  == iRect(0,1,4,2));
    assert (pump(iRect(1,1,2,2),-1,0) == iRect(2,1,0,2));
    assert (pump(iRect(1,1,2,2),-2,0) == iRect());
    assert (pump(iRect(1,1,2,2),0,1)  == iRect(1,0,2,4));
    assert (pump(iRect(1,1,2,2),0,-1) == iRect(1,2,2,0));
    assert (pump(iRect(1,1,2,2),0,-2) == iRect());
    try { pump(iRect(),1); } catch (Err e) { assert(e.str() == "Empty rectangle in pump()"); }
    try { pump(iRect(),1,2); } catch (Err e) { assert(e.str() == "Empty rectangle in pump()"); }

    iPoint p0(0,0), p1(1,2), p2(2,3);
    assert(expand(iRect(),p1) == iRect(p1,p1));
    assert(expand(iRect(p0,p0),p1) == iRect(p0,p1));
    assert(expand(iRect(p1,p2),p0) == iRect(p0,p2));

    assert(expand(iRect(),iRect(p0,p0)) == iRect(p0,p0));
    assert(expand(iRect(p0,p0),iRect()) == iRect(p0,p0));
    assert(expand(iRect(p0,p1),iRect(p0,p0)) == iRect(p0,p1));
    assert(expand(iRect(p0,p1),iRect(p1,p2)) == iRect(p0,p2));
    assert(expand(iRect(p1,p2),iRect(p0,p1)) == iRect(p0,p2));
    assert(expand(iRect(p1,p2),iRect(p0,p2)) == iRect(p0,p2));

    assert(intersect(iRect(),iRect(p0,p0)) == iRect());
    assert(intersect(iRect(p1,p2),iRect()) == iRect());
    assert(intersect(iRect(p1,p2),iRect(p0,p0)) == iRect());
    assert(intersect(iRect(p1,p2),iRect(p0,p1)) == iRect(p1,p1));
    assert(intersect(iRect(p0,p1),iRect(p1,p2)) == iRect(p1,p1));
    assert(intersect(iRect(p0,p2),iRect(p1,p2)) == iRect(p1,p2));

    assert(!contains(iRect(),p0));
    assert(contains(iRect(p0,p2),(p0+p2)/2));
    assert(contains(iRect(p0,p2),p0));
    assert(contains(iRect(p0,p2),p2));

    assert(!contains_l(iRect(),p0));
    assert(contains_l(iRect(p0,p2),(p0+p2)/2));
    assert(contains_l(iRect(p0,p2),p0));
    assert(!contains_l(iRect(p0,p2),p2));

    assert(!contains_u(iRect(),p0));
    assert(contains_u(iRect(p0,p2),(p0+p2)/2));
    assert(!contains_u(iRect(p0,p2),p0));
    assert(contains_u(iRect(p0,p2),p2));

    assert(!contains_n(iRect(),p0));
    assert(contains_n(iRect(p0,p2),(p0+p2)/2));
    assert(!contains_n(iRect(p0,p2),p0));
    assert(!contains_n(iRect(p0,p2),p2));

    assert(contains(iRect(p0,p2),iRect(p0,p0)));
    assert(contains(iRect(p0,p2),iRect(p2,p2)));
    assert(contains(iRect(p0,p2),iRect(p1,p1)));
    assert(contains(iRect(p0,p2),iRect(p1,p2)));
    assert(contains(iRect(p0,p2),iRect(p0,p2)));
    assert(!contains(iRect(p0,p1),iRect(p0,p2)));
    assert(!contains(iRect(p0,p1),iRect(p2,p2)));
    assert(!contains(iRect(p0,p1),iRect()));
  }


/*
  // tile_cover, tile_in
  {
    assert(dRect(dPoint(1.1,2.1), dPoint(2.1,5.1)).tile_cover(2) == iRect(0,1,1,1));
    assert(dRect(dPoint(1.1,2.1), dPoint(1.8,5.1)).tile_cover(2) == iRect(0,1,0,1));
    assert(dRect(dPoint(1.1,2.1), dPoint(1.8,4)).tile_cover(2) == iRect(0,1,0,0));
    assert(dRect(dPoint(1,2), dPoint(1.8,4)).tile_cover(2) == iRect(0,1,0,0));
  }
*/

   // input/output
  {
    assert(type_to_str(dRect()) == "[]");
    assert(type_to_str(dRect(0,0,0,0)) == "[0,0,0,0]");
    assert(type_to_str(dRect(0,0,1,1)) == "[0,0,1,1]");
    assert(type_to_str(dRect(12345678,12345678,12345678,12345678))
            == "[12345678,12345678,12345678,12345678]"); // precision is set at least to 8

    assert(str_to_type<dRect>("[]") == dRect());
    assert(str_to_type<dRect>(" [ ] ") == dRect());
    assert(str_to_type<dRect>("[0,0,0,0]") == dRect(0,0,0,0));
    assert(str_to_type<dRect>(" [ 0 , 0 , 0 , 0 ] ") == dRect(0,0,0,0));
    assert(str_to_type<dRect>(" [ 1e-10 , 1e5 , 0.1 , 1.23 ] ") == dRect(1e-10,1e5,0.1,1.23));

    try { str_to_type<dRect>(" [ 0 , 1 "); }
    catch (Err e) { assert(e.str() == "can't parse value:  [ 0 , 1 "); }

    try { str_to_type<dRect>("[0,1]"); }
    catch (Err e) { assert(e.str() == "can't parse value: [0,1]"); }

    try { str_to_type<dRect>("[0,1,1,1]m"); }
    catch (Err e) { assert(e.str() == "can't parse value: [0,1,1,1]m"); }
  }

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond