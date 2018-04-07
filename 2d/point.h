#ifndef POINT_H
#define POINT_H

#include <iostream> // for << and >> operators
#include <iomanip>
#include <ios>
#include <cmath>    // for rint
#include "err/err.h" // for Err

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

  /// Constructor
  Point(T _x=0, T _y=0): x(_x), y(_y) { }

  /// Swap point with other one.
  void swap (Point & other) {
    std::swap (x, other.x);
    std::swap (y, other.y);
  }

  /******************************************************************/
  // operators +,-,/,*

  /// Subtract coordinates
  Point<T> & operator-= (Point const & other) { x -= other.x; y -= other.y; return *this; }

  /// Subtract coordinates
  Point<T> operator- (Point<T> const & other) const { return Point<T>(x-other.x, y-other.y); }

  /// Add coordinates
  Point<T> & operator+= (Point<T> const & other) { x += other.x; y += other.y; return *this; }

  /// Add coordinates
  Point<T> operator+ (Point<T> const & other) const { return Point<T>(x+other.x, y+other.y); }

  /// Divide coordinates by k
  Point<T> & operator/= (const T k) { x /= k; y /= k; return *this; }

  /// Divide coordinates by k
  Point<T> operator/ (const T k) const { return Point<T>(x/k, y/k); }

  /// Multiply coordinates by k
  Point<T> & operator*= (const T k) { x *= k; y *= k; return *this; }

  /// Multiply coordinates by k
  Point<T> operator* (T k) const { return Point<T>(x*k, y*k); }

  /// Invert point coordinates
  Point<T> operator- () const { return Point<T>(-x,-y); }

  /******************************************************************/
  // operators <=>

  /// Less then operator
  bool operator< (const Point<T> & other) const { return (x<other.x) || ((x==other.x)&&(y<other.y)); }

  /// Equal opertator: (x==other.x) && (y==other.y)
  bool operator== (const Point<T> & other) const { return (x==other.x)&&(y==other.y); }

  // derived operators:
  bool operator!= (const Point<T> & other) const { return !(*this==other); } ///< operator!=
  bool operator>= (const Point<T> & other) const { return !(*this<other);  } ///< operator>=
  bool operator<= (const Point<T> & other) const { return *this<other || *this==other; } ///< operator<=
  bool operator>  (const Point<T> & other) const { return !(*this<=other); } ///< operator>

  /******************************************************************/

  /// Cast to Point<double>
  operator Point<double>() const{
    return Point<double>(double(this->x), double(this->y));
  }

  /// Cast to Point<int>
  operator Point<int>() const{
    return Point<int>(int(this->x), int(this->y));
  }

  /******************************************************************/

  /// Calculate manhattan length: abs(x) + abs(y)
  T mlen() const { return ::abs(x) + ::abs(y); }

  /// Calculate length: sqrt(x^2 + y^2).
  double len() const { return sqrt((double)(x*x+y*y));}

  /// Normalize: *this/len(*this). Error if point is 0
  /// Throw error i
  Point<double> norm() const {
     if (x==0 && y==0) throw Err() << "Point norm: zero length";
     return Point<double>(*this)/this->len();
  }

  /// rint function: change coordinates to the nearest integers
  Point<T> rint() const { return Point<T>((T)::rint(x),(T)::rint(y)); }

  /// floor function: change coordinates to nearest smaller integers
  Point<T> floor() const { return Point<T>((T)::floor(x),(T)::floor(y)); }

  /// ceil function: change coordinates to nearest larger integers
  Point<T> ceil() const { return Point<T>((T)::ceil(x),(T)::ceil(y)); }

  /// abs function: change coordinates to their absolute values
  Point<T> abs() const { return Point<T>(x>0?x:-x, y>0?y:-y); }

};

/******************************************************************/

/// Calculate manhattan length: abs(x) + abs(y)
/// \relates Point
template <typename T>
T mlen (const Point<T> & p) { return p.mlen(); }

/// Calculate length: sqrt(x^2 + y^2)
/// \relates Point
template <typename T>
double len(const Point<T> & p) { return p.len();}

/// Normalize: *this/len(*this). Throw error if point is 0
/// \relates Point
template <typename T>
Point<double> norm(const Point<T> & p) { return p.norm(); }

/// rint function
/// \relates Point
template <typename T>
Point<T> rint(const Point<T> & p){ return p.rint(); }

/// floor function
/// \relates Point
template <typename T>
Point<T> floor(const Point<T> & p){ return p.floor(); }

/// ceil function
/// \relates Point
template <typename T>
Point<T> ceil(const Point<T> & p){ return p.ceil(); }

/// abs function
/// \relates Point
template <typename T>
Point<T> abs(const Point<T> & p){ return p.abs(); }

/******************************************************************/

/// Scalar multiplication: p1.x*p2.x + p1.y*p2.y
/// \relates Point
template <typename T>
double pscal(const Point<T> & p1, const Point<T> & p2){
  return p1.x*p2.x + p1.y*p2.y;
}

/// Distance between two points: (p1-p2).len()
/// \relates Point
template <typename T>
double dist(const Point<T> & p1, const Point<T> & p2){
  return (p1-p2).len();
}

/******************************************************************/

/// Output operator: print point as a two-element json array
/// \relates Point
template <typename T>
std::ostream & operator<< (std::ostream & s, const Point<T> & p){
  s << std::setprecision(8) << "[" << p.x << "," << p.y << "]";
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
  if (sep!=']'){
    s.setstate(std::ios::failbit);
    return s;
  }
  s.setstate(std::ios::goodbit);
  return s;
}

/******************************************************************/

/// Point with double coordinates
/// \relates Point
typedef Point<double> dPoint;

/// Point with int coordinates
/// \relates Point
typedef Point<int> iPoint;

#endif
