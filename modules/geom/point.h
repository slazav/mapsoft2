#ifndef POINT_H
#define POINT_H

#include <iostream> // for << and >> operators
#include <iomanip>
#include <ios>
#include <cmath>    // for rint
#include "err/err.h" // for Err
#include "opt/opt.h" // for str_to_type

///\addtogroup libmapsoft
///@{

/** 2-d point
  - coordinates of any type
  - arithmetic (point+point, point*number, -point)
  - comparison operators
  - scalar multiplication, vector length
  - input/output as two-element json arrays
*/
template <typename T>
struct Point {
  T x; ///< x coordinate
  T y; ///< y coordinate
  T z; ///< z coordinate

  /// Constructor: make a point using two coordinates
  Point(): x(0), y(0), z(0) { }

  /// Constructor: make a point using two coordinates
  Point(T _x, T _y, T _z = 0): x(_x), y(_y), z(_z) { }

  /// Constructor: make a point using string "[x,y]"
  Point(const std::string & s) { *this = str_to_type<Point>(s);}

  /// Swap point with other one.
  void swap (Point & other) {
    std::swap (x, other.x);
    std::swap (y, other.y);
    std::swap (z, other.z);
  }

  /******************************************************************/
  // operators +,-,/,*

  /// Subtract coordinates
  Point & operator-= (Point const & other) { x -= other.x; y -= other.y; z -= other.z; return *this; }

  /// Add coordinates
  Point & operator+= (Point const & other) { x += other.x; y += other.y; z += other.z; return *this; }

  /// Divide coordinates by k
  Point & operator/= (const T k) { x /= k; y /= k; z /= k; return *this; }

  /// Multiply coordinates by k
  Point & operator*= (const T k) { x *= k; y *= k; z *= k; return *this; }

  /// Subtract coordinates
  Point operator- (Point const & other) const { return Point(x-other.x, y-other.y, z-other.z); }

  /// Add coordinates
  Point operator+ (Point const & other) const { return Point(x+other.x, y+other.y, z+other.z); }

  /// Invert point coordinates
  Point operator- () const { return Point(-x,-y,-z); }

  /// Divide coordinates by k
  Point operator/ (const T k) const { return Point(x/k, y/k, z/k); }

  /// Multiply coordinates by k
  Point operator* (const T k) const { return Point(x*k, y*k, z*k); }

  /******************************************************************/
  // operators <=>

  /// Less then operator
  bool operator< (const Point & other) const {
    return (x<other.x) || ((x==other.x)&&(y<other.y)) ||
           ((x==other.x)&&(y==other.y)&&(z<other.z));
  }

  /// Equality opertator
  bool operator== (const Point & other) const {
    return (x==other.x)&&(y==other.y)&&(z==other.z);
  }

  // derived operators:
  bool operator!= (const Point & other) const { return !(*this==other); } ///< operator!=
  bool operator>= (const Point & other) const { return !(*this<other);  } ///< operator>=
  bool operator<= (const Point & other) const { return *this<other || *this==other; } ///< operator<=
  bool operator>  (const Point & other) const { return !(*this<=other); } ///< operator>

  /******************************************************************/

  /// Cast to Point<double>
  operator Point<double>() const{
    return Point<double>(double(this->x), double(this->y), double(this->z));
  }

  /// Cast to Point<int>
  operator Point<int>() const{
    return Point<int>(int(this->x), int(this->y), int(this->z));
  }

  /******************************************************************/
  // Some functions. Below same functions are defined outside the class

  /// rint function: change coordinates to the nearest integers
  void to_rint() {
    x=(T)::rint(x);
    y=(T)::rint(y);
    z=(T)::rint(z);
  }

  /// floor function: change coordinates to nearest smaller integers
  void to_floor() {
    x=(T)::floor(x);
    y=(T)::floor(y);
    z=(T)::floor(z);
  }

  /// ceil function: change coordinates to nearest larger integers
  void to_ceil() {
    x=(T)::ceil(x);
    y=(T)::ceil(y);
    z=(T)::ceil(z);
  }

  /// abs function: change coordinates to their absolute values
  void to_abs() {
    x=x>0?x:-x;
    y=y>0?y:-y;
    z=z>0?z:-z;
  }

  /// rotate the point around point c by the angle a (rad, clockwise) in x-y plane
  void rotate2d(const Point & c, const double a) {
    double C=cos(a), S=sin(a);
    *this = Point(C*(double)(x-c.x) + S*(double)(y-c.y) + c.x,
                  C*(double)(y-c.y) - S*(double)(x-c.x) + c.y, z);
  }

  /// project the point to x-y plane
  void flatten() { z=0; }

  /******************************************************************/

  /// Calculate manhattan length: abs(x) + abs(y) + abs(z)
  T mlen() const { return std::abs(x) + std::abs(y) + std::abs(z); }

  /// Calculate length: sqrt(x^2 + y^2 + z^2).
  double len() const { return sqrt((double)(x*x+y*y+z*z));}

  /// Calculate 2D manhattan length: abs(x) + abs(y)
  T mlen2d() const { return std::abs(x) + std::abs(y); }

