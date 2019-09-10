///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include <iomanip>
#include "image_cache.h"
#include "err/assert_err.h"

int
main(){
  try{

    ImageCache icache(2);

    Image i1 = icache.get("test_data/test_fullc1.jpg");

    Image i2 = icache.get("test_data/test_fullc2.jpg");

    Image i3 = icache.get("test_data/test_fullc1.jpg");

    assert(i1.size() == iPoint(320,240));
    assert(i2.size() == iPoint(500,500));
    assert(i1.data() == i3.data());

    assert_err(icache.get("test_data/missing.jpg"),
      "image_load_jpeg: can't open file: test_data/missing.jpg");

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
