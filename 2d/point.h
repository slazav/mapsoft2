#ifndef MAPSOFT_POINT_H
#define MAPSOFT_POINT_H

#include <iostream> // for << and >> operators
#include <cmath>    // for rint

///\addtogroup 2d
///@{
///\defgroup point
///@{

/// 2-d point
template <typename T>
struct Point {
  T x; ///< x coordinate
  T y; ///< y coordinate

  /// Constructors
  Point(T _x, T _y): x(_x), y(_y) { }
  Point(): x(0), y(0) { }

  /// Swap point with other one.
  void swap (Point & other) {
    std::swap (x, other.x);
    std::swap (y, other.y);
  }

  /// Subtract corresponding coordinates: p=Point(p.x-other.x, p.y-other.y).
  Point<T> & operator-= (Point const & other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  Point<T> & operator- (Point const & other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  /// Add corresponding coordinates: p=Point(p.x+other.x, p.y+other.y).
  Point<T> & operator+= (Point const & other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  Point<T> & operator+ (Point const & other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  /// Divide coordinates by k: p=Point(p.x/k, p.y/k).
  Point<T> & operator/= (T k) {
    x /= k;
    y /= k;
    return *this;
  }
  Point<T> & operator/ (T k) {
    x /= k;
    y /= k;
    return *this;
  }

  /// Multiply coordinates by k: p=Point(p.x*k, p.y*k).
  Point<T> & operator*= (T k) {
    x *= k;
    y *= k;
    return *this;
  }
  Point<T> & operator* (T k) {
    x *= k;
    y *= k;
    return *this;
  }

  /// Invert point coordinates.
  Point<T> & operator- () {
    x = -x;
    y = -y;
    return *this;
  }

  /// Less then operator: (x < other.x)  || ((x == other.x) && (y < other.y)).
  bool operator< (const Point<T> & other) const {
    return (x<other.x) || ((x==other.x)&&(y<other.y));
  }

  /// Equal opertator: (x==other.x) && (y==other.y).
  bool operator== (const Point<T> & other) const {
    return (x==other.x)&&(y==other.y);
  }

  /// Equal opertator: (x==other.x) && (y==other.y).
  bool operator!= (const Point<T> & other) const {
    return (x!=other.x)||(y!=other.y);
  }

  /// Cast to Point<double>.
  operator Point<double>() const{
    return Point<double>(double(this->x), double(this->y));
  }

  /// Cast to Point<int>.
  operator Point<int>() const{
    return Point<int>(int(rint(this->x)), int(rint(this->y)));
  }
//  Point<T> operator+ (Point<T> const & t) { return *$self + t; }
//  Point<T> operator- (Point<T> const & t) { return *$self - t; }
//  Point<T> operator/ (T k) { return *$self / k; }
//  Point<T> operator* (T k) { return *$self * k; }

  /// Calculate manhattan length: abs(x)+abs(y).
  T mlen () const { return abs(x) + abs(y); }
  double len() const { return sqrt((double)(x*x+y*y));}
  Point<T> abs() const { return Point<T>(abs(x), abs(y));}

};

/// \relates Point
/// \brief Output operator: print point as a comma-separated pair of coordinartes.
template <typename T>
std::ostream & operator<< (std::ostream & s, const Point<T> & p){
  s << p.x << "," << p.y;
  return s;
}

/// \relates Point
/// \brief Input operator: read point from a comma-separated pair of coordainates.
template <typename T>
std::istream & operator>> (std::istream & s, Point<T> & p){
  char sep;
  s >> std::ws >> p.x >> std::ws >> sep;
  if (sep!=','){
    s.setstate(std::ios::failbit);
    return s;
  }
  s >> std::ws >> p.y >> std::ws;
  s.setstate(std::ios::goodbit);
  return s;
}

/// \relates Point
/// \brief Scalar multiplication: p1.x*p2.x + p1.y*p2.y.
template <typename T>
double pscal(const Point<T> & p1, const Point<T> & p2){
  return p1.x*p2.x + p1.y*p2.y;
}
/// \relates Point
/// \brief Length: sqrt(double(p.x*p.x + p.y*p.y)).
/// \todo move to Point class?
template <typename T>
double pdist(const Point<T> & p){
  return sqrt((double)(pscal(p,p)));
}
/// \relates Point
/// \brief Distance: pdist(p1-p2).
template <typename T>
double pdist(const Point<T> & p1, const Point<T> & p2){
  return pdist(p1-p2);
}
/// \relates Point
/// \brief Normalize.
/// \todo move to Point class?
template <typename T>
Point<double> pnorm(const Point<T> & p){
  double l = pdist(p);
  return Point<double>(double(p.x)/l, double(p.y/l));
}
/// \relates Point
/// \brief Absolute value.
/// \todo move to Point class?
template <typename T>
Point<T> pabs(const Point<T> & p){
  return Point<T>(
    p.x>0?p.x:-p.x,
    p.y>0?p.y:-p.y
  );
}

/// \relates Point
/// \brief Point with double coordinates
typedef Point<double> dPoint;
/// \relates Point
/// \brief Point with int coordinates
typedef Point<int>    iPoint;

#endif /* MAPSOFT_POINT_H */
