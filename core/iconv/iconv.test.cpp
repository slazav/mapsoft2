///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "iconv/iconv.h"
#include "err/err.h"

int
main(){
  try{

    // convert from utf8 to koi8-r:
    IConv C1("UTF8", "KOI8-R");
    assert( C1.cnv("п©я─п╦п╡п╣я┌!") == "привет!");

    // non-utf characters are not modified:
    assert( C1.cnv("привет!") == "привет!");

    // unknown charset:
    try { IConv C3("UTF8", "AAA"); }
    catch (Err e) { assert(e.str() == "can't do iconv conversion from UTF8 to AAA"); }

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond