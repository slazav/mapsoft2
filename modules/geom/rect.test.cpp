///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "rect.h"
#include "opt/opt.h"
#include "err/assert_err.h"

void test_expand_v(const iRect & r0, const int v, const iRect & rr){
    iRect r(r0);
    assert(expand(r0,v) == rr);
    r.expand(v);
    assert(r == rr);
}

void test_expand_vv(const iRect & r0, const int v1, const int v2, const iRect & rr){
    iRect r(r0);
    assert(expand(r0,v1,v2) == rr);
    r.expand(v1,v2);
    assert(r == rr);
}

void test_expand_pt(const iRect & r0, const iPoint & p, const iRect & rr){
    iRect r(r0);
    assert(expand(r0,p) == rr);
    r.expand(p);
    assert(r == rr);
}

void test_expand_r(const iRect & r0, const iRect & r1, const iRect & rr){
    iRect r(r0);
    assert(expand(r0,r1) == rr);
    r.expand(r1);
    assert(r == rr);
}

void test_intersect(const iRect & r0, const iRect & r1, const iRect & rr){
    iRect r(r0);
    assert(intersect(r0,r1) == rr);
    r.intersect(r1);
    assert(r == rr);
}

void test_rint(const dRect & r0, const iRect & rr){
    dRect r(r0);
    assert(rint(r0) == rr);
    r.to_rint();
    assert(r == rr);
}

void test_floor(const dRect & r0, const iRect & rr){
    dRect r(r0);
    assert(floor(r0) == rr);
    r.to_floor();
    assert(r == rr);
}

void test_ceil(const dRect & r0, const iRect & rr){
    dRect r(r0);
    assert(ceil(r0) == rr);
    r.to_ceil();
    assert(r == rr);
}


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

  assert_err(r1.tlc(), "Empty rectangle in tlc call");
  assert_err(r1.trc(), "Empty rectangle in trc call");
  assert_err(r1.blc(), "Empty rectangle in blc call");
  assert_err(r1.brc(), "Empty rectangle in brc call");
  assert_err(r1.cnt(), "Empty rectangle in cnt call");

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
  assert_err(r1+=iPoint(), "Empty rectangle in operator+");
  assert_err(r1-=iPoint(), "Empty rectangle in operator-");
  assert_err(r1/=5, "Empty rectangle in operator/");
  assert_err(r1*=5, "Empty rectangle in operator*");
  assert_err(-r1, "Empty rectangle in operator-");

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

  {
    // rint, floor, ceil
    dRect dr1 = dRect(dPoint(5.8,6.8), dPoint(7.2,8.8));

    dRect dr2 = dRect(-dPoint(5.8,6.8), -dPoint(7.2,8.8));

    dRect dr1f = dRect(dPoint(6,7), dPoint(7,8));
    dRect dr2f = dRect(dPoint(-6,-7), dPoint(-8,-9));

    dRect dr1c = dRect(dPoint(6,7), dPoint(8,9));
    dRect dr2c = dRect(dPoint(-5,-6), dPoint(-7,-8));

    test_rint(dr1, dRect(dPoint(6,7), dPoint(7,9)));
    test_rint(-dr1,-dRect(dPoint(6,7), dPoint(7,9)));
    test_rint(dRect(dPoint(4.8,5.8), dPoint(5.3,9.3)), dRect(dPoint(5,6), dPoint(5,9)));
    test_rint(dRect(-dPoint(5.8,6.8), -dPoint(7.2,8.8)), -dRect(dPoint(6,7), dPoint(7,9)));
    test_rint(dRect(), dRect());

    test_floor(dr1, dRect(dPoint(6,7), dPoint(7,8)));
    test_floor(-dr1, -dRect(dPoint(6,7), dPoint(7,8)));
    test_floor(dRect(dPoint(1.1,1.1), dPoint(1.9,1.9)), dRect());
    test_floor(dRect(), dRect());

    test_ceil(dr1, dRect(dPoint(5,6), dPoint(8,9)));
    test_ceil(-dr1, -dRect(dPoint(5,6), dPoint(8,9)));
    test_ceil(dRect(), dRect());
  }

  //expand, intersect, contains
  {
    test_expand_v(iRect(1,1,2,2),  1, iRect(0,0,4,4));
    test_expand_v(iRect(1,1,2,2), -1, iRect(2,2,0,0));
    test_expand_v(iRect(1,1,2,2), -2, iRect());
    assert_err(iRect().expand(1), "Empty rectangle in expand()");
    assert_err(expand(iRect(),1), "Empty rectangle in expand()");

    test_expand_vv(iRect(1,1,2,2),  2,1, iRect(-1,0,6,4));
    test_expand_vv(iRect(1,1,2,2),  0,-1, iRect(1,2,2,0));
    test_expand_vv(iRect(1,1,2,2),  -2,0, iRect());
    assert_err(expand(iRect(),1,1), "Empty rectangle in expand()");
    assert_err(iRect().expand(1,1), "Empty rectangle in expand()");

    iPoint p0(0,0), p1(1,2), p2(2,3);
    test_expand_pt(iRect(),      p1, iRect(p1,p1));
    test_expand_pt(iRect(p0,p0), p1, iRect(p0,p1));
    test_expand_pt(iRect(p1,p2), p0, iRect(p0,p2));

    test_expand_r(iRect(),      iRect(p0,p0), iRect(p0,p0));
    test_expand_r(iRect(p0,p0),      iRect(), iRect(p0,p0));
    test_expand_r(iRect(p0,p1), iRect(p0,p0), iRect(p0,p1));
    test_expand_r(iRect(p0,p1), iRect(p1,p2), iRect(p0,p2));
    test_expand_r(iRect(p1,p2), iRect(p0,p1), iRect(p0,p2));
    test_expand_r(iRect(p1,p2), iRect(p0,p2), iRect(p0,p2));

    test_intersect(     iRect(), iRect(p0,p0), iRect());
    test_intersect(     iRect(), iRect(p0,p1), iRect());
    test_intersect(     iRect(),      iRect(), iRect());
    test_intersect(iRect(p0,p0),      iRect(), iRect());
    test_intersect(iRect(p0,p1),      iRect(), iRect());
    test_intersect(iRect(p1,p2), iRect(p0,p0), iRect());
    test_intersect(iRect(p1,p2), iRect(p0,p1), iRect(p1,p1));
    test_intersect(iRect(p1,p0), iRect(p1,p2), iRect(p1,p1));
    test_intersect(iRect(p0,p2), iRect(p1,p2), iRect(p1,p2));
    test_intersect(iRect(p0,p2), iRect(p0,p1), iRect(p0,p1));

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

  {  // dist
     dRect r1(1.2,3.2, 5.6,7.8);
     dRect r2(1.3,3.4, 10.1,10.2);
     double d = hypot(dist(r1.tlc(),r2.tlc()), dist(r1.brc(),r2.brc()));
     assert(abs(d-dist(r1,r2)) < 1e-6);
     assert(std::isinf(dist(r1,dRect())));
     assert(dist(dRect(),dRect()) == 0);
     assert(dist(r1,r1) == 0);

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

    assert_err(str_to_type<dRect>(" [ 0 , 1 "),
      "can't parse value:  [ 0 , 1 ");

    assert_err(str_to_type<dRect>("[0,1]"),
      "can't parse value: [0,1]");

    assert_err(str_to_type<dRect>("[0,1,1,1]m"),
      "can't parse value: [0,1,1,1]m");
  }

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond