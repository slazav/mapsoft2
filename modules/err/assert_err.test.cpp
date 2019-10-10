///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "assert_err.h"

void errorfunc(){
  throw Err() << "some error";
}

void nonerrorfunc(){}


int
main(){
  try{
    assert_err(errorfunc(), "some error");

//  assert_err(errorfunc(), "some error1");
//  assert_err(nonerrorfunc(), "some error");

    assert_err(throw Err() << "eee", "eee");

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond