///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "err/assert_err.h"
#include "gobj_test_dots.h"

int
main(){
  try {

    int col1 = 0xFFFF00FF;
    int col2 = 0xFF000000;

    GObjDots o1;
    assert_eq(o1.bbox(), GObj::MAX_RANGE);

    // Image filled with 0xFFFF00FF color + CairoWrapper
    Image img(100,100, IMAGE_32ARGB);
    CairoWrapper cr;
    cr.set_surface_img(img);
    cr->set_color(col1);
    cr->paint();

    assert_eq(img.get32(0,0), col1);
    assert_eq(img.get32(90,90), col1);

    dRect r1 = dRect(8,8,24,24);
    cr->save();
    cr->translate(-r1.tlc());
    o1.draw(cr, r1);
//    cr->save_png("a.png");

    assert_eq(img.get32(7,7), col1);
    assert_eq(img.get32(2,2), col2);
    assert_eq(img.get32(12,12), col2);
    assert_eq(img.get32(22,12), col2);
    assert_eq(img.get32(32,12), col1);
    assert_eq(img.get32(17,17), col1);
    cr->restore();

    o1.rescale(2.0); // now dots should be drawn in 20x20 pt grid
    r1 = dRect(16,16,48,48); // same range of object coords
    cr->set_color(col1);
    cr->paint();
    cr->save();
    cr->translate(-r1.tlc());
    o1.draw(cr, r1);
//    cr->save_png("a.png");
    assert_eq(img.get32(14,14), col1);
    assert_eq(img.get32(4,4), col2);
    assert_eq(img.get32(24,24), col2);
    assert_eq(img.get32(44,24), col2);
    assert_eq(img.get32(64,24), col1);
    assert_eq(img.get32(34,34), col1);

    cr->restore();

  }
  catch (Err E){
    std::cerr << "Error: " << E.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond