#include <cassert>
#include <fstream>
#include "conv.h"

// Build a conversion x->10*x^2, y->10*y.
// convert a line [0,0],[1,1] with frw_acc
// and a line [0,0],[10,10]] with bck_acc.
// Use different accuracies in the conversion.
// Write results to files.
// Same source accurace parameter produce similar
// accuracy in both forward and backwart conversion.

// simple conversion x->x^2, y->y
class MyConv : public Conv {
  public:
  void frw_pt(dPoint & p) const {
    p.x*=10*p.x;
    p.y*=10;
  }
  void bck_pt(dPoint & p) const {
    p.x=sqrt(p.x)/10;
    p.y/=10;
  }
};

main(){
  try{

    MyConv cnv;

    dLine l1("[[0,0],[1,1]]");
    dLine l2("[[0,0],[10,10]]");
    std::ofstream ff1("conv.test.frw_acc1.tmp");
    std::ofstream ff2("conv.test.frw_acc2.tmp");
    double acc[] = {0.5,0.1,0.01,0.002};
    for (int i=0; i<sizeof(acc)/sizeof(double); i++) {
      std::cerr << acc[i] << "\n";
      dLine l1a = cnv.frw_acc(l1, acc[i]);
      dLine l2a = cnv.bck_acc(l2, acc[i]);
      dLine::const_iterator p;
      for (p=l1a.begin(); p!=l1a.end(); p++) ff1 << p->x << " " << p->y << "\n";
      for (p=l2a.begin(); p!=l2a.end(); p++) ff2 << p->x << " " << p->y << "\n";
      ff1 << "\n";
      ff2 << "\n";
    }

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
  }
}
