#ifndef LINE_H
#define LINE_H

#include <iostream>
#include <ios>
#include <cmath>
#include <list>
#include <vector>
#include "point.h"
#include "rect.h"
#include "err/err.h"
#include "opt/opt.h"
#include "jansson.h"

///\addtogroup libmapsoft
///@{


/// 2d line: std::vector of Point.
template <typename T>
struct Line : std::vector<Point<T> > {

  /// Constructor: make an empty line
  Line() {}

  /// Constructor: make a line using string "[[x1,y1],[x2,y2]]"
  Line(const std::string & s) { *this = str_to_type<Line>(s);}

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
  Line<T> & operator- () const {
    for (typename Line<T>::iterator i=this->begin(); i!=this->end(); i++) (*i)=-(*i);
    return *this;
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
  bool operator!= (const Point<T> & other) const { return !(*this==other); } ///< operator!=
  bool operator>= (const Point<T> & other) const { return !(*this<other);  } ///< operator>=
  bool operator<= (const Point<T> & other) const { return *this<other || *this==other; } ///< operator<
  bool operator>  (const Point<T> & other) const { return !(*this<=other); } ///< operator>

  /******************************************************************/

  /// Cast to Line<double>
  operator Line<double>() const{
    Line<double> ret;
    for (typename Line<T>::const_iterator i=this->begin(); i!=this->end(); i++)
      ret.push_back(dPoint(*i));
    return ret;
  }

  /// Cast to Line<int>
  operator Line<int>() const{
    Line<int> ret;
    for (typename Line<T>::const_iterator i=this->begin(); i!=this->end(); i++)
      ret.push_back(iPoint(*i));
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

  /// Invert line.
  Line<T> invert(void) const{
    Line<T> ret;
    for (typename Line<T>::const_reverse_iterator i=this->rbegin();
                              i!=this->rend(); i++) ret.push_back(*i);
    return ret;
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

  /// Line bounding box
  Rect<T> bbox() const{
    if (this->size()<1) return Rect<T>();
    Point<T> min((*this)[0]), max((*this)[0]);

    for (typename Line<T>::const_iterator i = this->begin(); i!=this->end(); i++){
      if (i->x > max.x) max.x = i->x;
      if (i->y > max.y) max.y = i->y;
      if (i->x < min.x) min.x = i->x;
      if (i->y < min.y) min.y = i->y;
    }
    return Rect<T>(min,max);
  }

  /// rint function: change corner coordenates to nearest integers
  Line<T> rint() const{
    Line<T> ret;
    for (typename Line<T>::const_iterator i=this->begin(); i!=this->end(); i++)
      ret.push_back(i->rint());
    return ret;
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

/// Invert line.
/// \relates Line
template <typename T>
Line<T> invert(const Line<T> & l) { return l.invert(); }

/// Is line l just shifted version of this. Shift is returned
/// \relates Line
template <typename T>
bool is_shifted(const Line<T> & l1, const Line<T> & l2, Point<T> & shift){
  return l1.is_shifted(l2, shift);
}

/// Line bounding box
/// \relates Line
template <typename T>
Rect<T> bbox(const Line<T> & l) { return l.bbox(); }

/// rint function: change corner coordenates to nearest integers
/// \relates Line
template <typename T>
Line<T> rint(const Line<T> & l) { return l.rint(); }

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
  std::string str=os.str();

  json_error_t e;
  json_t *J = json_loadb(str.data(), str.size(), 0, &e);
  l.clear();  // clear old contents

  try {
    if (!J)
      throw Err() << e.text;
    if (!json_is_array(J))
      throw Err() << "Reading line: a JSON array is expected";

    json_t *P;
    size_t index;
    json_array_foreach(J, index, P){
      if (!json_is_array(P) || json_array_size(P)!=2)
        throw Err() << "Reading line point: a JSON two-element array is expected";
      json_t *X = json_array_get(P, 0);
      json_t *Y = json_array_get(P, 1);
      if (!X || !Y || !json_is_number(X) || !json_is_number(Y))
        throw Err() << "Reading line point: a numerical values expected";
      l.push_back(Point<T>(json_number_value(X), json_number_value(Y)));
    }
  }
  catch (Err e){
    json_decref(J);
    throw e;
  }
  json_decref(J);
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
