///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "line.h"
#include "opt/opt.h"
#include "err/assert_err.h"

int
main(){
  try{

  {
    iLine l1;
    assert_eq (l1.size(), 0);
    assert_eq (l1.length(), 0);
    l1.push_back(iPoint(0,0,0));
    l1.push_back(iPoint(2,2,1));
    l1.push_back(iPoint(2,0));
    l1.push_back(iPoint(0,0,3));
    assert_eq (l1.size(), 4);

    assert_eq(l1, iLine("[[0,0],[2,2,1],[2,0],[0,0,3]]"));
    assert_eq(-l1, -iLine("[[0,0],[2,2,1],[2,0],[0,0,3]]"));
    assert_eq(-l1, iLine("[[0,0],[-2,-2,-1],[-2,0],[0,0,-3]]"));

    // +,-,*,/
    assert_eq((l1), iLine("[[0,0],[2,2,1],[2,0],[0,0,3]]"));
    assert_eq((iPoint(1,2,1)+l1), iLine("[[1,2,1],[3,4,2],[3,2,1],[1,2,4]]"));
    assert_eq((l1+iPoint(1,2,1)), iLine("[[1,2,1],[3,4,2],[3,2,1],[1,2,4]]"));
    assert_eq((l1-iPoint(1,-1,1)), iLine("[[-1,1,-1],[1,3],[1,1,-1],[-1,1,2]]"));
    assert_eq((2*l1), iLine("[[0,0],[4,4,2],[4,0,0],[0,0,6]]"));
    assert_eq((l1*2), iLine("[[0,0],[4,4,2],[4,0,0],[0,0,6]]"));
    assert_eq((l1/2), iLine("[[0,0],[1,1],[1,0,0],[0,0,1]]"));

    assert_eq((l1+=iPoint(1,2,1)), iLine("[[1,2,1],[3,4,2],[3,2,1],[1,2,4]]"));
    assert_eq((l1-=iPoint(1,1,1)), iLine("[[0,1],[2,3,1],[2,1,0],[0,1,3]]"));
    assert_eq((l1*=2), iLine("[[0,2],[4,6,2],[4,2,0],[0,2,6]]"));
    assert_eq((l1/=3), iLine("[[0,0],[1,2],[1,0,0],[0,0,2]]"));

    // <=>
    iLine l2(l1);
    assert_eq(l2,l1);
    assert(l2<=l1);
    assert(l2>=l1);
    assert(!(l2<l1));
    assert(!(l2>l1));
    assert(!(l2!=l1));
    *l1.rbegin() = iPoint(2,2);
    assert(l2!=l1);
    assert(l2<l1);
    assert(l2<=l1);
    assert(!(l2>l1));
    assert(!(l2>=l1));

    l1.resize(l1.size()-1);
    assert(l2!=l1);
    assert(l2>l1);
    assert(l2>=l1);
    assert(!(l2<l1));
    assert(!(l2<=l1));

    // is_shifted
    iLine l3("[[0,0,0],[1,2,2],[3,5,8]]");
    iPoint sh;
    assert_eq(l3.is_shifted(invert(l3), sh), false);
    assert_eq(l3.is_shifted(l3+iPoint(2,2), sh), true);
    assert_eq(sh, iPoint(2,2));
    assert_eq(l3.is_shifted(l3+iPoint(2,2), sh), is_shifted(l3, l3+iPoint(2,2), sh));

    // length
    l3 = str_to_type<iLine>("[[0,0,0],[1,2,2],[3,5,8]]");
    assert_eq(l3.size(), 3);
    assert_feq(l3.length(), 10, 1e-6);
    assert_feq(l3.length(), length(l3), 1e-6);

    // length2d
    l3 = str_to_type<iLine>("[[0,0,100],[3,4,1],[6,0,-23]]");
    assert_eq(l3.size(), 3);
    assert_feq(l3.length2d(), 10, 1e-6);
    assert_feq(l3.length2d(), length2d(l3), 1e-6);

    // bbox
    assert_eq(iLine().bbox(), iRect());
    assert_eq(l2.bbox(), iRect(0,0,1,2));
    assert_eq(l2.bbox(), bbox(l2));

    // empty
    assert_eq(iLine().is_empty(), true);
    assert_eq(iLine("[[1,2]]").is_empty(), false);
    assert_eq(iLine("[[1,2],[1,2]]").is_empty(), false);

  }

  // invert
  {
    iLine l, l0("[[1,2],[3,4,5],[4,5],[5,6]]");
    iLine li("[[5,6],[4,5],[3,4,5],[1,2]]");
    l=l0;
    assert_eq(invert(l), li);
    assert_eq(l, l0);

    l.invert();
    assert_eq(l, li);

    l.invert();
    assert_eq(l, l0);
  }

  // rotate2d
  {
    iLine l, l0("[[0,0,10],[1000,0,5]]");
    iPoint c1(0,0), c2(500,500);
    iLine lr1("[[0,0,10],[866,-499,5]]");
    iLine lr2("[[-183,316,10],[683,-183,5]]");

    double a=30*M_PI/180.0;
    l=l0;
    assert_eq(rotate2d(l,c1,a), lr1);
    assert_eq(rotate2d(l,c2,a), lr2);
    assert_eq(l,l0);
    l.rotate2d(c1,a);
    assert_eq(l,lr1);
    l=l0;
    l.rotate2d(c2,a);
    assert_eq(l,lr2);
  }

  // flatten, rint, floor, ceil, abs
  {
    dLine l, l0("[[0.1,2.8,3.1],[-0.1,-3.9,-4.6]]");
    dLine li("[[0,3,3],[0,-4,-5]]");
    dLine lc("[[1,3,4],[0,-3,-4]]");
    dLine lf("[[0,2,3],[-1,-4,-5]]");
    dLine lz("[[0.1,2.8],[-0.1,-3.9]]");
    dLine la("[[0.1,2.8,3.1],[0.1,3.9,4.6]]");

    l=l0;
    assert_deq(flatten(l), lz, 1e-6);
    assert_deq(l,l0, 1e-6);
    l.flatten();
    assert_deq(l,lz, 1e-6);

    l=l0;
    assert_eq(rint(l), iLine(li));
    assert_deq(l,l0, 1e-6);
    l.to_rint();
    assert_deq(l,li, 1e-6);

    l=l0;
    assert_eq(floor(l), iLine(lf));
    assert_deq(l,l0, 1e-6);
    l.to_floor();
    assert_deq(l,lf, 1e-6);

    l=l0;
    assert_eq(ceil(l), iLine(lc));
    assert_deq(l,l0, 1e-6);
    l.to_ceil();
    assert_deq(l,lc, 1e-6);

    l=l0;
    assert_deq(abs(l), la, 1e-6);
    assert_deq(l,l0, 1e-6);
    l.to_abs();
    assert_deq(l,la, 1e-6);

  }

  // open/close
  {
    dLine l("[[1,2],[2,3],[4,5]]");
    assert_deq(open(l), l, 1e-6);
    assert_deq(close(l), dLine("[[1,2],[2,3],[4,5],[1,2]]"), 1e-6);
    assert_deq(close(close(l)), close(l), 1e-6);
    assert_deq(open(close(l)), l, 1e-6);
    assert_deq(close(open(l)), close(l), 1e-6);

    assert_deq(open(dLine("[]")), dLine("[]"), 1e-6);
    assert_deq(close(dLine("[]")), dLine("[]"), 1e-6);
    assert_deq(open(dLine("[[1,1]]")), dLine("[[1,1]]"), 1e-6);
    assert_deq(close(dLine("[[1,1]]")), dLine("[[1,1]]"), 1e-6);
  }

  // flip_x, flip_y
  {
    iLine l1("[[1,1],[2,2],[1,1],[2,2]]");
    iLine ly("[[1,9],[2,8],[1,9],[2,8]]");
    iLine lx("[[9,1],[8,2],[9,1],[8,2]]");
    assert_eq(flip_y(l1,10), ly);

    assert_eq(flip_x(l1,10), lx);
    assert_eq(flip_y(flip_y(l1,10),10), l1);
    assert_eq(flip_x(flip_x(l1,10),10), l1);
    l1.flip_x(10);
    assert_eq(l1, lx);
    l1.flip_x(10);
    l1.flip_y(10);
    assert_eq(l1, ly);
  }

  // rect_to_line
  assert_eq(rect_to_line(iRect(1,1,2,2)), iLine("[[1,1],[3,1],[3,3],[1,3],[1,1]]"));
  assert_eq(rect_to_line(iRect(1,1,2,2), true), iLine("[[1,1],[3,1],[3,3],[1,3],[1,1]]"));
  assert_eq(rect_to_line(iRect(1,1,2,2), false), iLine("[[1,1],[3,1],[3,3],[1,3]]"));

  // dist
  assert_deq(
    dLine("[[0,0],[1,1],[2,2]]"),
    dLine("[[0,0],[1,1],[2,2]]"), 1e-6);
  assert_deq(
    dLine("[]"),
    dLine("[]"), 1e-6);
  assert(dist(
    dLine("[[0,0],[1,1],[2,2]]"),
    dLine("[[0,0],[1,1]]")) == INFINITY);
  assert(dist(
    dLine("[[0,0],[1,1]]"),
    dLine("[[0,0],[1,1],[2,2]]")) == INFINITY);
  assert_feq(dist(
    dLine("[[0,0],[1,2],[2,2]]"),
    dLine("[[0,0],[1,1],[2,2]]")), 1, 1e-6);

  // iLine <-> dLine casting
  assert_eq(dLine(str_to_type<iLine>("[[0,0,1],[2,0],[2,2]]")),
               str_to_type<dLine>("[[0,0,1],[2,0],[2,2]]"));
  assert_eq(iLine(str_to_type<dLine>("[[0.8,0.2,1.2],[2.1,0.2],[2.2,2.9]]")),
               str_to_type<iLine>("[[0,0,1],[2,0],[2,2]]"));



  // input/output
  assert_eq(type_to_str(str_to_type<iLine>("[[0,0],[2,0,3],[2,2]]")),
         "[[0,0],[2,0,3],[2,2]]");

  assert_eq(type_to_str(str_to_type<iLine>("[[0,0],[2,0],[2,2]] ")),
         "[[0,0],[2,0],[2,2]]");

  assert_eq(type_to_str(str_to_type<iLine>("[]")),
         "[]");

  assert_err(
    str_to_type<iLine>("[[0,0],[2,0],[2,2]]a"),
    "end of file expected near 'a'");

  assert_err(
    str_to_type<iLine>("[[0,0],[2,0],[2,2"),
    "']' expected near end of file");

  assert_err(
    str_to_type<iLine>("[0,0],[2,0],[2,2]"),
    "end of file expected near ','");

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
