#ifndef LINE_H
#define LINE_H

#include <iostream>
#include <algorithm>
#include <ios>
#include <cmath>
#include <list>
#include <vector>
#include "point.h"
#include "rect.h"
#include "err/err.h"
#include "opt/opt.h"

///\addtogroup libmapsoft
///@{

template <typename T> class Line;
/// convert string to line
Line<double> string_to_line(const std::string & s);

/// 2d line: std::vector of Point.
template <typename T>
struct Line : std::vector<Point<T> > {

  /// Constructor: make an empty line
  Line() {}

  /// Constructor: make a line using string "[[x1,y1],[x2,y2]]"
  Line(const std::string & s) { *this = string_to_line(s);}

  /******************************************************************/
  // operators +,-,/,*

  /// Add p to every point (shift the line)
  Line<T> & operator+= (const Point<T> & p) {
    for (typename Line<T>::iterator i=this->begin(); i!=this->end(); i++) (*i)+=p;
    return *this;
  }

  /// Subtract p from every point of line
  Line<T> & operator-= (const Point<T> & p) {
    for (typename Line<T>::iterator i=this->begin(); i!=this->end(); i++) (*i)-=p;
    return *this;
  }

  /// Divide coordinates by k
  Line<T> & operator/= (const T k) {
    for (typename Line<T>::iterator i=this->begin(); i!=this->end(); i++) (*i)/=k;
    return *this;
  }

  /// Multiply coordinates by k
  Line<T> & operator*= (const T k) {
    for (typename Line<T>::iterator i=this->begin(); i!=this->end(); i++) (*i)*=k;
    return *this;
  }

  /// Add p to every point (shift the line)
  Line<T> operator+ (const Point<T> & p) const { Line<T> ret(*this); return ret+=p; }

  /// Subtract p from every point (shift the line)
  Line<T> operator- (const Point<T> & p) const { Line<T> ret(*this); return ret-=p; }

  /// Divide coordinates by k
  Line<T> operator/ (const T k) const { Line<T> ret(*this); return ret/=k; }

  /// Multiply coordinates by k
  Line<T> operator* (const T k) const { Line<T> ret(*this); return ret*=k; }

  /// Invert coordinates
  Line<T> operator- () const {
    Line<T> ret;
    for (typename Line<T>::const_iterator i=this->begin(); i!=this->end(); i++)
      ret.push_back(-(*i));
    return ret;
  }

  /******************************************************************/
  // operators <=>

  /// Less then operator.
  /// L1 is smaller then L2 if first different point in L1 is smaller or does not exist.
  bool operator< (const Line<T> & p) const {
    typename Line<T>::const_iterator i1=this->begin(), i2=p.begin();
    do {
      if (i1==this->end()){
        if (i2==p.end()) return false;
        else return true;
      }
      else if (i2==p.end()) return false;

      if ((*i1)!=(*i2)) return (*i1) < (*i2);
      i1++; i2++;
    } while(1);
  }

  /// Equal opertator.
  bool operator== (const Line<T> & p) const {
    if (this->size()!=p.size()) return false;
    typename Line<T>::const_iterator i1=this->begin(), i2=p.begin();
    do {
      if (i1==this->end()) return true;
      if ((*i1)!=(*i2)) return false;
      i1++; i2++;
    } while(1);
  }

  /// Same comparison as == but for opposite directions.
  bool is_inv(const Line<T> & p) const {
    if (this->size()!=p.size()) return false;
    typename Line<T>::const_iterator i1=this->begin();
    typename Line<T>::const_reverse_iterator  i2=p.rbegin();
    do {
      if (i1==this->end()) return true;
      if ((*i1)!=(*i2)) return false;
      i1++; i2++;
    } while(1);
  }

  // derived operators:
  bool operator!= (const Line<T> & other) const { return !(*this==other); } ///< operator!=
  bool operator>= (const Line<T> & other) const { return !(*this<other);  } ///< operator>=
  bool operator<= (const Line<T> & other) const { return *this<other || *this==other; } ///< operator<
  bool operator>  (const Line<T> & other) const { return !(*this<=other); } ///< operator>

  /******************************************************************/

  /// Cast to Line<double>
  operator Line<double>() const{
    Line<double> ret;
    for (auto const & p:*this) ret.push_back(dPoint(p));
    return ret;
  }

  /// Cast to Line<int>
  operator Line<int>() const{
    Line<int> ret;
    for (auto const & p:*this) ret.push_back(iPoint(p));
    return ret;
  }

