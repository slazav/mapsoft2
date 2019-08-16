///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "assert_err.h"

void errorfunc(){
  throw Err() << "some error";
}

void nonerrorfunc(){}


int
main(){

  assert_err(errorfunc(), "some error");
//  assert_err(errorfunc(), "some error1");
//  assert_err(nonerrorfunc(), "some error");

  assert_err(throw Err() << "eee", "eee");

}

///\endcond