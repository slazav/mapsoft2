#include <cassert>
#include <iostream>
#include "iconv/iconv.h"
#include "err/err.h"

main(){
  try{

    IConv C1("UTF8", "KOI8-R");
    assert( C1.cnv("п©я─п╦п╡п╣я┌!") == "привет!");

    try { IConv C3("UTF8", "AAA"); }
    catch (Err e) { assert(e.str() == "can't do iconv conversion from UTF8 to AAA"); }

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
  }
}
