#ifndef RECT_H
#define RECT_H

#include <iostream>
#include <limits>
#include <iomanip>
#include <ios>
#include "point.h"
#include "err/err.h"

///\addtogroup libmapsoft
///@{

/** 2-d rectangle.
 - Coordinates are counted from top-left.
   Top-left corner has lowest coordinates, bottom-right corner
   has highest coordinates.
 - All corners are included in the rectangle.
 - There is a difference between empty rectangle (such as a
   bounding box of a zero-point line) and zero-size rectangle
   (bounding box of a one-point line).
 - many functions throw error if rectangle is empty
*/
template <typename T>
struct Rect {
  T x; ///< x coordinate
  T y; ///< y coordinate
  T w; ///< width
  T h; ///< height
  bool e; ///< empty flag

  /// Constructor: create empty rectangle
  Rect() : x(0), y(0), w(0), h(0), e(true) { }

  /** Constructor: create non-empty rectangle by two opposite corners.
      You can use any pair of points in any order. */
  Rect(Point<T> p1, Point<T> p2)
    : x(std::min(p1.x, p2.x)), y(std::min(p1.y, p2.y)),
      w(std::max(p1.x,p2.x) - std::min(p1.x,p2.x)),
      h(std::max(p1.y,p2.y) - std::min(p1.y,p2.y)),
      e(false) { }

  /** Constructor: create non-empty rectangle from x,y,w,h values.
      If w (or h) is negative, then x (or y) is right (or bottom) coordinate:
      Rect(0,0,-1,-1) = Rect(-1,-1,1,1) */
  Rect(T _x, T _y, T _w=0, T _h=0)
    : x(_w>=0?_x:_x+_w), y(_h>=0?_y:_y+_h),
      w(_w>=0?_w:-_w), h(_h>=0?_h:-_h),
      e(false) { }

  /// Constructor: create a rectangle string "[]" or "[x,y,w,h]"
  Rect(const std::string & s) { *this = str_to_type<Rect>(s);}

  /// Swap rectangle with other one
  void swap (Rect & other){
    std::swap (x, other.x);
    std::swap (y, other.y);
    std::swap (w, other.w);
    std::swap (h, other.h);
    std::swap (e, other.e);
  }

  /// Test if rectangle is empty
  bool empty() const { return e; }

  /// Test if rectangle has zero size (but not empty)
  bool zsize() const { return (w==0 || h==0) && !e; }

  /******************************************************************/
  // corners

  /// Top-left corner
  Point<T> tlc() const {
    if (e) throw Err() << "Empty rectangle in tlc call";
    return  Point<T>(x,y);
  }

  /// Top-right corner
  Point<T> trc() const {
    if (e) throw Err() << "Empty rectangle in trc call";
    return  Point<T>(x+w,y);
  }

  /// Bottom-right corner
  Point<T> brc() const {
    if (e) throw Err() << "Empty rectangle in brc call";
    return  Point<T>(x+w,y+h);
  }

  /// Bottom-left corner
  Point<T> blc() const {
    if (e) throw Err() << "Empty rectangle in blc call";
    return  Point<T>(x,y+h);
  }

  /// Center point
  Point<T> cnt() const {
    if (e) throw Err() << "Empty rectangle in cnt call";
    return  Point<T>(x+w/2,y+h/2);
  }

  /******************************************************************/
  // operators +,-,/,*

  /// Divide all coordinates by k
  Rect<T> & operator/= (T k) {
    if (e) throw Err() << "Empty rectangle in operator/";
    x /= k; y /= k; w /= k; h /= k; return *this; }

  /// Multiply all coordinates by k
  Rect<T> & operator*= (T k) {
    if (e) throw Err() << "Empty rectangle in operator*";
    x *= k; y *= k; w *= k; h *= k; return *this; }

  /// Subtract point from the rectangle (shift the rectangle)
  Rect<T> & operator-= (Point<T> p) {
    if (e) throw Err() << "Empty rectangle in operator-";
    x -= p.x; y -= p.y; return *this; }

  /// Add point to the rectangle (shift the rectangle)
  Rect<T> & operator+= (Point<T> p){
    if (e) throw Err() << "Empty rectangle in operator+";
    x += p.x; y += p.y; return *this; }

  /// Divide all coordinates by k
  Rect<T> operator/ (T k) const { Rect<T> ret(*this); return ret/=k; }

  /// Multiply all coordinates by k
  Rect<T> operator* (T k) const { Rect<T> ret(*this); return ret*=k; }

  /// Subtract point from the rectangle (shift the rectangle)
  Rect<T> operator- (Point<T> p) const { Rect<T> ret(*this); return ret-=p; }

