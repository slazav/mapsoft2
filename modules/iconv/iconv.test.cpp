///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "iconv.h"
#include "err/assert_err.h"

int
main(){
  try{

    // convert from utf8 to koi8-r:
    IConv C1("UTF8", "KOI8-R");
    assert( C1("п©я─п╦п╡п╣я┌!") == "привет!");

    // non-utf characters are not modified:
    assert( C1("привет!") == "привет!");

    // trivial conversion:
    IConv C2;
    assert( C2("п©я─п╦п╡п╣я┌!") == "п©я─п╦п╡п╣я┌!");

    // unknown charset:
    assert_err(IConv C3("UTF8", "AAA"),
      "can't do iconv conversion from UTF8 to AAA");

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond