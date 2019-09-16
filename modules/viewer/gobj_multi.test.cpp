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
    std::shared_ptr<GObj> o1(new GObjFill(cnv, col1));
    std::shared_ptr<GObj> o2(new GObjDots(cnv, col2));
    std::shared_ptr<GObj> o3(new GObjDots(cnv, col1));

    /************************************************/
    // combine both objects
    GObjMulti o(cnv);
    o.signal_redraw_me().connect(sigc::ptr_fun(inc_redraw_counter));
    assert(redraw_counter == 0);

    assert(o.size() == 0);
    o.add(20, o1); // fill
    o.add(10, o2); // dots
    assert(redraw_counter == 2);

    assert(o.size() == 2);
    assert(o.get_depth(o1) == 20);
    assert(o.get_depth(o2) == 10);
    assert_err(o.get_depth(o3), "GObjMulti::get_depth: no such object");
    assert_err(o.get_depth(NULL), "GObjMulti::get_depth: no such object");

    assert(o.get_visibility(o1) == true);
    assert(o.get_visibility(o2) == true);
    assert_err(o.get_visibility(o3), "GObjMulti::get_visibility: no such object");
    assert_err(o.get_visibility(NULL), "GObjMulti::get_visibility: no such object");

    assert(o.bbox() == GObj::MAX_RANGE);

    /************************************************/
    // Image filled with black color + CairoWrapper
    Image img(100,100, IMAGE_32ARGB);
    CairoWrapper cr;
    cr.set_surface_img(img);
    cr->set_color(col0);
    cr->paint();
    assert(img.get32(0,0)   == col0);
    assert(img.get32(90,90) == col0);

    dRect r1 = dRect(0,0,100,100);
    o.draw(cr, r1);
//      cr->save_png("a.png");
    assert(img.get32(5,5) == col1);
    assert(img.get32(10,10) == col2);
    assert(img.get32(20,20) == col2);

    assert(redraw_counter == 2);
    o.set_visibility(o1, false);
    assert(o.get_visibility(o1) == false);
    assert(redraw_counter == 3);

    cr->set_color(col0);
    cr->paint();
    o.draw(cr, r1);
//      cr->save_png("a.png");
    assert(img.get32(5,5) == col0);
    assert(img.get32(10,10) == col2);
    assert(img.get32(20,20) == col2);

    o.set_visibility(o1, true);
    assert(o.get_visibility(o1) == true);
    assert(redraw_counter == 4);

    cr->set_color(col0);
    cr->paint();
    o.draw(cr, r1);
//      cr->save_png("a.png");
    assert(img.get32(5,5) == col1);
    assert(img.get32(10,10) == col2);
    assert(img.get32(20,20) == col2);

    o.set_depth(o1, 5);
    assert(redraw_counter == 5);
    assert(o.get_depth(o1) == 5);

    cr->set_color(col0);
    cr->paint();
    o.draw(cr, r1);
//      cr->save_png("a.png");
    assert(img.get32(5,5) == col1);
    assert(img.get32(10,10) == col1);
    assert(img.get32(20,20) == col1);

    o.del(o1);
    assert(redraw_counter == 6);
    assert(o.size()==1);

    cr->set_color(col0);
    cr->paint();
    o.draw(cr, r1);
//      cr->save_png("a.png");
    assert(img.get32(5,5) == col0);
    assert(img.get32(10,10) == col2);
    assert(img.get32(20,20) == col2);

    o.clear();
    assert(redraw_counter == 7);
    assert(o.size()==0);

    o.add(20, o1); // fill
    o.add(10, o2); // dots
    assert(redraw_counter == 9);
    o.rescale(10);
    assert(redraw_counter == 10);

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