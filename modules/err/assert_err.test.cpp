///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "assert_err.h"

void errorfunc(){
  throw Err() << "some error";
}

void nonerrorfunc(){}

double dist(double v1, double v2){return fabs(v1-v2);}

int
main(){
  try{
    assert_err(errorfunc(), "some error");

//  assert_err(errorfunc(), "some error1");
//  assert_err(nonerrorfunc(), "some error");

    assert_err(throw Err() << "eee", "eee");

    assert_eq(10, 10);
    assert_eq(20-10, 10);
    assert_eq(10, 20-10);

    assert_feq(10.0, 10.0, 1e-6);
    assert_feq(10.1, 10.2, 0.2);
    assert_feq(10.2, 10.1, 0.2);

    assert_feq(1, 2-1, 1e-6);
    assert_feq(2-1, 1, 1e-6);

    // using dist() function
    assert_deq(10.0, 10.0, 1e-6);
    assert_deq(10.1, 10.2, 0.2);
    assert_deq(10.2, 10.1, 0.2);

    assert_deq(1, 2-1, 1e-6);
    assert_deq(2-1, 1, 1e-6);



  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond