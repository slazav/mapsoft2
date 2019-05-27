///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "multiline.h"
#include "opt/opt.h"

int
main(){
  try{

  iMultiLine ml1;
  iLine l1("[[0,1],[2,3]]");
  iLine l2("[[4,5],[6,7]]");
  iMultiLine ml2;
  ml2.push_back(l1);
  ml2.push_back(l2);
  assert(ml1.size() == 0);
  assert(ml2.size() == 2);

  assert(ml2 == iMultiLine("[[[0,1],[2,3]],[[4,5],[6,7]]]"));
  assert(ml1 == iMultiLine("[]"));

  assert(-ml2 == -iMultiLine("[[[0,1],[2,3]],[[4,5],[6,7]]]"));
  assert(-ml2 ==  iMultiLine("[[[0,-1],[-2,-3]],[[-4,-5],[-6,-7]]]"));

  // +,-,*,/
  iPoint p(1,2);
  assert((ml2+p) == iMultiLine("[[[1,3],[3,5]],[[5,7],[7,9]]]"));
  assert((ml2-p) == iMultiLine("[[[-1,-1],[1,1]],[[3,3],[5,5]]]"));
  assert((ml2*2) == iMultiLine("[[[0,2],[4,6]],[[8,10],[12,14]]]"));
  assert((ml2/2) == iMultiLine("[[[0,0],[1,1]],[[2,2],[3,3]]]"));

  assert((ml2+=p) == iMultiLine("[[[1,3],[3,5]],[[5,7],[7,9]]]"));
  assert((ml2-=p) == iMultiLine("[[[0,1],[2,3]],[[4,5],[6,7]]]"));
  assert((ml2*=2) == iMultiLine("[[[0,2],[4,6]],[[8,10],[12,14]]]"));
  assert((ml2/=2) == iMultiLine("[[[0,1],[2,3]],[[4,5],[6,7]]]"));

  // <=>
  assert(ml1 == iMultiLine());
  ml1=ml2; // Multilines are equal
  assert(  ml1==ml2);
  assert(!(ml1!=ml2));
  assert(ml1 >= ml2);
  assert(ml1 <= ml2);
  assert(!(ml1 > ml2));
  assert(!(ml1 < ml2));
  assert(  ml1.operator==(ml2));
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
  assert(ml1.length() == 0);
  assert(ml2.length() == l1.length()+l2.length());
  assert(length(ml2) == l1.length()+l2.length());

  assert(ml1.bbox2d() == iRect());
  assert(ml2.bbox2d() == expand(l1.bbox2d(),l2.bbox2d()));
  assert(bbox2d(ml2) == ml2.bbox2d());

  {
     dMultiLine ml1("[ [[0,0,0], [1,2,2]], [[0,0,0], [1,2,-2], [2,0,0]] ]");
     assert(ml1.length() == 9);

     dMultiLine ml2("[ [[0,0,0], [3,4,2]], [[0,0,0], [3,4,-2], [6,0,0]] ]");
     assert(ml2.length2d() == 15);
  }

  assert(rint(dMultiLine("[[[1.1,1.8],[3.9,1.1]],[]]")) == dMultiLine("[[[1,2],[4,1]],[]]"));
  assert(flatten(iMultiLine("[[[1,8,9],[1,2,3]],[]]")) == iMultiLine("[[[1,8],[1,2]],[]]"));

  // rotate2d
  { 
    iMultiLine l, l0("[[[0,0,10],[1000,0,5]], [], [[0,0]]]");
    iPoint c1(0,0), c2(500,500);
    iMultiLine lr1("[[[0,0,10],[866,-499,5]], [], [[0,0]]]");
    iMultiLine lr2("[[[-183,316,10],[683,-183,5]], [], [[-183,316]]]");

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
    dMultiLine l, l0("[[[0.1,2.8,3.1],[-0.1,-3.9,-4.6]], [], [[0,0]]]");
    dMultiLine li("[[[0,3,3],[0,-4,-5]], [], [[0,0]]]");
    dMultiLine lc("[[[1,3,4],[0,-3,-4]], [], [[0,0]]]");
    dMultiLine lf("[[[0,2,3],[-1,-4,-5]], [], [[0,0]]]");
    dMultiLine lz("[[[0.1,2.8],[-0.1,-3.9]], [], [[0,0]]]");
    dMultiLine la("[[[0.1,2.8,3.1],[0.1,3.9,4.6]], [], [[0,0]]]");

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

  // iLine <-> dLine casting
  assert(dMultiLine(str_to_type<iMultiLine>("[[[0,0],[2,0],[2,2]],[]]")) ==
                    str_to_type<dMultiLine>("[[[0,0],[2,0],[2,2]],[]]"));
  assert(iMultiLine(str_to_type<dMultiLine>("[[[0.8,0.2],[2.1,0.2],[2.2,2.9]],[]]")) ==
                    str_to_type<iMultiLine>("[[[0,0],[2,0],[2,2]],[]]"));

  // input/output
  assert(type_to_str(str_to_type<iMultiLine>("[[[0,0],[2,0],[2,2]],[]]")) ==
         "[[[0,0],[2,0],[2,2]],[]]");

  assert(type_to_str(str_to_type<iMultiLine>("[[[0,0],[2,0],[2,2]],[]] ")) ==
         "[[[0,0],[2,0],[2,2]],[]]");

  assert(type_to_str(str_to_type<iMultiLine>("[]")) ==
         "[]");

  assert(type_to_str(str_to_type<iMultiLine>("[[]]")) ==
         "[[]]");
  assert(type_to_str(str_to_type<iMultiLine>("[[],[]]")) ==
         "[[],[]]");

  try { str_to_type<iMultiLine>("[[[0,0],[2,0],[2,2]]]a"); }
  catch (Err e) {assert(e.str() == "end of file expected near 'a'");}

  try { str_to_type<iMultiLine>("[[[0,0],[2,0],[2,2"); }
  catch (Err e) {assert(e.str() == "']' expected near end of file");}

  try { str_to_type<iMultiLine>("[0,0],[2,0],[2,2]"); }
  catch (Err e) {assert(e.str() == "end of file expected near ','");}


  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond