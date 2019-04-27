///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "line.h"
#include "opt/opt.h"

int
main(){
  try{

  iLine l1;
  assert (l1.size() == 0);
  assert (l1.length() == 0);
  l1.push_back(iPoint(0,0,0));
  l1.push_back(iPoint(2,2,1));
  l1.push_back(iPoint(2,0,2));
  l1.push_back(iPoint(0,0,3));
  assert (l1.size() == 4);

  assert (l1 == iLine("[[0,0],[2,2,1],[2,0,2],[0,0,3]]"));

  // +,-,*,/
  assert((l1) == iLine("[[0,0],[2,2,1],[2,0,2],[0,0,3]]"));
  assert((iPoint(1,2,1)+l1) == iLine("[[1,2,1],[3,4,2],[3,2,3],[1,2,4]]"));
  assert((l1+iPoint(1,2,1)) == iLine("[[1,2,1],[3,4,2],[3,2,3],[1,2,4]]"));
  assert((l1-iPoint(1,-1,1)) == iLine("[[-1,1,-1],[1,3],[1,1,1],[-1,1,2]]"));
  assert((2*l1) == iLine("[[0,0],[4,4,2],[4,0,4],[0,0,6]]"));
  assert((l1*2) == iLine("[[0,0],[4,4,2],[4,0,4],[0,0,6]]"));
  assert((l1/2) == iLine("[[0,0],[1,1],[1,0,1],[0,0,1]]"));

  assert((l1+=iPoint(1,2,1)) == iLine("[[1,2,1],[3,4,2],[3,2,3],[1,2,4]]"));
  assert((l1-=iPoint(1,1,1)) == iLine("[[0,1],[2,3,1],[2,1,2],[0,1,3]]"));
  assert((l1*=2) == iLine("[[0,2],[4,6,2],[4,2,4],[0,2,6]]"));
  assert((l1/=3) == iLine("[[0,0],[1,2],[1,0,1],[0,0,2]]"));

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

  // invert
  iLine l3 = l1.invert();
  assert(l3!=l1);
  assert(l3.is_inv(l1));
  assert(!l3.is_inv(l3));
  assert(type_to_str(l1) == "[[0,0],[1,2],[1,0,1]]");
  assert(type_to_str(l2) == "[[0,0],[1,2],[1,0,1],[0,0,2]]");
  assert(type_to_str(l3) == "[[1,0,1],[1,2],[0,0]]");
  assert(l3.invert() == invert(l3));

  // is_shifted
  iPoint sh;
  assert(l3.is_shifted(l3.invert(), sh) == false);
  assert(l3.is_shifted(l3+iPoint(2,2), sh) == true);
  assert(sh == iPoint(2,2));
  assert(l3.is_shifted(l3+iPoint(2,2), sh) == is_shifted(l3, l3+iPoint(2,2), sh));

  // length
  l3 = str_to_type<iLine>("[[0,0],[2,0],[2,2]]");
  assert (l3.size() == 3);
  assert (l3.length() == 4);
  assert (l3.length() == length(l3));

  // bbox2d
  assert(iLine().bbox2d() == iRect());
  assert(l2.bbox2d() == iRect(0,0,1,2));
  assert(l2.bbox2d() == bbox2d(l2));

  // rint
  assert(rint(dLine("[[1.1,1.8],[3.9,1.1]]")) == dLine("[[1,2],[4,1]]"));

  // flatten
  assert(flatten(iLine("[[1,2,8],[2,3,4]]")) == iLine("[[1,2],[2,3]]"));

  // rotate
  {
    iLine l1("[[0,0],[1000,0]]");
    double a=30*M_PI/180.0;
    assert(l1.rotate2d(iPoint(0,0),a) == iLine("[[0,0],[866,-499]]")); // sqrt(3)/2, -1/2
    assert(l1.rotate2d(iPoint(500,500), a) == iLine("[[-183,317],[683,-183]]"));
    assert(rotate2d(l1,iPoint(500,500), a) == iLine("[[-183,317],[683,-183]]"));
    assert(rotate2d(iLine(), iPoint(500,500), a) == iLine());

    dLine l2("[[0,0],[1000,0]]");
    assert(iLine(l2.rotate2d(dPoint(0,0),a)) == iLine("[[0,0],[866,-499]]"));
    assert(iLine(l2.rotate2d(dPoint(500,500), a)) == iLine("[[-183,316],[683,-183]]"));
    assert(iLine(rotate2d(l2,dPoint(500,500), a)) == iLine("[[-183,316],[683,-183]]"));
    assert(rotate2d(dLine(), dPoint(500,500), a) == dLine());
  }


  // rect_to_line
  assert(rect_to_line(iRect(1,1,2,2)) == iLine("[[1,1],[3,1],[3,3],[1,3],[1,1]]"));
  assert(rect_to_line(iRect(1,1,2,2), true)  == iLine("[[1,1],[3,1],[3,3],[1,3],[1,1]]"));
  assert(rect_to_line(iRect(1,1,2,2), false) == iLine("[[1,1],[3,1],[3,3],[1,3]]"));


  // iLine <-> dLine casting
  assert(dLine(str_to_type<iLine>("[[0,0],[2,0],[2,2]]")) ==
               str_to_type<dLine>("[[0,0],[2,0],[2,2]]"));
  assert(iLine(str_to_type<dLine>("[[0.8,0.2],[2.1,0.2],[2.2,2.9]]")) ==
               str_to_type<iLine>("[[0,0],[2,0],[2,2]]"));



  // input/output
  assert(type_to_str(str_to_type<iLine>("[[0,0],[2,0],[2,2]]")) ==
         "[[0,0],[2,0],[2,2]]");

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
