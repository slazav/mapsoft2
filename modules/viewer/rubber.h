#ifndef RUBBER_VIEWER_H
#define RUBBER_VIEWER_H

#include <list>
#include "simple_viewer.h"

///\addtogroup gred
///@{
/**\defgroup rubber
"Rubber lines" in the viewer. Can be attached to the drawing plane or to the mouse.
It is possible to use a few Rubber objects in one viewer.
*/
///@{

typedef unsigned int rubbfl_t;
#define RUBBFL_PLANE     0x0 ///< both points connected to the plane
#define RUBBFL_MOUSE_P1X 0x1 ///< p1.x connected to mouse
#define RUBBFL_MOUSE_P1Y 0x2 ///< p1.y connected to mouse
#define RUBBFL_MOUSE_P1  0x3 ///< p1 connected to mouse
#define RUBBFL_MOUSE_P2X 0x4 ///< p2.x connected to mouse
#define RUBBFL_MOUSE_P2Y 0x8 ///< p2.y connected to mouse
#define RUBBFL_MOUSE_P2  0xC ///< p2 connected to mouse
#define RUBBFL_MOUSE     0xF ///< both points connected to the mouse

#define RUBBFL_TYPEMASK 0xF0
#define RUBBFL_LINE  0x00    ///< line
#define RUBBFL_ELL   0x10    ///< ellipse in a box with p1-p2 diagonal
#define RUBBFL_ELLC  0x20    ///< ellipse with center in p1
#define RUBBFL_CIRC  0x30    ///< circle with p1-p2 diameter
#define RUBBFL_CIRCC 0x40    ///< circle with p1-p2 radius

/**
 Class for the rubber segment -- two points with some flags
 flags bits (see definitions above):
 0 - p1.x connected to mouse?
 1 - p1.y connected to mouse?
 2 - p2.x connected to mouse?
 3 - p2.y connected to mouse?
 4-7 - type of segment:
   0x00 -- line
   0x10 -- ellipse in a box with p1-p2 diagonal
   0x20 -- ellipse with center in p1
   0x30 -- circle with p1-p2 diameter
   0x40 -- circle with p1-p2 radius
*/

struct RubberSegment{
  rubbfl_t flags;
  dPoint p1, p2;

  /// create RubberSegment from two points and flags
  RubberSegment(const dPoint & p1_, const dPoint & p2_, const rubbfl_t flags_);

  // calculate absolute coordinates of the first point
  dPoint get_p1(const iPoint & mouse, const iPoint & origin);

  // calculate absolute coordinates of the second point
  dPoint get_p2(const iPoint & mouse, const iPoint & origin);

};


/// Rubber -- class for drawing rubber lines on a viewer
class Rubber{
private:
  std::list<RubberSegment> rubber;
  iPoint mouse_pos;
  SimpleViewer * viewer;

public:

  Rubber(SimpleViewer * v);

private:

  /// callbacks to be connected to viewer signals
  void on_motion(GdkEventMotion * event);
  void on_draw(Cairo::RefPtr<Cairo::Context> const & cr); ///<draw rubber


public:

  /// redraw rabber
  void redraw();

  /// add segment to a rubber
  void add(const RubberSegment & s);
  void add(const dPoint & p1, const dPoint & p2,
           const rubbfl_t flags = RUBBFL_PLANE);
  void add(const double x1, const double y1,
           const double x2, const double y2,
           const rubbfl_t flags = RUBBFL_PLANE);

  /// remove the last segment from the rubber and get it
  RubberSegment pop(void);
  /// get the last segment from the rubber
  RubberSegment get(void) const;
  /// cleanup rubber
  void clear();
  /// count segments
  int size() const;
  /// dump rubber to stderr
  void dump(void) const;
  /// modify coordinates connected to plane
  void rescale(double k);

  /// High-level functions for adding some types of segments
  void add_sq_mark(const dPoint & p, bool mouse=true, int size=5); ///< square mark
  void add_cr_mark(const dPoint & p, bool mouse=true, int size=5);  ///< cross-and-circle mark
  void add_line(const dPoint & p);                     ///< line from p to mouse
  void add_line(const dPoint & p1, const dPoint & p2); ///< line from p1 to p2
  void add_rect(const dPoint & p);                     ///< rectangle from p to mouse
  void add_rect(const dPoint & p1, const dPoint & p2); ///< rectangle from p1 to p2
  void add_ell(const dPoint & p);                      ///< ellipse
  void add_ellc(const dPoint & p);
  void add_circ(const dPoint & p);
  void add_circc(const dPoint & p);

};

#endif
