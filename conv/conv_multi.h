#ifndef CONV_MULTI_H
#define CONV_MULTI_H

#include "conv.h"
#include <list>

///\addtogroup lib2d
///@{
///\defgroup conv_multi
///@{

/// Composite conversion
/// User must keep all parts until this conversion is used
class ConvMulti : public Conv {

  std::list<const Conv *> cnvs;
  std::list<bool>   dirs;

public:

  /// constructor - trivial transformation
  ConvMulti(){}

  /// constructor - 2 conversions
  ConvMulti(const Conv * c1, const Conv * c2, bool dir1=true, bool dir2=true){
    cnvs.push_back(c1);
    cnvs.push_back(c2);
    dirs.push_back(dir1);
    dirs.push_back(dir2);
  }

  void push_front (const Conv * c, bool d=true){
    cnvs.push_front(c);
    dirs.push_front(d);
  }
  void push_back (const Conv * c, bool d=true){
    cnvs.push_back(c);
    dirs.push_back(d);
  }

  void frw(dPoint & p) const{
    p/=Conv::sc_src;
    std::list<const Conv *>::const_iterator c;
    std::list<bool>::const_iterator d;
    for (c=cnvs.begin(), d=dirs.begin(); c!=cnvs.end(); c++, d++)
      if (*d) (*c)->frw(p); else (*c)->bck(p);
    p*=Conv::sc_dst;
  }

  void bck(dPoint & p) const{
    p/=Conv::sc_dst;
    std::list<const Conv *>::const_reverse_iterator c;
    std::list<bool>::const_reverse_iterator d;
    for (c=cnvs.rbegin(), d=dirs.rbegin(); c!=cnvs.rend(); c++, d++)
      if (*d) (*c)->bck(p); else (*c)->frw(p);
    p*=Conv::sc_src;
  }

};

#endif
