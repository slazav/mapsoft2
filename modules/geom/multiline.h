#ifndef MULTILINE_H
#define MULTILINE_H

#include <iostream>
#include <ios>
#include <cmath>
#include <list>
#include <vector>
#include "line.h"
#include "point.h"

///\addtogroup libmapsoft
///@{

template <typename T>
class MultiLine;
MultiLine<double> string_to_mline(const std::string & s);


/// Line with multiple segments (std::vector of Line)
template <typename T>
struct MultiLine : std::vector<Line<T> > {

  /// Constructor: make an empty line
  MultiLine() {}

  /// Constructor: make a line using string "[ [[x1,y1],[x2,y2]] , [[x3,y4],[x5,y5]]]"
  MultiLine(const std::string & s) { *this = string_to_mline(s);}

  /******************************************************************/
  // operators +,-,/,*

  /// Add p to every point (shift the line)
  MultiLine<T> & operator+= (const Point<T> & p) {
    for (typename MultiLine<T>::iterator i=this->begin(); i!=this->end(); i++) (*i)+=p;
    return *this;
  }

  /// Subtract p from every point of line
  MultiLine<T> & operator-= (const Point<T> & p) {
    for (typename MultiLine<T>::iterator i=this->begin(); i!=this->end(); i++) (*i)-=p;
    return *this;
  }

  /// Divide coordinates by k
  MultiLine<T> & operator/= (const T k) {
    for (typename MultiLine<T>::iterator i=this->begin(); i!=this->end(); i++) (*i)/=k;
    return *this;
  }

  /// Multiply coordinates by k
  MultiLine<T> & operator*= (const T k) {
    for (typename MultiLine<T>::iterator i=this->begin(); i!=this->end(); i++) (*i)*=k;
    return *this;
  }

  /// Add p to every point (shift the line)
  MultiLine<T> operator+ (const Point<T> & p) const { MultiLine<T> ret(*this); return ret+=p; }

  /// Subtract p from every point (shift the line)
  MultiLine<T> operator- (const Point<T> & p) const { MultiLine<T> ret(*this); return ret-=p; }

  /// Divide coordinates by k
  MultiLine<T> operator/ (const T k) const { MultiLine<T> ret(*this); return ret/=k; }

  /// Multiply coordinates by k
  MultiLine<T> operator* (const T k) const { MultiLine<T> ret(*this); return ret*=k; }

  /// Invert coordinates
  MultiLine<T> operator- () const {
    MultiLine<T> ret;
    for (typename MultiLine<T>::const_iterator i=this->begin(); i!=this->end(); i++)
      ret.push_back(-(*i));
    return ret;
  }

