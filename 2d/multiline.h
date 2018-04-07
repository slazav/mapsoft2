#ifndef MULTILINE_H
#define MULTILINE_H

#include <iostream>
//#include <ios>
#include <cmath>
#include <list>
#include <vector>
#include "point.h"

///\addtogroup mapsoft
///@{

/// 2d line:  std::vector<Point<T> >.

/// Line with multiple segments (std::vector<Line<T> >)
template <typename T>
struct MultiLine : std::vector<Line<T> >
#ifndef SWIG
    , public boost::additive<MultiLine<T> >,
    public boost::additive<MultiLine<T>, Point<T> >,
    public boost::multiplicative<MultiLine<T>,T>,
    public boost::less_than_comparable<MultiLine<T> >,
    public boost::equality_comparable<MultiLine<T> >
#endif  //SWIG
{

  /// Divide coordinates by k.
  MultiLine<T> & operator/= (T k) {
    for (typename MultiLine<T>::iterator i=this->begin(); i!=this->end(); i++) (*i)/=k;
    return *this;
  }

  /// Multiply coordinates by k.
  MultiLine<T> & operator*= (T k) {
    for (typename MultiLine<T>::iterator i=this->begin(); i!=this->end(); i++) (*i)*=k;
    return *this;
  }

  /// Add p to every point of line.
  MultiLine<T> & operator+= (Point<T> p) {
    for (typename MultiLine<T>::iterator i=this->begin(); i!=this->end(); i++) (*i)+=p;
    return *this;
  }

  /// Subtract p from every point of line.
  MultiLine<T> & operator-= (Point<T> p) {
    for (typename MultiLine<T>::iterator i=this->begin(); i!=this->end(); i++) (*i)-=p;
    return *this;
  }

  /// Calculate sum of line lengths.
  double length () const {
    double ret=0;
    for(typename MultiLine<T>::const_iterator i=this->begin(); i!=this->end(); i++)
      ret+=i->length();
    return ret;
  }

#ifndef SWIG
  /// Less then operator.
  /// меньше, если первая отличающаяся линия меньше,
  /// или не существует
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
#endif

  /// MultiLine range.
  Rect<T> range() const{
    if (this->size()<1) return Rect<T>(0,0,0,0);
    typename MultiLine<T>::const_iterator i=this->begin();
    Rect<T> ret=i->range();
    while ((++i) != this->end())  ret = rect_bounding_box(ret, i->range());
    return ret;
  }

  /// Center of the range.
  /// \todo use the same name for Line, MultiLine and Rect?
  Point<T> center() const{
    return range().CNT();
  }

#ifndef SWIG
  /// Cast to MultiLine<double>
  operator MultiLine<double>() const{
    MultiLine<double> ret;
    for (typename MultiLine<T>::const_iterator i=this->begin(); i!=this->end(); i++)
      ret.push_back(dLine(*i));
    return ret;
  }

  /// Cast to MultiLine<int>
  operator MultiLine<int>() const{
    MultiLine<int> ret;
    for (typename MultiLine<T>::const_iterator i=this->begin(); i!=this->end(); i++)
      ret.push_back(iLine(*i));
    return ret;
  }
#else  //SWIG
  %extend {
    MultiLine<T> operator+ (Point<T> &p) { return *$self + p; }
    MultiLine<T> operator- (Point<T> &p) { return *$self - p; }
    MultiLine<T> operator* (T p) { return *$self * p; }
    MultiLine<T> operator/ (T p) { return *$self / p; }
    swig_cmp(MultiLine<T>);
  }
#endif  //SWIG  
};

/// MultiLine with double coordinates
/// \relates MultiLine
typedef MultiLine<double> dMultiLine;

/// MultiLine with int coordinates
/// \relates MultiLine
typedef MultiLine<int>    iMultiLine;

#endif