  /// Add point to the rectangle (shift the rectangle)
  Rect<T> operator+ (Point<T> p) const { Rect<T> ret(*this); return ret+=p; }

  /// operator-: invert
  Rect<T> operator- () const {
    if (e) throw Err() << "Empty rectangle in operator-";
    Rect<T> ret(-this->tlc(), -this->brc()); return ret;
  }

  /******************************************************************/
  // operators <=>

  /// Less-then operator: compare TLC, then BRC.
  /// Empty rectungles are smaller then non-empty ones.
  bool operator< (const Rect<T> & r) const {
      if (r.e) return false;
      if (e) return true;
      if (tlc()!=r.tlc()) return (tlc()<r.tlc());
      return (brc()<r.brc());
  }

  /// Equal operator: Empty rectungles are always equal. 
  bool operator== (const Rect<T> & r) const {
      if (e && r.e) return true;
      if (e || r.e) return false;
      return (x==r.x)&&(y==r.y)&&(w==r.w)&&(h==r.h);
  }

  // derived operators:
  bool operator!= (const Rect<T> & other) const { return !(*this==other); } ///< operator!=
  bool operator>= (const Rect<T> & other) const { return !(*this<other);  } ///< operator>=
  bool operator<= (const Rect<T> & other) const { return *this<other || *this==other; } ///< operator<=
  bool operator>  (const Rect<T> & other) const { return !(*this<=other); } ///< operator>

  /******************************************************************/

  /// Cast to Rect<double>
  operator Rect<double>() const{
    if (e) return Rect<double>();
    return Rect<double>(double(this->x), double(this->y),
                        double(this->w), double(this->h));
  }

  /// Cast to Rect<int>
  operator Rect<int>() const{
    if (e) return Rect<int>();
    return Rect<int>(int(this->x), int(this->y),
                     int(this->w), int(this->h));
  }

  /******************************************************************/
  // Some functions. Below same functions are defined outside the class

  /// rint function: change corner coordenates to nearest integers
  Rect<T> rint() const {
    if (e) return Rect<T>();
   return Rect<T>(tlc().rint(), brc().rint());
  }

  /// floor function: shrink the rectangle to nearest integer coordinate
  Rect<T> floor() const {
    if (e) return Rect<T>();
    return Rect<T>(tlc().ceil(), brc().floor());
  }

  /// ceil function: expand the rectangle to nearest integer coordinates
  Rect<T> ceil() const {
    if (e) return Rect<T>();
    return Rect<T>(tlc().floor(), brc().ceil());
  }

  /// Pump rectangle to each side by val value.
  /// If the rectangle is empty throw an error.
  /// If val is negative rectangle can shrink to an empty one.
  Rect<T> pump (T val) const {
    if (e) throw Err() << "Empty rectangle in pump()";
    if (w+2*val<0 || h+2*val<0) return Rect<T>();
    return Rect<T> (x-val, y-val, w+2*val, h+2*val);
  }

  /// Pump rectangle by vx and vy values.
  /// If the rectangle is empty throw an error.
  /// If val is negative rectangle can shrink to an empty one.
  Rect<T> pump (T vx, T vy) const {
    if (e) throw Err() << "Empty rectangle in pump()";
    if (w+2*vx<0 || h+2*vy<0) return Rect<T>();
    return Rect<T> (x-vx, y-vy, w+2*vx, h+2*vy);
  }

  /// Expand rectangle to cover point p. Can be used with empty rectangle.
  Rect<T> expand (const Point<T> & p) const {
    if (e) return Rect<T>(p,p);
    T x1 =  std::min (x, p.x);
    T y1 =  std::min (y, p.y);
    T x2 =  std::max (x+w, p.x);
    T y2 =  std::max (y+h, p.y);
    return Rect<T>(x1,y1,x2-x1,y2-y1);
  }

  /// Expand rectangle to cover rectangle r. Can be used with empty rectangle.
  Rect<T> expand (const Rect<T> & r) const {
    if (e)   return r;
    if (r.e) return *this;
    T x1 =  std::min (x, r.x);
    T y1 =  std::min (y, r.y);
    T x2 =  std::max (x+w, r.x+r.w);
    T y2 =  std::max (y+h, r.y+r.h);
    return Rect<T>(x1,y1,x2-x1,y2-y1);
  }

  /// Calculate intersection with rectangle r. Can be used with empty rectangle.
  Rect<T> intersect (const Rect<T> & r) const {
    if (e || r.e)   return Rect<T>();
    T x1 =  std::max (x, r.x);
    T y1 =  std::max (y, r.y);
    T x2 =  std::min (x+w, r.x+r.w);
    T y2 =  std::min (y+h, r.y+r.h);
    T w = x2-x1;
    T h = y2-y1;
    if (w<0 || h<0) return Rect<T>();
    return Rect<T>(x1,y1,w,h);
  }

