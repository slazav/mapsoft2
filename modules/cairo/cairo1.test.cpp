///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "cairo_wrapper.h"

int
main(){
  try{
    CairoWrapper cw;
    cw.set_surface_svg("cairo1.svg", 150,100);

//    cw->set_source_rgb(1,0,0);
    cw->set_color(0xFF0000);
    cw->mkpath(dLine("[[10,50],[130,10],[80,90]]"), 0);
    cw->fill_preserve();
//    cw->set_source_rgba(0,0,0,0.5);
    cw->set_color_a(0x8F000000);
    cw->stroke();

    cw->save_png("cairo1.png");
  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  } 
  return 0;
}

///\endcond

