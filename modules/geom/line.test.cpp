///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "line.h"
#include "opt/opt.h"

int
main(){
  try{

  {
    iLine l1;
    assert (l1.size() == 0);
    assert (l1.length() == 0);
    l1.push_back(iPoint(0,0,0));
    l1.push_back(iPoint(2,2,1));
    l1.push_back(iPoint(2,0));
    l1.push_back(iPoint(0,0,3));
    assert (l1.size() == 4);

    assert (l1 == iLine("[[0,0],[2,2,1],[2,0],[0,0,3]]"));
    assert (-l1 == -iLine("[[0,0],[2,2,1],[2,0],[0,0,3]]"));
    assert (-l1 == iLine("[[0,0],[-2,-2,-1],[-2,0],[0,0,-3]]"));

    // +,-,*,/
    assert((l1) == iLine("[[0,0],[2,2,1],[2,0],[0,0,3]]"));
    assert((iPoint(1,2,1)+l1) == iLine("[[1,2,1],[3,4,2],[3,2,1],[1,2,4]]"));
    assert((l1+iPoint(1,2,1)) == iLine("[[1,2,1],[3,4,2],[3,2,1],[1,2,4]]"));
    assert((l1-iPoint(1,-1,1)) == iLine("[[-1,1,-1],[1,3],[1,1,-1],[-1,1,2]]"));
    assert((2*l1) == iLine("[[0,0],[4,4,2],[4,0,0],[0,0,6]]"));
    assert((l1*2) == iLine("[[0,0],[4,4,2],[4,0,0],[0,0,6]]"));
    assert((l1/2) == iLine("[[0,0],[1,1],[1,0,0],[0,0,1]]"));

    assert((l1+=iPoint(1,2,1)) == iLine("[[1,2,1],[3,4,2],[3,2,1],[1,2,4]]"));
    assert((l1-=iPoint(1,1,1)) == iLine("[[0,1],[2,3,1],[2,1,0],[0,1,3]]"));
    assert((l1*=2) == iLine("[[0,2],[4,6,2],[4,2,0],[0,2,6]]"));
    assert((l1/=3) == iLine("[[0,0],[1,2],[1,0,0],[0,0,2]]"));

    // <=>
    iLine l2(l1);
    assert(l2==l1);
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
    assert(l3.is_shifted(invert(l3), sh) == false);
    assert(l3.is_shifted(l3+iPoint(2,2), sh) == true);
    assert(sh == iPoint(2,2));
    assert(l3.is_shifted(l3+iPoint(2,2), sh) == is_shifted(l3, l3+iPoint(2,2), sh));

    // length
    l3 = str_to_type<iLine>("[[0,0,0],[1,2,2],[3,5,8]]");
    assert (l3.size() == 3);
    assert (l3.length() == 10);
    assert (l3.length() == length(l3));

    // length2d
    l3 = str_to_type<iLine>("[[0,0,100],[3,4,1],[6,0,-23]]");
    assert (l3.size() == 3);
    assert (l3.length2d() == 10);
    assert (l3.length2d() == length2d(l3));

    // bbox2d
    assert(iLine().bbox2d() == iRect());
    assert(l2.bbox2d() == iRect(0,0,1,2));
    assert(l2.bbox2d() == bbox2d(l2));
  }

  // invert
  {
    iLine l, l0("[[1,2],[3,4,5],[4,5],[5,6]]");
    iLine li("[[5,6],[4,5],[3,4,5],[1,2]]");
    l=l0;
    assert(invert(l) == li);
    assert(l == l0);

    l.invert();
    assert(l == li);

    l.invert();
    assert(l == l0);
  }

  // rotate2d
  {
    iLine l, l0("[[0,0,10],[1000,0,5]]");
    iPoint c1(0,0), c2(500,500);
    iLine lr1("[[0,0,10],[866,-499,5]]");
    iLine lr2("[[-183,316,10],[683,-183,5]]");

    double a=30*M_PI/180.0;
    l=l0;
    assert(rotate2d(l,c1,a) == lr1);
    assert(rotate2d(l,c2,a) == lr2);
    assert(l==l0);
    l.rotate2d(c1,a);
    assert(l==lr1);
    l=l0;
    l.rotate2d(c2,a);
    assert(l==lr2);
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
    assert(flatten(l) == lz);
    assert(l==l0);
    l.flatten();
    assert(l==lz);

    l=l0;
    assert(rint(l) == li);
    assert(l==l0);
    l.to_rint();
    assert(l==li);

    l=l0;
    assert(floor(l) == lf);
    assert(l==l0);
    l.to_floor();
    assert(l==lf);

    l=l0;
    assert(ceil(l) == lc);
    assert(l==l0);
    l.to_ceil();
    assert(l==lc);

    l=l0;
    assert(abs(l) == la);
    assert(l==l0);
    l.to_abs();
    assert(l==la);

  }

  // rect_to_line
  assert(rect_to_line(iRect(1,1,2,2)) == iLine("[[1,1],[3,1],[3,3],[1,3],[1,1]]"));
  assert(rect_to_line(iRect(1,1,2,2), true)  == iLine("[[1,1],[3,1],[3,3],[1,3],[1,1]]"));
  assert(rect_to_line(iRect(1,1,2,2), false) == iLine("[[1,1],[3,1],[3,3],[1,3]]"));

  // dist
  assert(dist(
    dLine("[[0,0],[1,1],[2,2]]"),
    dLine("[[0,0],[1,1],[2,2]]")) == 0);
  assert(dist(
    dLine("[]"),
    dLine("[]")) == 0);
  assert(dist(
    dLine("[[0,0],[1,1],[2,2]]"),
    dLine("[[0,0],[1,1]]")) == INFINITY);
  assert(dist(
    dLine("[[0,0],[1,1]]"),
    dLine("[[0,0],[1,1],[2,2]]")) == INFINITY);
  assert(dist(
    dLine("[[0,0],[1,2],[2,2]]"),
    dLine("[[0,0],[1,1],[2,2]]")) == 1);

  // iLine <-> dLine casting
  assert(dLine(str_to_type<iLine>("[[0,0,1],[2,0],[2,2]]")) ==
               str_to_type<dLine>("[[0,0,1],[2,0],[2,2]]"));
  assert(iLine(str_to_type<dLine>("[[0.8,0.2,1.2],[2.1,0.2],[2.2,2.9]]")) ==
               str_to_type<iLine>("[[0,0,1],[2,0],[2,2]]"));



  // input/output
  assert(type_to_str(str_to_type<iLine>("[[0,0],[2,0,3],[2,2]]")) ==
         "[[0,0],[2,0,3],[2,2]]");

  assert(type_to_str(str_to_type<iLine>("[[0,0],[2,0],[2,2]] ")) ==
         "[[0,0],[2,0],[2,2]]");

  assert(type_to_str(str_to_type<iLine>("[]")) ==
         "[]");

  try { str_to_type<iLine>("[[0,0],[2,0],[2,2]]a"); }
  catch (Err e) {assert(e.str() == "end of file expected near 'a'");}

  try { str_to_type<iLine>("[[0,0],[2,0],[2,2"); }
  catch (Err e) {assert(e.str() == "']' expected near end of file");}

  try { str_to_type<iLine>("[0,0],[2,0],[2,2]"); }
  catch (Err e) {assert(e.str() == "end of file expected near ','");}


  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
