#ifndef FIG_H
#define FIG_H

#include <list>
#include <vector>
#include <string>
#include <map>
#include <iostream>

#include "geom/line.h"
#include "opt/opt.h"

#define FIG_COLOR_DEF 0
#define FIG_ELLIPSE   1
#define FIG_POLYLINE  2
#define FIG_SPLINE    3
#define FIG_TXT       4
#define FIG_ARC       5
#define FIG_COMPOUND  6
#define FIG_END_COMPOUND -FIG_COMPOUND


/// fig object
struct FigObj : iLine {
  // iLine object contains coordinates.
  // For ellipse it is the center, for arc it is three points.
  int     type;
  int     sub_type;
  int     line_style;          ///    (enumeration type, solid, dash, dotted, etc.)
  int     thickness;           ///    (1/80 inch)
  int     pen_color;           ///    (enumeration type, pen color)
  int     fill_color;          ///    (enumeration type, fill color)
  int     depth;               ///    (enumeration type)
  int     pen_style;           ///    (pen style, not used)
  int     area_fill;           ///    (enumeration type, -1 = no fill)
  float   style_val;           ///    (1/80 inch, specification for dash/dotted lines)
  int     join_style;          ///    (enumeration type)
  int     cap_style;           ///    (enumeration type)
  int     radius;              ///    (1/80 inch, radius of arc-boxes)
  int     direction;           ///    (0: clockwise, 1: counterclockwise)
  float   angle;               ///    (radians, the angle of the x-axis)
  int     forward_arrow;       ///    (0: no forward arrow, 1: on)
  int     backward_arrow;      ///    (0: no forward arrow, 1: on)
  float   center_x, center_y;  ///    (center of the arc, should be ignored?)
  int     radius_x, radius_y;  ///    (ellipse radia, Fig units)
  int     start_x, start_y;    ///    (Fig units; the 1st ellipse point entered)
  int     end_x, end_y;        ///    (Fig units; the last ellipse point entered)
  int     font;                ///    (enumeration type)
  int     font_size;           ///    (font size in points)
  int     font_flags;          ///    (bit vector)
  int     height, length;      ///    (text dimensions, should be ignored?)
  // forward/backward arrow parameters:
  int     farrow_type, barrow_type;
  int     farrow_style, barrow_style;
  float   farrow_thickness, barrow_thickness;
  float   farrow_width, barrow_width;
  float   farrow_height, barrow_height;

  std::string  image_file;
  int          image_orient;
  std::string  text;
  std::vector<std::string> comment;
  std::vector<float>  f; // spline parameters

  /******************************************************************/
  /// Constructor -- make a default object
  FigObj();

  /******************************************************************/
  // operators <=>

  /// Equal operator
  bool operator== (const FigObj & o) const;

  bool operator< (const FigObj & o) const;

  // derived operators:
  bool operator!= (const FigObj & other) const { return !(*this==other); } ///< operator!=
  bool operator>= (const FigObj & other) const { return !(*this<other);  } ///< operator>=
  bool operator<= (const FigObj & other) const { return *this<other || *this==other; } ///<
  bool operator>  (const FigObj & other) const { return !(*this<=other); } ///< operator>

  /******************************************************************/
  // operators +,-,/,*

  /// Add point p (shift FigObj)
  FigObj & operator+= (const iPoint & p) {
    for (iterator i=begin(); i!=end(); i++) *i += p;
    center_x+=p.x;
    center_y+=p.y;
    return *this;
  }

  /// Subtract point p (shift FigObj)
  FigObj & operator-= (const iPoint & p) {
    for (iterator i=begin(); i!=end(); i++) *i -= p;
    center_x-=p.x;
    center_y-=p.y;
    return *this;
  }

  /// Divide coordinates by k
  template <typename T>
  FigObj & operator/= (const T k) {
    for (iterator i=begin(); i!=end(); i++) *i /= k;
    center_x/=k;
    center_y/=k;
    return *this;
  }

  /// Multiply coordinates by k
  template <typename T>
  FigObj & operator*= (const T k) {
    for (iterator i=begin(); i!=end(); i++) *i *= k;
    center_x*=k;
    center_y*=k;
    return *this;
  }

  /// Add point p (shift FigObj)
  FigObj operator+ (const iPoint & p) const { FigObj ret(*this); return ret+=p; }

  /// Subtract point p (shift FigObj)
  FigObj operator- (const iPoint & p) const { FigObj ret(*this); return ret-=p; }

  /// Divide coordinates by k
  template <typename T>
  FigObj operator/ (const T k) const { FigObj ret(*this); return ret/=k; }

  /// Multiply coordinates by k
  template <typename T>
  FigObj operator* (const T k) const { FigObj ret(*this); return ret*=k; }

  /// Invert coordinates
  FigObj operator- () const {
    FigObj ret(*this);
    for (FigObj::iterator i=ret.begin(); i!=ret.end(); i++) (*i)=-(*i);
    return ret;
  }

