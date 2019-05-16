///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "multiline.h"
#include "opt/opt.h"

int
main(){
  try{

  iMultiLine ml1;
  iLine l1 = str_to_type<iLine>("[[0,1],[2,3]]");
  iLine l2 = str_to_type<iLine>("[[4,5],[6,7]]");
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
  assert(!(ml1>ml2));
  assert(!(ml1<ml2));
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

  assert(rint(dMultiLine("[[[1.1,1.8],[3.9,1.1]],[]]")) == dMultiLine("[[[1,2],[4,1]],[]]"));

  assert(flatten(iMultiLine("[[[1,8,9],[1,2,3]],[]]")) == iMultiLine("[[[1,8],[1,2]],[]]"));

  // rotate
  {
    iMultiLine l("[[],[[0,0],[1000,0]]]");
    double a=30*M_PI/180.0;
    assert(l.rotate2d(iPoint(0,0),a) == iMultiLine("[[],[[0,0],[866,-499]]]"));
    assert(l.rotate2d(iPoint(500,500), a) == iMultiLine("[[],[[-183,317],[683,-183]]]"));
    assert(rotate2d(l,iPoint(500,500), a) == iMultiLine("[[],[[-183,317],[683,-183]]]"));
    assert(iMultiLine(l.rotate2d(dPoint(500,500), a)) == iMultiLine("[[],[[-183,317],[683,-183]]]"));
    assert(rotate2d(iMultiLine(), iPoint(500,500), a) == iMultiLine());
    assert(rotate2d(iMultiLine("[[],[]]"), iPoint(500,500), a) == iMultiLine("[[],[]]"));
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