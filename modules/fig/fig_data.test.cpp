///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "fig_data.h"

int
main(){
  try {

    // static members
    assert(Fig::colors.size() == 33);
    assert(Fig::colors.find(-1)->second == 0);
    assert(Fig::colors.find(0)->second == 0);
    assert(Fig::colors.find(1)->second == 0xff0000);
    assert(Fig::colors.find(10)->second == 0xd00000);
    assert(Fig::colors.find(31)->second == 0x00d7ff);

    assert(Fig::psfonts.size() == 36);
    assert(Fig::psfonts.find(-1)->second == "Default");
    assert(Fig::psfonts.find(0)->second  == "Times-Roman");
    assert(Fig::psfonts.find(34)->second == "ZapfDingbats");

    assert(Fig::texfonts.size() == 6);
    assert(Fig::texfonts.find(0)->second == "Default");
    assert(Fig::texfonts.find(1)->second  == "Roman");
    assert(Fig::texfonts.find(5)->second == "Typewriter");

    assert(Fig::cm2fig == 1200.0 / 1.05 / 2.54);
    assert(Fig::fig2cm == 1.0 / Fig::cm2fig);
  }
  catch (Err E){
    std::cerr << "Error: " << E.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond