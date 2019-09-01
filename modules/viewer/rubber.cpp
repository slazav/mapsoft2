#include <cassert>
#include "rubber.h"

/****************************************************************/

RubberSegment::RubberSegment(
  const iPoint & p1_, const iPoint & p2_, const rubbfl_t flags_):
      p1(p1_), p2(p2_), flags(flags_){
}

iPoint
RubberSegment::get_p1(Point<int> mouse, Point<int> origin){
  return iPoint ((flags & RUBBFL_MOUSE_P1X)? (p1.x+mouse.x) : (p1.x-origin.x),
                (flags & RUBBFL_MOUSE_P1Y)? (p1.y+mouse.y) : (p1.y-origin.y));
}

iPoint
RubberSegment::get_p2(Point<int> mouse, Point<int> origin){
  return iPoint ((flags & RUBBFL_MOUSE_P2X)? (p2.x+mouse.x) : (p2.x-origin.x),
                (flags & RUBBFL_MOUSE_P2Y)? (p2.y+mouse.y) : (p2.y-origin.y));
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
Rubber::on_draw(Cairo::RefPtr<Cairo::Context> const & cr){

  for (auto & s:rubber){
    iPoint p1 = s.get_p1(mouse_pos, viewer->get_origin());
    iPoint p2 = s.get_p2(mouse_pos, viewer->get_origin());

    int w,h,x,y;

    switch (s.flags & RUBBFL_TYPEMASK){
       case RUBBFL_LINE:
         cr->move_to(p1.x, p1.y);
         cr->line_to(p2.x, p2.y);
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
  std::vector<double> d;
  d.push_back(3);
  d.push_back(3);
  cr->set_dash(d, 0);
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
Rubber::add(const iPoint & p1, const iPoint & p2, const rubbfl_t flags){
  add(RubberSegment(p1, p2, flags));
}

void
Rubber::add(const int x1, const int y1,
                const int x2, const int y2, const rubbfl_t flags){
  add(RubberSegment(iPoint(x1,y1), Point<int>(x2,y2), flags));
}

/// remove the last segment from the rubber and get it
RubberSegment
Rubber::pop(void){
  if (rubber.size()<1) return RubberSegment(iPoint(),iPoint(),0);
  RubberSegment s = *rubber.rbegin();
  rubber.pop_back();
  redraw();
  return s;
}

/// get the last segment from the rubber
RubberSegment
Rubber::get(void) const{
  if (rubber.size()<1) return RubberSegment(iPoint(),iPoint(),0);
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
    if (!(s.flags & RUBBFL_MOUSE_P1X)) s.p1.x*=k;
    if (!(s.flags & RUBBFL_MOUSE_P1Y)) s.p1.y*=k;
    if (!(s.flags & RUBBFL_MOUSE_P2X)) s.p2.x*=k;
    if (!(s.flags & RUBBFL_MOUSE_P2Y)) s.p2.y*=k;
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
Rubber::add_sq_mark(const iPoint & p, bool mouse, int size){
  int fl = mouse? RUBBFL_MOUSE:RUBBFL_PLANE;
  iPoint p1(size,size), p2(size,-size);
  add( p-p1, p-p2, fl);
  add( p-p2, p+p1, fl);
  add( p+p1, p+p2, fl);
  add( p+p2, p-p1, fl);
}

void
Rubber::add_cr_mark(const iPoint & p, bool mouse, int size){
  int fl = mouse? RUBBFL_MOUSE:RUBBFL_PLANE;
  int s = sqrt(0.5)*size;
  add(p-iPoint(s,s),  p+iPoint(s,s), fl);
  add(p-iPoint(s,-s), p+iPoint(s,-s), fl);
  add(p, p+iPoint(size,0), fl | RUBBFL_CIRCC);
}

void
Rubber::add_line(const iPoint & p){
  add(p, iPoint(0,0), RUBBFL_MOUSE_P2);
}

void
Rubber::add_line(const iPoint & p1, const iPoint & p2){
  add(p1, p2);
}

void
Rubber::add_rect(const iPoint & p){
  add(iPoint(0,p.y), iPoint(0,0), RUBBFL_MOUSE_P1X | RUBBFL_MOUSE_P2);
  add(iPoint(p.x,0), iPoint(0,0), RUBBFL_MOUSE_P1Y | RUBBFL_MOUSE_P2);
  add(iPoint(0,p.y), p, RUBBFL_MOUSE_P1X);
  add(iPoint(p.x,0), p, RUBBFL_MOUSE_P1Y);
}

void
Rubber::add_rect(const iPoint & p1, const iPoint & p2){
  add(p1, iPoint(p1.x,p2.y));
  add(iPoint(p1.x,p2.y), p2);
  add(p2, iPoint(p2.x,p1.y));
  add(iPoint(p2.x,p1.y), p1);
}

void
Rubber::add_ell(const iPoint & p){
  add(p, iPoint(0,0), RUBBFL_MOUSE_P2 | RUBBFL_ELL);
}

void
Rubber::add_ellc(const iPoint & p){
  add(p, iPoint(0,0), RUBBFL_MOUSE_P2 | RUBBFL_ELLC);
}

void
Rubber::add_circ(const iPoint & p){
  add(p, iPoint(0,0), RUBBFL_MOUSE_P2 | RUBBFL_CIRC);
}

void
Rubber::add_circc(const iPoint & p){
  add(p, iPoint(0,0), RUBBFL_MOUSE_P2 | RUBBFL_CIRCC);
}