  /******************************************************************/
  // operators <=>
  /// Less then operator.
  /// L1 is smaller then L2 if first different line in L1 is smaller or does not exist.
  bool operator< (const MultiLine<T> & p) const {
    typename MultiLine<T>::const_iterator i1=this->begin(), i2=p.begin();
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
  bool operator== (const MultiLine<T> & p) const {
    if (this->size()!=p.size()) return false;
    typename MultiLine<T>::const_iterator i1=this->begin(), i2=p.begin();
    do {
      if (i1==this->end()) return true;
      if ((*i1)!=(*i2)) return false;
      i1++; i2++;
    } while(1);
  }

  // derived operators:
  bool operator!= (const MultiLine<T> & other) const { return !(*this==other); } ///< operator!=
  bool operator>= (const MultiLine<T> & other) const { return !(*this<other);  } ///< operator>=
  bool operator<= (const MultiLine<T> & other) const { return *this<other || *this==other; } ///< ope
  bool operator>  (const MultiLine<T> & other) const { return !(*this<=other); } ///< operator>

  /******************************************************************/

  /// Cast to MultiLine<double>.
  operator MultiLine<double>() const{
    MultiLine<double> ret;
    for (typename MultiLine<T>::const_iterator i=this->begin(); i!=this->end(); i++)
      ret.push_back(dLine(*i));
    return ret;
  }

  /// Cast to MultiLine<int>.
  operator MultiLine<int>() const{
    MultiLine<int> ret;
    for (typename MultiLine<T>::const_iterator i=this->begin(); i!=this->end(); i++)
      ret.push_back(iLine(*i));
    return ret;
  }

  /******************************************************************/
  // Some functions. Below same functions are defined outside the class

  /// MultiLine length (sum of segment lengths).
  double length () const {
    double ret=0;
    typename MultiLine<T>::const_iterator i;
    for(i=this->begin(); i!=this->end(); i++) ret+=i->length();
    return ret;
  }

  /// MultiLine bounding box.
  Rect<T> bbox2d() const{
    if (this->size()<1) return Rect<T>();
    typename MultiLine<T>::const_iterator i=this->begin();
    Rect<T> ret=i->bbox2d();
    while ((++i) != this->end())  ret = expand(ret, i->bbox2d());
    return ret;
  }

  /// rint function: change corner coordenates to nearest integers
  MultiLine<T> rint() const{
    MultiLine<T> ret;
    for (typename MultiLine<T>::const_iterator i=this->begin(); i!=this->end(); i++)
      ret.push_back(i->rint());
    return ret;
  }

  /// rotate the MultiLine around c at the angle a (rad, clockwise)
  MultiLine rotate2d(const Point<T> & c, const double a) const {
    double C=cos(a), S=sin(a);
    MultiLine ret(*this);
    for (typename MultiLine::iterator i=ret.begin(); i!=ret.end(); i++)
      for (typename Line<T>::iterator j=i->begin(); j!=i->end(); j++)
        *j = Point<T>(C*(j->x-c.x)+S*(j->y-c.y), C*(j->y-c.y)-S*(j->x-c.x)) + c;
    return ret;
  }

  /// Project the multiline into x-y plane
  MultiLine flatten() const {
    MultiLine ret;
    for (typename MultiLine<T>::const_iterator i=this->begin(); i!=this->end(); i++)
      ret.push_back(i->flatten());
    return ret;
  }

};

/******************************************************************/
// additional operators

/// Multiply coordinates by k (k*multiline = multiline*k)
/// \relates MultiLine
template <typename T>
MultiLine<T> operator* (const T k, const MultiLine<T> & l) { return l*k; }

/// Add p to every point (shift the line) (p+multiline = multiline+p)
/// \relates MultiLine
template <typename T>
MultiLine<T> operator+ (const Point<T> & p, const MultiLine<T> & l) { return l+p; }

/******************************************************************/
// functions, similar to ones inside the class

/// Calculate MultiLine length.
/// \relates MultiLine
template <typename T>
double length(const MultiLine<T> & l){ return l.length(); }

/// MultiLine bounding box
/// \relates MultiLine
template <typename T>
Rect<T> bbox2d(const MultiLine<T> & l) { return l.bbox2d(); }

/// rint function: change corner coordenates to nearest integers
/// \relates Line
template <typename T>
MultiLine<T> rint(const MultiLine<T> & l) { return l.rint(); }

/// rotate a MultiLine around c at the angle a (rad)
template <typename T>
MultiLine<T> rotate2d(const MultiLine<T> & l, const Point<T> & c, const double a) { return l.rotate2d(c,a); }

/// Project the multiline to x-y plane.
template <typename T>
MultiLine<T> flatten(const MultiLine<T> & l) { return l.flatten(); }

/******************************************************************/
// input/output

/// \relates MultiLine
/// \brief Output operator: print Line as a JSON array of lines
template <typename T>
std::ostream & operator<< (std::ostream & s, const MultiLine<T> & l){
  s << "[";
  for (typename MultiLine<T>::const_iterator i=l.begin(); i!=l.end(); i++)
    s << ((i==l.begin())? "":",") << *i;
  s << "]";
  return s;
}

/// \brief Input operator: read Line from a JSON array of points
/// \note This >> operator is different from that in
/// Point or Rect. It always reads the whole stream and
/// returns error if there are extra characters.
/// No possibility to read two objects from one stream.

/// \todo Move json code somewhere else.

/// \relates MultiLine
template <typename T>
std::istream & operator>> (std::istream & s, MultiLine<T> & ml){
  // read the whole stream into a string
  std::ostringstream os;
  s>>os.rdbuf();
  ml=string_to_mline(os.str());
  return s;
}


/******************************************************************/
// type definitions

/// MultiLine with double coordinates
/// \relates MultiLine
typedef MultiLine<double> dMultiLine;

/// MultiLine with int coordinates
/// \relates MultiLine
typedef MultiLine<int> iMultiLine;

///@}
#endif
