#include <cassert>
#include "line.h"
#include "opt/opt.h"

main(){
  try{

  iLine l1;
  assert (l1.size() == 0);
  assert (l1.length() == 0);
  l1.push_back(iPoint(0,0));
  l1.push_back(iPoint(2,2));
  l1.push_back(iPoint(2,0));
  l1.push_back(iPoint(0,0));
  assert (l1.size() == 4);

  assert (l1 == iLine("[[0,0],[2,2],[2,0],[0,0]]"));

  // +,-,*,/
  assert((l1) == iLine("[[0,0],[2,2],[2,0],[0,0]]"));
  assert((iPoint(1,2)+l1) == iLine("[[1,2],[3,4],[3,2],[1,2]]"));
  assert((l1+iPoint(1,2)) == iLine("[[1,2],[3,4],[3,2],[1,2]]"));
  assert((l1-iPoint(1,-1)) == iLine("[[-1,1],[1,3],[1,1],[-1,1]]"));
  assert((2*l1) == iLine("[[0,0],[4,4],[4,0],[0,0]]"));
  assert((l1*2) == iLine("[[0,0],[4,4],[4,0],[0,0]]"));
  assert((l1/2) == iLine("[[0,0],[1,1],[1,0],[0,0]]"));

  assert((l1+=iPoint(1,2)) == iLine("[[1,2],[3,4],[3,2],[1,2]]"));
  assert((l1-=iPoint(1,1)) == iLine("[[0,1],[2,3],[2,1],[0,1]]"));
  assert((l1*=2) == iLine("[[0,2],[4,6],[4,2],[0,2]]"));
  assert((l1/=3) == iLine("[[0,0],[1,2],[1,0],[0,0]]"));

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
  assert(type_to_str(l1) == "[[0,0],[1,2],[1,0]]");
  assert(type_to_str(l2) == "[[0,0],[1,2],[1,0],[0,0]]");
  assert(type_to_str(l3) == "[[1,0],[1,2],[0,0]]");
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

  // bbox
  assert(iLine().bbox() == iRect());
  assert(l2.bbox() == iRect(0,0,1,2));
  assert(l2.bbox() == bbox(l2));

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
  }
}
