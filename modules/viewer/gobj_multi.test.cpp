///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "err/assert_err.h"
#include "gobj_multi.h"
#include "gobj_test_dots.h"
#include "gobj_test_fill.h"

int redraw_counter=0;
void inc_redraw_counter(const dRect & r){
  ++redraw_counter;
}

int
main(){
  try {

    int col0 = 0xFF000000;
    int col1 = 0xFFFF00FF;
    int col2 = 0xFF00FF00;

    // make objects
    std::shared_ptr<ConvBase> cnv(new ConvBase);
    std::shared_ptr<GObj> o1(new GObjFill(col1));
    std::shared_ptr<GObj> o2(new GObjDots(col2));
    std::shared_ptr<GObj> o3(new GObjDots(col1));

    /************************************************/
    // combine both objects
    GObjMulti o;
    o.set_cnv(cnv);
    o.signal_redraw_me().connect(sigc::ptr_fun(inc_redraw_counter));
    assert_eq(redraw_counter, 0);

    assert_eq(o.size(), 0);
    o.add(20, o1); // fill
    o.add(10, o2); // dots
    assert_eq(redraw_counter, 2);

    assert_eq(o.size(), 2);
    assert_eq(o.get_depth(o1), 20);
    assert_eq(o.get_depth(o2), 10);
    assert_err(o.get_depth(o3), "GObjMulti::get_depth: no such object");
    assert_err(o.get_depth(NULL), "GObjMulti::get_depth: no such object");

    assert_eq(o.get_visibility(o1), true);
    assert_eq(o.get_visibility(o2), true);
    assert_err(o.get_visibility(o3), "GObjMulti::get_visibility: no such object");
    assert_err(o.get_visibility(NULL), "GObjMulti::get_visibility: no such object");

    assert_eq(o.bbox(), GObj::MAX_RANGE);

    /************************************************/
    // Image filled with black color + CairoWrapper
    Image img(100,100, IMAGE_32ARGB);
    CairoWrapper cr;
    cr.set_surface_img(img);
    cr->set_color(col0);
    cr->paint();
    assert_eq(img.get32(0,0), col0);
    assert_eq(img.get32(90,90), col0);

    dRect r1 = dRect(0,0,100,100);
    o.draw(cr, r1);
//      cr->save_png("a.png");
    assert_eq(img.get32(5,5), col1);
    assert_eq(img.get32(10,10), col2);
    assert_eq(img.get32(20,20), col2);

    assert_eq(redraw_counter, 2);
    o.set_visibility(o1, false);
    assert_eq(o.get_visibility(o1), false);
    assert_eq(redraw_counter, 3);

    cr->set_color(col0);
    cr->paint();
    o.draw(cr, r1);
//      cr->save_png("a.png");
    assert_eq(img.get32(5,5), col0);
    assert_eq(img.get32(10,10), col2);
    assert_eq(img.get32(20,20), col2);

    o.set_visibility(o1, true);
    assert_eq(o.get_visibility(o1), true);
    assert_eq(redraw_counter, 4);

    cr->set_color(col0);
    cr->paint();
    o.draw(cr, r1);
//      cr->save_png("a.png");
    assert_eq(img.get32(5,5), col1);
    assert_eq(img.get32(10,10), col2);
    assert_eq(img.get32(20,20), col2);

    o.set_depth(o1, 5);
    assert_eq(redraw_counter, 5);
    assert_eq(o.get_depth(o1), 5);

    cr->set_color(col0);
    cr->paint();
    o.draw(cr, r1);
//      cr->save_png("a.png");
    assert_eq(img.get32(5,5), col1);
    assert_eq(img.get32(10,10), col1);
    assert_eq(img.get32(20,20), col1);

    o.del(o1);
    assert_eq(redraw_counter, 6);
    assert_eq(o.size(),1);

    cr->set_color(col0);
    cr->paint();
    o.draw(cr, r1);
//      cr->save_png("a.png");
    assert_eq(img.get32(5,5), col0);
    assert_eq(img.get32(10,10), col2);
    assert_eq(img.get32(20,20), col2);

    o.clear();
    assert_eq(redraw_counter, 7);
    assert_eq(o.size(),0);

    o.add(20, o1); // fill
    o.add(10, o2); // dots
    assert_eq(redraw_counter, 9);
    o.rescale(10);
    assert_eq(redraw_counter, 10);

    // check that cnv was rescaled properly
    dPoint pt(10,10);
    cnv->frw(pt);
    assert(dist2d(pt, dPoint(1,1)) < 1e-6);

  }
  catch (Err E){
    std::cerr << "Error: " << E.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond