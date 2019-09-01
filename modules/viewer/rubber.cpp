#include <cassert>
#include "cairo/cairo_wrapper.h"
#include "rubber.h"

/****************************************************************/

RubberSegment::RubberSegment(
    const dPoint & p1_, const dPoint & p2_,
    const dPoint & q1_, const dPoint & q2_,
    const rubbfl_t flags_):
    p1(p1_), p2(p2_), q1(q1_), q2(q2_), flags(flags_){
}

dPoint
RubberSegment::get_p1(const iPoint & mouse, const iPoint & origin){
  return dPoint (
    p1.x + q1.x + ((flags & RUBBFL_MOUSE_P1X)? mouse.x : -origin.x),
    p1.y + q1.y + ((flags & RUBBFL_MOUSE_P1Y)? mouse.y : -origin.y)
  );
}

dPoint
RubberSegment::get_p2(const iPoint & mouse, const iPoint & origin){
  return dPoint (
    p2.x + q2.x + ((flags & RUBBFL_MOUSE_P2X)? mouse.x : -origin.x),
    p2.y + q2.y + ((flags & RUBBFL_MOUSE_P2Y)? mouse.y : -origin.y)
  );
}

/****************************************************************/

// show event -> (Viewer::draw)
//            \-> (Rubber::draw)
// mouse events -> (Rubber::move) -> invalidate region -|

Rubber::Rubber(SimpleViewer * v): viewer(v){
  assert(viewer != NULL);

  /// Remove rubber before redraw, put it back after.
  viewer->signal_draw().connect_notify(
    sigc::mem_fun (*this, &Rubber::on_draw), true);

  /// Remove rubber before mouse motion, put it back after.
  viewer->signal_motion_notify_event().connect_notify(
    sigc::mem_fun (*this, &Rubber::on_motion), true);

  // Rescale rubber
  viewer->signal_on_rescale().connect(
    sigc::mem_fun (*this, &Rubber::rescale));
}

/// After mouse motion we need to record mouse position and
/// redraw everything
void
Rubber::on_motion (GdkEventMotion * event) {
  if (!event->is_hint) return;
  if (rubber.empty()) return;
  mouse_pos=iPoint((int)event->x,(int)event->y);
  redraw();
}

/// functions for drawing and erasing rubber
void
Rubber::on_draw(Cairo::RefPtr<Cairo::Context> const & cr_){
  CairoWrapper cr(cr_);

  for (auto & s:rubber){
    dPoint p1 = s.get_p1(mouse_pos, viewer->get_origin());
    dPoint p2 = s.get_p2(mouse_pos, viewer->get_origin());
    int w,h,x,y;

    switch (s.flags & RUBBFL_TYPEMASK){
       case RUBBFL_LINE:
         cr->move_to(p1);
         cr->line_to(p2);
         break;
       case RUBBFL_ELL:
         w=abs(p2.x-p1.x);
         h=abs(p2.y-p1.y);
         x=(p2.x+p1.x)/2;
         y=(p2.y+p1.y)/2;
         goto circles;
       case RUBBFL_ELLC:
         w=2*abs(p2.x-p1.x);
         h=2*abs(p2.y-p1.y);
         x=p1.x;
         y=p1.y;
         goto circles;
       case RUBBFL_CIRC:
         w=h=(int)dist(p2, p1);
         x=(p2.x+p1.x)/2;
         y=(p2.y+p1.y)/2;
         goto circles;
       case RUBBFL_CIRCC:
         w=h=(int)dist(p2, p1)*2;
         x=p1.x;
         y=p1.y;
       circles:
         if (w==0 || h==0) break;
         cr->save();
         cr->translate(x, y);
         cr->scale(w/2.0, h/2.0);
         cr->arc(0.0, 0.0, 1.0, 0.0, 2*M_PI);
         cr->restore();
         break;
       default:
         throw Err() << "Rubber: bad type: " << (s.flags & RUBBFL_TYPEMASK);
    }
  }
  cr->set_source_rgb(1,1,1);
  cr->set_line_width(1);
  cr->set_line_cap(Cairo::LINE_CAP_ROUND);
  cr->set_line_join(Cairo::LINE_JOIN_ROUND);

  cr->stroke_preserve();
  cr->set_source_rgb(0,0,0);
  cr->set_dash(3,3);
  cr->stroke();
}

void
Rubber::redraw() {
  auto win = viewer->get_window();
  if (win) win->invalidate(true);
}

/// add segment to a rubber
void
Rubber::add(const RubberSegment & s){
  rubber.push_back(s);
  redraw();
}

void
Rubber::add(const dPoint & p1, const dPoint & p2,
            const dPoint & q1, const dPoint & q2,
            const rubbfl_t flags){
  add(RubberSegment(p1, p2, q1, q2, flags));
}

/// remove the last segment from the rubber and get it
RubberSegment
Rubber::pop(void){
  if (rubber.size()<1) throw Err() << "Rubber::pop: empty rubber";
  RubberSegment s = *rubber.rbegin();
  rubber.pop_back();
  redraw();
  return s;
}

/// get the last segment from the rubber
RubberSegment
Rubber::get(void) const{
  if (rubber.size()<1) throw Err() << "Rubber::get: empty rubber";
  return *rubber.rbegin();
}

void
Rubber::clear(){
  rubber.clear();
  redraw();
}

int
Rubber::size() const{
  return rubber.size();
}

void Rubber::rescale(double k){
  for (auto & s:rubber){
    s.p1.x*=k;
    s.p1.y*=k;
    s.p2.x*=k;
    s.p2.y*=k;
  }
  redraw();
}

void Rubber::dump(void) const{
  std::cerr << "Rubber:\n";
  for (auto & s:rubber){
    std::cerr << s.p1 << " " << s.p2 
              << " " << (s.flags & RUBBFL_MOUSE_P1X)
              << " " << (s.flags & RUBBFL_MOUSE_P2X)
              << " " << (s.flags & RUBBFL_MOUSE_P1Y)
              << " " << (s.flags & RUBBFL_MOUSE_P2Y)
              << " " << (s.flags >>4) 
              << "\n";
  }
  std::cerr << "-------\n";
}

/****************************************************************/
/// High-level functions for adding some types of segments

void
Rubber::add_sq_mark(const dPoint & p, bool mouse, int size){
  int fl = mouse? RUBBFL_MOUSE:RUBBFL_PLANE;
  dPoint p1(size,size), p2(size,-size);
  add(p,p, -p1,-p2, fl);
  add(p,p, -p2, p1, fl);
  add(p,p,  p1, p2, fl);
  add(p,p,  p2,-p1, fl);
}

void
Rubber::add_cr_mark(const dPoint & p, bool mouse, int size){
  int fl = mouse? RUBBFL_MOUSE:RUBBFL_PLANE;
  double s = sqrt(0.5)*size;
  add(p,p, -dPoint(s,s),  dPoint(s,s), fl);
  add(p,p, -dPoint(s,-s), dPoint(s,-s), fl);
  add(p,p, dPoint(0,0), dPoint(size,0), fl | RUBBFL_CIRCC);
}

void
Rubber::add_line(const dPoint & p){
  dPoint pz(0,0);
  add(p, pz, pz, pz, RUBBFL_MOUSE_P2);
}

void
Rubber::add_line(const dPoint & p1, const dPoint & p2){
  dPoint pz(0,0);
  add(p1, p2, pz, pz);
}

void
Rubber::add_rect(const dPoint & p){
  dPoint pz(0,0);
  add(dPoint(0,p.y), pz,pz,pz, RUBBFL_MOUSE_P1X | RUBBFL_MOUSE_P2);
  add(dPoint(p.x,0), pz,pz,pz, RUBBFL_MOUSE_P1Y | RUBBFL_MOUSE_P2);
  add(dPoint(0,p.y), p,pz,pz, RUBBFL_MOUSE_P1X);
  add(dPoint(p.x,0), p,pz,pz, RUBBFL_MOUSE_P1Y);
}

void
Rubber::add_rect(const dPoint & p1, const dPoint & p2){
  dPoint pz(0,0);
  add(p1, dPoint(p1.x,p2.y), pz,pz);
  add(dPoint(p1.x,p2.y), p2, pz,pz);
  add(p2, dPoint(p2.x,p1.y), pz,pz);
  add(dPoint(p2.x,p1.y), p1, pz,pz);
}

void
Rubber::add_ell(const dPoint & p){
  dPoint pz(0,0);
  add(p, pz,pz,pz, RUBBFL_MOUSE_P2 | RUBBFL_ELL);
}

void
Rubber::add_ellc(const dPoint & p){
  dPoint pz(0,0);
  add(p, pz,pz,pz, RUBBFL_MOUSE_P2 | RUBBFL_ELLC);
}

void
Rubber::add_circ(const dPoint & p){
  dPoint pz(0,0);
  add(p, pz,pz,pz, RUBBFL_MOUSE_P2 | RUBBFL_CIRC);
}

void
Rubber::add_circc(const dPoint & p){
  dPoint pz(0,0);
  add(p, pz,pz,pz, RUBBFL_MOUSE_P2 | RUBBFL_CIRCC);
}