  /******************************************************************/
  // Some functions. Below same functions are defined outside the class

  /// Calculate line length.
  double length() const {
    double ret=0;
    for (int i=1; i<this->size(); i++)
      ret+=dist((*this)[i-1], (*this)[i]);
    return ret;
  }

  /// Calculate line length.
  double length2d() const {
    double ret=0;
    for (int i=1; i<this->size(); i++)
      ret+=dist2d((*this)[i-1], (*this)[i]);
    return ret;
  }

  /// Line bounding box in x-y plane
  Rect<T> bbox() const{
    if (this->size()<1) return Rect<T>();
    Point<T> min((*this)[0]), max((*this)[0]);

    for (auto const & p:*this){
      if (p.x > max.x) max.x = p.x;
      if (p.y > max.y) max.y = p.y;
      if (p.x < min.x) min.x = p.x;
      if (p.y < min.y) min.y = p.y;
    }
    return Rect<T>(min,max);
  }

  /// Is line l just shifted version of this. Shift is returned
  bool is_shifted(const Line<T> & l, Point<T> & shift) const{
    shift = Point<T>(0,0);
    if (this->size()!=l.size()) return false;
    if (this->size()==0) return true;
    typename Line<T>::const_iterator i1=this->begin(), i2=l.begin();
    shift = (*i2) - (*i1);
    do {
      if (i1==this->end()) return true;
      if ((*i2)-(*i1) != shift) return false;
      i1++; i2++;
    } while(1);
  }

  /// Is line empty
  bool is_empty() const {return this->size() == 0;}

  /// Invert line.
  void invert(void) { std::reverse(this->begin(), this->end()); }

  /// rint function: change corner coordinates to nearest integers
  void to_rint() { for (auto & p:*this) p.to_rint(); }

  /// floor function: change coordinates to nearest smaller integers
  void to_floor() { for (auto & p:*this) p.to_floor(); }

  /// ceil function: change coordinates to nearest larger integers
  void to_ceil() { for (auto & p:*this) p.to_ceil(); }

  /// abs function: change coordinates to their absolute values
  void to_abs() { for (auto & p:*this) p.to_abs(); }

  /// Rotate the line around c at the angle a (rad, clockwise) in x-y plane.
  /// Here we do not use Point::rotate2d to calculate sin/cos only ones
  /// and make things faster.
  void rotate2d(const Point<T> & c, const double a) {
    double C=cos(a), S=sin(a);
    for (auto & p:*this)
      p=Point<T>(C*(double)(p.x-c.x)+S*(double)(p.y-c.y)+c.x,
                 C*(double)(p.y-c.y)-S*(double)(p.x-c.x)+c.y, p.z);
  }

  /// Project the line into x-y plane
  void flatten() { for (auto & p:*this) p.z=0; }

  // "close" the line: add last point equals to the first one
  // (if it is not equal)
  void close(){
    if (this->size()>1 && *(this->rbegin())!=*(this->begin()))
      this->push_back(*(this->begin()));
  }

  // "open" the line: if the last point equals to the first one
  // then remove it.
  void open(){
    if (this->size()>1 && *(this->rbegin())==*(this->begin()))
      this->resize(this->size()-1);
  }

  // flip the line around y=y0 line
  void flip_y(const T y0=0){
    for (auto & p:*this) p.y = y0 - p.y;
  }

  // flip the line around x=x0 line
  void flip_x(const T x0=0){
    for (auto & p:*this) p.x = x0 - p.x;
  }

};

/******************************************************************/
// additional operators

/// Multiply coordinates by k (k*line = line*k)
/// \relates Line
template <typename T>
Line<T> operator* (const T k, const Line<T> & l) { return l*k; }

/// Add p to every point (p+line = line+p)
/// \relates Line
template <typename T>
Line<T> operator+ (const Point<T> & p, const Line<T> & l) { return l+p; }

/******************************************************************/
// functions, similar to ones in the class

/// Calculate line length.
/// \relates Line
template <typename T>
double length(const Line<T> & l){ return l.length(); }

/// Calculate 2D line length.
/// \relates Line
template <typename T>
double length2d(const Line<T> & l){ return l.length2d(); }

/// Line bounding box
/// \relates Line
template <typename T>
Rect<T> bbox(const Line<T> & l) { return l.bbox(); }

/// Is line l just shifted version of this. Shift is returned
/// \relates Line
template <typename T>
bool is_shifted(const Line<T> & l1, const Line<T> & l2, Point<T> & shift){
  return l1.is_shifted(l2, shift);
}


/// Invert line.
/// \relates Line
template <typename T>
Line<T> invert(const Line<T> & l) {
  Line<T> ret(l);
  std::reverse(ret.begin(), ret.end());
  return ret;
}

/// rint function: change corner coordenates to nearest integers
/// \relates Line
template <typename T>
Line<int> rint(const Line<T> & l) {
  Line<int> ret;
  for (auto const & p:l) ret.push_back(rint(p));
  return ret;
}

/// floor function: change coordinates to nearest smaller integers
/// \relates Line
template <typename T>
Line<int> floor(const Line<T> & l) {
  Line<int> ret;
  for (auto const & p:l) ret.push_back(floor(p));
  return ret;
}

/// ceil function: change coordinates to nearest larger integers
/// \relates Line
template <typename T>
Line<int> ceil(const Line<T> & l) {
  Line<int> ret;
  for (auto const & p:l) ret.push_back(ceil(p));
  return ret;
}

/// abs function: change coordinates to their absolute values
template <typename T>
Line<T> abs(const Line<T> & l) {
  Line<T> ret(l);
  for (auto & p:ret) p.to_abs();
  return ret;
}

/// rotate a line around c at the angle a (rad)
template <typename T>
Line<T> rotate2d(const Line<T> & l, const Point<T> & c, const double a) {
  Line<T> ret(l);
  ret.rotate2d(c,a);
  return ret;
}

/// Project the line to x-y plane.
template <typename T>
Line<T> flatten(const Line<T> & l) {
  Line<T> ret(l);
  for (auto & p:ret) p.z=0;
  return ret;
}

// "close" the line: add last point equals to the first one
// (if it is not equal)
template <typename T>
Line<T> close(const Line<T> & l){
  Line<T> ret(l); ret.close(); return ret; }

// "open" the line: if the last point equals to the first one
// then remove it.
template <typename T>
Line<T> open(const Line<T> & l){
  Line<T> ret(l); ret.open(); return ret; }

// flip the line around y=y0 line
template <typename T>
Line<T> flip_y(const Line<T> & l, const T y0=0){
  Line<T> ret(l); ret.flip_y(y0); return ret; }

// flip the line around x=x0 line
template <typename T>
Line<T> flip_x(const Line<T> & l, const T x0=0){
  Line<T> ret(l); ret.flip_x(x0); return ret; }

/******************************************************************/
// additional functions

/// Convert rectangle to a line. Line goes clockwise,
/// starting from top-left corner (<tlc>, <trc>, <brc>, <blc>, and
/// if <closed> parameter is true back to <tlc>).
/// \relates Line
template <typename T>
Line<T> rect_to_line(const Rect<T> & r, bool closed=true) {
  Line<T> ret;
  ret.push_back(r.tlc());
  ret.push_back(r.trc());
  ret.push_back(r.brc());
  ret.push_back(r.blc());
  if (closed) ret.push_back(r.tlc());
  return ret;
}

/// Distance between two lines A and B: sqrt(sum(dist(A[i],B[i])^2)).
/// Returns +inf for lines fith different number of points.
template <typename T>
double dist(const Line<T> & A, const Line<T> & B){
  double ret = 0;
  if (A.size() != B.size()) return INFINITY;
  for (int i=0; i<A.size(); i++) ret+=pow(dist(A[i],B[i]),2);
  return sqrt(ret);
}

/******************************************************************/
// input/output

/// \relates Line
/// \brief Output operator: print Line as a JSON array of points
template <typename T>
std::ostream & operator<< (std::ostream & s, const Line<T> & l){
  s << "[";
  for (typename Line<T>::const_iterator i=l.begin(); i!=l.end(); i++)
    s << ((i==l.begin())? "":",") << *i;
  s << "]";
  return s;
}

/// Input operator: read Line from a JSON array of points
/// This >> operator is different from that in
/// Point or Rect. It always reads the whole stream and
/// returns error if there are extra characters.
/// No possibility to read two objects from one stream.
/// \relates Line
template <typename T>
std::istream & operator>> (std::istream & s, Line<T> & l){
  // read the whole stream into a string
  std::ostringstream os;
  s>>os.rdbuf();
  l=string_to_line(os.str());
  return s;
}

/******************************************************************/
// type definitions

/// Line with double coordinates
/// \relates Line
typedef Line<double> dLine;

/// Line with int coordinates
/// \relates Line
typedef Line<int> iLine;

///@}
#endif