  /// Is rectangle contains a point
  bool contains (const Point<T> & p) const {
    if (e) return false;
    return (p.x >= x) && (p.x <= x+w) &&
           (p.y >= y) && (p.y <= y+h);
  }

  /// Is rectangle contains another rectangle.
  /// If any of two rectangles is empty false is returned.
  bool contains (const Rect<T> & r) const {
    if (e || r.e) return false;
    return (r.x >= x) && (r.x+r.w <= x+w) &&
           (r.y >= y) && (r.y+r.h <= y+h);
  }

};

/******************************************************************/
// additional operators

/// Multiply coordinates by k (k*rect = rect*k)
/// \relates Rect
template <typename T>
Rect<T> operator* (const T k, const Rect<T> & r) { return r*k; }

/// Add p to every point (p+rect = rect+p)
/// \relates Rect
template <typename T>
Rect<T> operator+ (const Point<T> & p, const Rect<T> & r) { return r+p; }

/******************************************************************/
// same functions as in the class

/// Pump rectangle to each side by val value.
/// \relates Rect
template <typename T>
Rect<T> pump(const Rect<T> & r, T val) { return r.pump(val); }

/// Pump rectangle by vx and vy values.
/// If the rectangle is empty throw an error.
/// If val is negative rectangle can shrink to an empty one.
/// \relates Rect
template <typename T>
Rect<T> pump(const Rect<T> & r, T vx, T vy) { return r.pump(vx,vy); }

/// Expand rectangle to cover point p. Can be used with empty rectangle.
/// \relates Rect
template <typename T>
Rect<T> expand (const Rect<T> & r, const Point<T> & p) { return r.expand(p); }

/// Expand rectangle to cover rectangle r. Can be used with empty rectangle.
/// \relates Rect
template <typename T>
Rect<T> expand (const Rect<T> & r1, const Rect<T> & r2) { return r1.expand(r2); }

/// Calculate intersection with rectangle r. Can be used with empty rectangle.
/// \relates Rect
template <typename T>
Rect<T> intersect (const Rect<T> & r1, const Rect<T> & r2) { return r1.intersect(r2); }

/// Is rectangle contains a point
/// \relates Rect
template <typename T>
bool contains (const Rect<T> & r, const Point<T> & p) { return r.contains(p); }

/// Is rectangle contains another rectangle.
/// If any of two rectangles is empty false is returned.
/// \relates Rect
template <typename T>
bool contains (const Rect<T> & r1, const Rect<T> & r2) { return r1.contains(r2); }

/******************************************************************/
// input/output

/// \relates Rect
/// \brief Output operator: print Rect as a json array: [] or [x,y,w,h]
template <typename T>
std::ostream & operator<< (std::ostream & s, const Rect<T> & r){
  if (r.e) {s << "[]"; return s;}
  s << std::setprecision(8) 
    << "[" << r.x << "," << r.y << "," << r.w << "," << r.h << "]";
  return s;
}

/// \relates Rect
/// \brief Input operator: read Rect from a json array: [] or [x,y,w,h]
template <typename T>
std::istream & operator>> (std::istream & s, Rect<T> & r){
  char sep;
  s >> std::ws >> sep;
  if (sep!='['){
    s.setstate(std::ios::failbit);
    return s;
  }

  s >> std::ws >> sep;
  if (sep==']'){
    s >> std::ws;
    s.setstate(std::ios::goodbit);
    r=Rect<T>();
    return s;
  }
  s.putback(sep);

  s >> std::ws >> r.x >> std::ws >> sep;
  if (sep!=','){ s.setstate(std::ios::failbit); return s; }

  s >> std::ws >> r.y >> std::ws >> sep;
  if (sep!=','){ s.setstate(std::ios::failbit); return s; }

  s >> std::ws >> r.w >> std::ws >> sep;
  if (sep!=','){ s.setstate(std::ios::failbit); return s; }

  s >> std::ws >> r.h >> std::ws >> sep >> std::ws;
  if (sep!=']'){ s.setstate(std::ios::failbit); return s; }

  r.e=false;
  s.setstate(std::ios::goodbit);
  return s;
}

/******************************************************************/
// type definitions

/// Rect with double coordinates
/// \relates Rect
typedef Rect<double> dRect;

/// Rect with int coordinates
/// \relates Rect
typedef Rect<int> iRect;

///@}
#endif
