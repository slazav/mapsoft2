#include "simple_viewer.h"

#include <cassert>
#include "image/image.h"
#include "geom/rect.h"

SimpleViewer::SimpleViewer(GObj * o) :
    obj(o),
    origin(iPoint(0,0)),
    bgcolor(0xFF000000),
    sc(1.0),
    on_drag(false) {

  // which events we want to recieve
  set_events (
    Gdk::BUTTON_PRESS_MASK |
    Gdk::BUTTON_RELEASE_MASK |
    Gdk::SCROLL_MASK |
    Gdk::POINTER_MOTION_MASK |
    Gdk::POINTER_MOTION_HINT_MASK );

  // connect signal from the object
  obj->signal_redraw_me().connect(
    sigc::mem_fun(this, &SimpleViewer::redraw));

  // suppress default themed drawing of the widget's background
  set_app_paintable();
}

/***********************************************************/

void
SimpleViewer::set_origin (iPoint p) {
  if (!obj) return;

  iRect r = range();
  int w=get_width();
  int h=get_height();

  if (obj->get_xloop()){
    p.x = p.x % r.w;
  }
  else {
    if (p.x + w >= r.x + r.w) p.x = r.x + r.w - w - 1;
    if (p.x < r.x) p.x=r.x;
  }

  if (obj->get_yloop()){
    p.y = p.y % r.h;
  }
  else {
    if (p.y + h >= r.y + r.h) p.y = r.y+ r.h - h - 1;
    if (p.y < r.y) p.y=r.y;
  }

  // now win->scroll invalidates the whole window,
  // see Scrolling section in
  //   https://developer.gnome.org/gtk3/stable/chap-drawing-model.html
  auto win = get_window();
  if (win) win->scroll(origin.x-p.x, origin.y-p.y);

  origin = p;
  signal_ch_origin_.emit(p);
}

/***********************************************************/

void
SimpleViewer::redraw (const iRect & range){
  if (range) queue_draw_area(range.x, range.y, range.w, range.h);
  else queue_draw();
}

void
SimpleViewer::rescale(const double k, const iPoint & cnt){
  if (!obj) return;
  signal_on_rescale_.emit(k);
  iPoint wsize(get_width(), get_height());
  iPoint wcenter = get_origin() + cnt;
  wcenter=iPoint(wcenter.x * k, wcenter.y * k);
  set_origin(wcenter - cnt);
  obj->rescale(k); // redraw_me signal should be emitted by object;
}

/***********************************************************/

void
SimpleViewer::draw(const CairoWrapper & crw, const iRect & r){
  if (!r) {redraw(); return;}
  signal_busy_.emit();

  // some objects want to draw on an image, without using
  // Cairo::Context. We create an image, make new CairoWrapper
  // and then transfer information back. Not very good-looking solution.

  // TODO: remove such objects, remove extra CairoWrapper
  // add correct clipping for original context.

  CairoWrapper crw1;
  crw1.set_surface_img(r.w, r.h);
  crw1->set_color(bgcolor);
  crw1->paint();

  if (obj){
    crw1->save();
    crw1->translate(-r.tlc()-origin);
    obj->draw(crw1, r+origin);
    crw1->restore();
  }
  crw1.get_surface()->flush();
  crw->set_source(crw1.get_surface(), r.x, r.y);
  crw->paint();
  signal_idle_.emit();
}

bool
SimpleViewer::on_draw (const Cairo::RefPtr<Cairo::Context> & cr){
  std::vector<Cairo::Rectangle> rects;
  cr->copy_clip_rectangle_list(rects);
  CairoWrapper crw(cr);

  for (auto const & r:rects)
    draw(crw, dRect(r.x,r.y,r.width,r.height));

  return false;
}

bool
SimpleViewer::on_button_press_event (GdkEventButton * event) {
  if ((event->button == 1) || (event->button == 2)){
    drag_pos = get_origin() + iPoint((int)event->x, (int)event->y);
    drag_start = get_origin();
    on_drag=true;
  }
  return false;
}

bool
SimpleViewer::on_button_release_event (GdkEventButton * event) {
  iPoint p;
  Gdk::ModifierType state;
  get_window()->get_pointer(p.x,p.y, state);
  if ((event->button == 1) || (event->button == 2)){
    on_drag=false;
    if (dist(drag_start, get_origin()) > 5) return true;
  }
  signal_click_.emit(drag_pos, event->button, state);
  return false;
}

bool
SimpleViewer::on_motion_notify_event (GdkEventMotion * event) {
  if (!event->is_hint) return false;
  if (on_drag){
    iPoint p((int)event->x, (int)event->y);
    set_origin(drag_pos - p);
  }
  return false;
}

// Note: grabbing focus and processing keypress events
// from the viewer itself is not so simple, and prevents
// other key processing.
// see http://www.mail-archive.com/gtk-list@gnome.org/msg03381.html
// It's better to connect this function to the main window's signal.
bool
SimpleViewer::on_key_press(GdkEventKey * event) {
  switch (event->keyval) {
    case 43:
    case 61:
    case 65451: // + =
      rescale(2.0);
      return true;
    case 45:
    case 95:
    case 65453: // _ -
      rescale(0.5);
      return true;
    case 'r':
    case 'R': // refresh
      redraw();
      return true;
  }
  return false;
}

bool
SimpleViewer::on_scroll_event(GdkEventScroll * event) {
  double scale = event->direction ? 0.5:2.0;
  rescale(scale, iPoint(event->x, event->y));
  return true;
}