  /******************************************************************/

  bool is_ellipse()  const {return (type==1);}
  bool is_polyline() const {return (type==2);}
  bool is_spline()   const {return (type==3);}
  bool is_text()     const {return (type==4);}
  bool is_arc()      const {return (type==5);}
  bool is_compound() const {return (type==6);}
  bool is_compound_end() const {return (type==-6);}

  bool is_closed() const {
    if (is_polyline()) return (sub_type>1);
    if (is_spline())   return (sub_type%2==1);
    if (is_ellipse())  return true;
    return false;
  }
  void close(){
    if (is_closed()) return;
    if (is_polyline()){ sub_type=3; return;}
    if (is_spline())  { sub_type++; return;}
    return;
  }
  void open(){
    if (!is_closed()) return;
    if (is_polyline()&&(sub_type==3)){ sub_type=1; return;}
    if (is_spline())  { sub_type--; return;}
    return;
  }

  /******************************************************************/

  /// set points from a line
  void set_points(const dLine & v);
  void set_points(const iLine & v);

  /// Convert to a line
  template <typename T>
  operator Line<T> () const {
    Line<T> ret;
    for (const_iterator i=begin(); i!=end(); i++)
      ret.push_back(Point<T>(i->x, i->y));
    return ret;
  }

};



/******************************************************************/
/******************************************************************/

/// fig-file
struct Fig:std::list<FigObj>{
  // const values
  static const double                    cm2fig, fig2cm;   // fig units
  static const std::map<int,int>         colors;           // fig colors
  static const std::map<int,std::string> psfonts;          // ps fonts
  static const std::map<int,std::string> texfonts;         // tex fonts

  std::string orientation;
  std::string justification;
  std::string units;
  std::string papersize;
  float magnification;
  std::string multiple_page;
  int transparent_color;
  int resolution;
  int coord_system; // ignored in xfig
  std::vector<std::string> comment;

  Fig(){
    orientation="Portrait";
    justification="Center";
    units="Metric";
    papersize="A4";
    magnification=100.0;
    multiple_page="Single";
    transparent_color=-2;
    resolution=1200;
    coord_system=2;
  }

  /******************************************************************/
  // operators +,-,/,*

  /// Add point p (shift the fig file)
  Fig & operator+= (const iPoint & p) {
    for (iterator i=begin(); i!=end(); i++) *i += p;
    return *this;
  }

  /// Subtract point p
  Fig & operator-= (const iPoint & p) {
    for (iterator i=begin(); i!=end(); i++) *i -= p;
    return *this;
  }

  /// Divide coordinates by k
  template <typename T>
  Fig & operator/= (const T k) {
    for (iterator i=begin(); i!=end(); i++) *i /= k;
    return *this;
  }

  /// Multiply coordinates by k
  template <typename T>
  Fig & operator*= (const T k) {
    for (iterator i=begin(); i!=end(); i++) *i *= k;
    return *this;
  }

  /// Add p to every point (shift the line)
  Fig operator+ (const iPoint & p) const { Fig ret(*this); return ret+=p; }

  /// Subtract p from every point (shift the line)
  Fig operator- (const iPoint & p) const { Fig ret(*this); return ret-=p; }

  /// Divide coordinates by k
  template <typename T>
  Fig operator/ (const T k) const { Fig ret(*this); return ret/=k; }

  /// Multiply coordinates by k
  template <typename T>
  Fig operator* (const T k) const { Fig ret(*this); return ret*=k; }

  /// Invert coordinates
  Fig operator- () const {
    Fig ret(*this);
    for (Fig::iterator i=ret.begin(); i!=ret.end(); i++) (*i)=-(*i);
    return *this;
  }

  /******************************************************************/

  iRect bbox() const{
    if (size()<1) return iRect();
    const_iterator i=begin();
    iRect ret = i->bbox();
    while ((++i) != this->end()) ret.expand(i->bbox());
    return ret;
  }

  /// remove empty compounds
    void remove_empty_comp();
};

/******************************************************************/
/* Read Fig-file from a stream. Objects will be appended,
   header will be rewrited (unless fig_header=0 is used).
   Options:
     fig_enc      -- encoding (default "KOI8-R")
     fig_header   -- read header (default 1)
*/
void read_fig(std::istream & s, Fig & w, const Opt & ropts = Opt());

/// Read object header string. Object will be incomplete
/// if further reading is needed (multiline text, arrowheads, point coordinates).
/// The object is not initialized.
/// Comments and options are not modified.
/// Text encoding, colors are not modified.
/// Returns number of points for lines and splines; multiline flag for text objects.
int read_figobj_header(FigObj &o, const std::string & header);

/* Write data in FIG file format
   Options:
     fig_enc      -- encoding (default "KOI8-R")
     fig_header   -- write header (default 1, used for tests)
     fig_7bit     -- write characters > 127 in \nnn form (default 0).
*/
void write_fig(std::ostream & s, const Fig & w, const Opt & wopts = Opt());


#endif