  /// Calculate 2D length: sqrt(x^2 + y^2).
  double len2d() const { return sqrt((double)(x*x+y*y));}


};

/******************************************************************/
// additional operators

/// Multiply coordinates by k (k*point = point*k)
/// \relates Point
template <typename T>
Point<T> operator* (const T k, const Point<T> & p) { return p*k; }

/******************************************************************/
// functions, similar to ones in the class

/// rint function
/// \relates Point
template <typename T>
Point<int> rint(const Point<T> & p){
  return Point<int>((T)::rint(p.x),(T)::rint(p.y),(T)::rint(p.z)); }

/// floor function
/// \relates Point
template <typename T>
Point<int> floor(const Point<T> & p){
  return Point<int>((T)::floor(p.x),(T)::floor(p.y),(T)::floor(p.z)); }

/// ceil function
/// \relates Point
template <typename T>
Point<int> ceil(const Point<T> & p){
  return Point<int>((T)::ceil(p.x),(T)::ceil(p.y),(T)::ceil(p.z)); }

/// abs function
/// \relates Point
template <typename T>
Point<T> abs(const Point<T> & p){
  return Point<T>(p.x>0?p.x:-p.x, p.y>0?p.y:-p.y, p.z>0?p.z:-p.z); }

/// rotate the point around point c by the angle a (rad, clockwise) in x-y plane
template <typename T>
Point<T> rotate2d(const Point<T> & p, const Point<T> & c, const double a){
  Point<T> p1(p);
  p1.rotate2d(c,a);
  return p1;
}

/// project the point to x-y plane
template <typename T>
Point<T> flatten(const Point<T> & p) { return Point<T>(p.x,p.y); }


/******************************************************************/

/// Calculate manhattan length: abs(x) + abs(y) + abs(z)
/// \relates Point
template <typename T>
T mlen (const Point<T> & p) { return p.mlen(); }

/// Calculate length: sqrt(x^2 + y^2 + z^2)
/// \relates Point
template <typename T>
double len(const Point<T> & p) { return p.len();}

/// Calculate manhattan length: abs(x) + abs(y)
/// \relates Point
template <typename T>
T mlen2d (const Point<T> & p) { return p.mlen2d(); }

/// Calculate length: sqrt(x^2 + y^2)
/// \relates Point
template <typename T>
double len2d(const Point<T> & p) { return p.len2d();}

/******************************************************************/
// extra functions

/// Scalar multiplication: p1.x*p2.x + p1.y*p2.y + p1.z*p2.z
/// \relates Point
template <typename T>
double pscal(const Point<T> & p1, const Point<T> & p2){
  return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z;
}

/// Distance between two points: (p1-p2).len()
/// \relates Point
template <typename T>
double dist(const Point<T> & p1, const Point<T> & p2){
  return (p1-p2).len();
}

/// Normalize: p/len(p). Throw error if point is 0
/// \relates Point
template <typename T>
Point<double> norm(const Point<T> & p) {
  if (p.x==0 && p.y==0 && p.z==0) throw Err() << "Point norm: zero length";
  return Point<double>(p)/p.len();
}

/// 2D scalar multiplication: p1.x*p2.x + p1.y*p2.y
/// \relates Point
template <typename T>
double pscal2d(const Point<T> & p1, const Point<T> & p2){
  return p1.x*p2.x + p1.y*p2.y;
}

/// 2D distance between two points: (p1-p2).len()
/// \relates Point
template <typename T>
double dist2d(const Point<T> & p1, const Point<T> & p2){
  return (p1-p2).len2d();
}

/// 2D normalize: flatten(p/len2d), z is set to 0. Throw error len2 is 0
/// \relates Point
template <typename T>
Point<double> norm2d(const Point<T> & p) {
  if (p.x==0 && p.y==0) throw Err() << "Point norm: zero length";
  double l = len2d(p);
  return Point<double>(p.x,p.y, 0)/l;
}

/******************************************************************/
// input/output

/// Output operator: print point as a two-element json array
/// \relates Point
template <typename T>
std::ostream & operator<< (std::ostream & s, const Point<T> & p){
  s << std::setprecision(9) << "[" << p.x << "," << p.y;
  if (p.z!=0) s << "," << p.z;
  s << "]";
  return s;
}

/// Input operator: read point from a two-element json array
/// \relates Point
template <typename T>
std::istream & operator>> (std::istream & s, Point<T> & p){
  char sep;
  s >> std::ws >> sep;
  if (sep!='['){
    s.setstate(std::ios::failbit);
    return s;
  }
  s >> std::ws >> p.x >> std::ws >> sep;
  if (sep!=','){
    s.setstate(std::ios::failbit);
    return s;
  }
  s >> std::ws >> p.y >> std::ws >> sep >> std::ws;
  if (sep==','){
    s >> std::ws >> p.z >> std::ws >> sep >> std::ws;
  }
  if (sep!=']'){
    s.setstate(std::ios::failbit);
    return s;
  }
  s.setstate(std::ios::goodbit);
  return s;
}

/******************************************************************/
// type definitions

/// Point with double coordinates
/// \relates Point
typedef Point<double> dPoint;

/// Point with int coordinates
/// \relates Point
typedef Point<int> iPoint;

///@}
#endif
