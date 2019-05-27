#include "simple_viewer.h"

#include <gdk/gdk.h>
#include <cassert>
#include "image_gdk.h"
#include "image/image.h"
#include "geom/rect.h"

SimpleViewer::SimpleViewer(GObj * o) :
    obj(o),
    origin(iPoint(0,0)),
    bgcolor(0xFF000000),
    sc(1.0),
    on_drag(false),
    waiting(0) {
  set_events (
    Gdk::BUTTON_PRESS_MASK |
    Gdk::BUTTON_RELEASE_MASK |
    Gdk::SCROLL_MASK |
    Gdk::POINTER_MOTION_MASK |
    Gdk::POINTER_MOTION_HINT_MASK );
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

  if (is_realized()){
    get_window()->scroll(origin.x-p.x, origin.y-p.y);
  }
  origin = p;
  signal_ch_origin_.emit(p);
}

/***********************************************************/

void
SimpleViewer::draw(const iRect & r){
  if (is_waiting()) return;
  if (r.empty()) {redraw(); return;}
  signal_busy_.emit();
  Image img(r.w, r.h, 32, 0xFF000000 | bgcolor);
  if (obj) obj->draw(img, r.tlc()+origin);
  draw_image(img, r.tlc());
  signal_idle_.emit();
}

void
SimpleViewer::draw_image (const Image & img, const iPoint & p){
  if (!is_realized()) return;
  Glib::RefPtr<Gdk::Pixbuf> pixbuf = make_pixbuf_from_image(img);
  Glib::RefPtr<Gdk::GC> gc = get_style()->get_fg_gc (get_state());
  Glib::RefPtr<Gdk::Window> widget = get_window();
  signal_before_draw_.emit();
  widget->draw_pixbuf(gc, pixbuf,
          0,0,  p.x, p.y,  img.width(), img.height(),
          Gdk::RGB_DITHER_NORMAL, 0, 0);
  signal_after_draw_.emit();
}

void
SimpleViewer::draw_image (const Image & img, const iRect & part, const iPoint & p){
  if (!is_realized()) return;
  Glib::RefPtr<Gdk::Pixbuf> pixbuf = make_pixbuf_from_image(img);
  Glib::RefPtr<Gdk::GC> gc = get_style()->get_fg_gc (get_state());
  Glib::RefPtr<Gdk::Window> widget = get_window();
  signal_before_draw_.emit();
  widget->draw_pixbuf(gc, pixbuf,
          part.x, part.y,  p.x, p.y,  part.w, part.h,
          Gdk::RGB_DITHER_NORMAL, 0, 0);
  signal_after_draw_.emit();
}

/***********************************************************/

void
SimpleViewer::redraw (void){
  if (is_waiting()) return;
  draw(iRect(0, 0, get_width(), get_height()));
}

void
SimpleViewer::rescale(const double k, const iPoint & cnt){
  if (!obj) return;
  signal_on_rescale_.emit(k);
  start_waiting();
  obj->rescale(k);
  iPoint wsize(get_width(), get_height());
  iPoint wcenter = get_origin() + cnt;
  wcenter=iPoint(wcenter.x * k, wcenter.y * k);
  set_origin(wcenter - cnt);
  stop_waiting();
}

/***********************************************************/

bool
SimpleViewer::on_expose_event (GdkEventExpose * event){
  GdkRectangle *rects;
  int nrects;
  gdk_region_get_rectangles(event->region, &rects, &nrects);
  for (int i = 0; i < nrects; ++i) {
    iRect r(rects[i].x, rects[i].y, rects[i].width, rects[i].height);
    draw(r);
  }
  g_free(rects);
  return true;
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
