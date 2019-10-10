///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "multiline.h"
#include "opt/opt.h"
#include "err/assert_err.h"

int
main(){
  try{

  iMultiLine ml1;
  iLine l1("[[0,1],[2,3]]");
  iLine l2("[[4,5],[6,7]]");
  iMultiLine ml2;
  ml2.push_back(l1);
  ml2.push_back(l2);
  assert_eq(ml1.size(), 0);
  assert_eq(ml2.size(), 2);

  assert_eq(ml2, iMultiLine("[[[0,1],[2,3]],[[4,5],[6,7]]]"));
  assert_eq(ml1, iMultiLine("[]"));

  assert_eq(-ml2, -iMultiLine("[[[0,1],[2,3]],[[4,5],[6,7]]]"));
  assert_eq(-ml2,  iMultiLine("[[[0,-1],[-2,-3]],[[-4,-5],[-6,-7]]]"));

  // reading single-segment multiline:
  assert_eq(iMultiLine("[[1,1],[2,2]]"), iMultiLine("[[[1,1],[2,2]]]"));
  assert(iMultiLine("[[]]") != iMultiLine("[]")); // exception

  // +,-,*,/
  iPoint p(1,2);
  assert_eq((ml2+p), iMultiLine("[[[1,3],[3,5]],[[5,7],[7,9]]]"));
  assert_eq((ml2-p), iMultiLine("[[[-1,-1],[1,1]],[[3,3],[5,5]]]"));
  assert_eq((ml2*2), iMultiLine("[[[0,2],[4,6]],[[8,10],[12,14]]]"));
  assert_eq((ml2/2), iMultiLine("[[[0,0],[1,1]],[[2,2],[3,3]]]"));

  assert_eq((ml2+=p), iMultiLine("[[[1,3],[3,5]],[[5,7],[7,9]]]"));
  assert_eq((ml2-=p), iMultiLine("[[[0,1],[2,3]],[[4,5],[6,7]]]"));
  assert_eq((ml2*=2), iMultiLine("[[[0,2],[4,6]],[[8,10],[12,14]]]"));
  assert_eq((ml2/=2), iMultiLine("[[[0,1],[2,3]],[[4,5],[6,7]]]"));

  // <=>
  assert_eq(ml1, iMultiLine());
  ml1=ml2; // Multilines are equal
  assert_eq(  ml1,ml2);
  assert(!(ml1!=ml2));
  assert(ml1 >= ml2);
  assert(ml1 <= ml2);
  assert(!(ml1 > ml2));
  assert(!(ml1 < ml2));
  assert(ml1.operator==(ml2));
  assert(!(ml1.operator!=(ml2)));
  assert(ml1.operator>=(ml2));
  assert(ml1.operator<=(ml2));
  assert(!(ml1.operator>(ml2)));
  assert(!(ml1.operator<(ml2)));
  *(ml1.rbegin()->rbegin()) = iPoint(10,10); // Last segment of ml1 shorter
  assert(ml1 !=  ml2);
  assert(ml1 >  ml2);
  assert(ml1 >= ml2);
  assert(ml2 <  ml1);
  assert(ml2 <= ml1);
  ml1.rbegin()->resize(ml1.rbegin()->size()-1); // ml2 shorter
  assert(ml1 !=  ml2);
  assert(ml1 <  ml2);
  assert(ml1 <= ml2);
  assert(ml2 >  ml1);
  assert(ml2 >= ml1);
  ml2.resize(ml2.size()-1);
  assert(ml1 !=  ml2);
  assert(ml1 >  ml2);
  assert(ml1 >= ml2);
  assert(ml2 <  ml1);
  assert(ml2 <= ml1);

  // length, bbox, rint
  ml2.clear();
  ml1.clear();
  ml2.push_back(l1);
  ml2.push_back(l2);
  assert_eq(ml1.length(), 0);
  assert_feq(ml2.length(), l1.length() + l2.length(), 1e-4);
  assert_feq(length(ml2), length(l1) + length(l2), 1e-4);

  assert_eq(ml1.bbox(), iRect());
  assert_eq(ml2.bbox(), expand(l1.bbox(),l2.bbox()));
  assert_eq(bbox(ml2), ml2.bbox());

  assert_eq(dMultiLine("[]").is_empty(), true);
  assert_eq(dMultiLine("[[],[]]").is_empty(), true);
  assert_eq(dMultiLine("[[],[[1,2]]]").is_empty(), false);
  assert_eq(dMultiLine("[[[1,2]]]").is_empty(), false);

  {
     dMultiLine ml1("[ [[0,0,0], [1,2,2]], [[0,0,0], [1,2,-2], [2,0,0]] ]");
     assert_eq(ml1.length(), 9);

     dMultiLine ml2("[ [[0,0,0], [3,4,2]], [[0,0,0], [3,4,-2], [6,0,0]] ]");
     assert_eq(ml2.length2d(), 15);
  }

  assert_eq(rint(dMultiLine("[[[1.1,1.8],[3.9,1.1]],[]]")), dMultiLine("[[[1,2],[4,1]],[]]"));
  assert_eq(flatten(iMultiLine("[[[1,8,9],[1,2,3]],[]]")), iMultiLine("[[[1,8],[1,2]],[]]"));

  // rotate2d
  { 
    iMultiLine l, l0("[[[0,0,10],[1000,0,5]], [], [[0,0]]]");
    iPoint c1(0,0), c2(500,500);
    iMultiLine lr1("[[[0,0,10],[866,-499,5]], [], [[0,0]]]");
    iMultiLine lr2("[[[-183,316,10],[683,-183,5]], [], [[-183,316]]]");

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
    dMultiLine l, l0("[[[0.1,2.8,3.1],[-0.1,-3.9,-4.6]], [], [[0,0]]]");
    dMultiLine li("[[[0,3,3],[0,-4,-5]], [], [[0,0]]]");
    dMultiLine lc("[[[1,3,4],[0,-3,-4]], [], [[0,0]]]");
    dMultiLine lf("[[[0,2,3],[-1,-4,-5]], [], [[0,0]]]");
    dMultiLine lz("[[[0.1,2.8],[-0.1,-3.9]], [], [[0,0]]]");
    dMultiLine la("[[[0.1,2.8,3.1],[0.1,3.9,4.6]], [], [[0,0]]]");

    l=l0;
    assert_eq(flatten(l), lz);
    assert_eq(l,l0);
    l.flatten();
    assert_eq(l,lz);

    l=l0;
    assert_eq(rint(l), li);
    assert_eq(l,l0);
    l.to_rint();
    assert_eq(l,li);

    l=l0;
    assert_eq(floor(l), lf);
    assert_eq(l,l0);
    l.to_floor();
    assert_eq(l,lf);

    l=l0;
    assert_eq(ceil(l), lc);
    assert_eq(l,l0);
    l.to_ceil();
    assert_eq(l,lc);

    l=l0;
    assert_eq(abs(l), la);
    assert_eq(l,l0);
    l.to_abs();
    assert_eq(l,la);

  }

  // open/close
  {
    dMultiLine l("[[[1,2],[2,3],[4,5]],[[1,1]],[],[[1,1],[2,2]]]");
    assert_eq(open(l), l);
    assert_eq(close(l), dMultiLine("[[[1,2],[2,3],[4,5],[1,2]],[[1,1]],[],[[1,1],[2,2],[1,1]]]"));
    assert_eq(close(close(l)), close(l));
    assert_eq(open(close(l)), l);
    assert_eq(close(open(l)), close(l));
  }

  // flip_x, flip_y
  {
    iMultiLine l1("[[[1,1],[2,2]],[[1,1],[2,2]]]");
    iMultiLine ly("[[[1,9],[2,8]],[[1,9],[2,8]]]");
    iMultiLine lx("[[[9,1],[8,2]],[[9,1],[8,2]]]");
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

  // dist
  assert_eq(dist(
    dMultiLine("[ [[0,0],[1,1],[2,2]], [[1,1]], [] ]"),
    dMultiLine("[ [[0,0],[1,1],[2,2]], [[1,1]], [] ]")), 0);
  assert(dist(
    dMultiLine("[]"),
    dMultiLine("[]")) == 0);
  assert(dist(
    dMultiLine("[[[0,0],[1,1],[2,2]],[]]"),
    dMultiLine("[[[0,0],[1,1]],[]]")) == INFINITY);
  assert(dist(
    dMultiLine("[[[0,0],[1,1]]]"),
    dMultiLine("[[[0,0],[1,1]],[]]")) == INFINITY);
  assert(dist(
    dMultiLine("[[[0,0],[1,1]],[]]"),
    dMultiLine("[[[0,0],[1,1]]]")) == INFINITY);
  assert(dist(
    dMultiLine("[ [[0,0],[1,4],[2,2]], [[1,1]], []]"),
    dMultiLine("[ [[0,0],[1,1],[2,2]], [[1,5]], []]")) == 5);

  // iLine <-> dLine casting
  assert_eq(dMultiLine(str_to_type<iMultiLine>("[[[0,0],[2,0],[2,2]],[]]")),
                    str_to_type<dMultiLine>("[[[0,0],[2,0],[2,2]],[]]"));
  assert_eq(iMultiLine(str_to_type<dMultiLine>("[[[0.8,0.2],[2.1,0.2],[2.2,2.9]],[]]")),
                    str_to_type<iMultiLine>("[[[0,0],[2,0],[2,2]],[]]"));

  // input/output
  assert_eq(type_to_str(str_to_type<iMultiLine>("[[[0,0],[2,0],[2,2]],[]]")),
         "[[[0,0],[2,0],[2,2]],[]]");

  assert_eq(type_to_str(str_to_type<iMultiLine>("[[[0,0],[2,0],[2,2]],[]] ")),
         "[[[0,0],[2,0],[2,2]],[]]");

  assert_eq(type_to_str(str_to_type<iMultiLine>("[]")),
         "[]");

  assert_eq(type_to_str(str_to_type<iMultiLine>("[[]]")),
         "[[]]");
  assert_eq(type_to_str(str_to_type<iMultiLine>("[[],[]]")),
         "[[],[]]");

  assert_err(
    str_to_type<iMultiLine>("[[[0,0],[2,0],[2,2]]]a"),
    "end of file expected near 'a'");

  assert_err(
    str_to_type<iMultiLine>("[[[0,0],[2,0],[2,2"),
    "']' expected near end of file");

  assert_err(
    str_to_type<iMultiLine>("[0,0],[2,0],[2,2]"),
    "end of file expected near ','");

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond