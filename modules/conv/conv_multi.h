#ifndef CONV_MULTI_H
#define CONV_MULTI_H

#include "conv_base.h"
#include <list>

///\addtogroup libmapsoft
///@{

/// Composite conversion
/// User must keep all parts until this conversion is used
class ConvMulti : public ConvBase {

  std::list<const ConvBase *> cnvs;
  std::list<bool> dirs;

public:

  /// constructor - trivial transformation
  ConvMulti(){}

  /// constructor - 2 conversions
  ConvMulti(const ConvBase *cnv1, const ConvBase *cnv2, bool frw1=true, bool frw2=true){
    cnvs.push_back(cnv1);
    cnvs.push_back(cnv2);
    dirs.push_back(frw1);
    dirs.push_back(frw2);
  }

  /// add a conversion in front of the list
  void push_front(const ConvBase *cnv, bool frw=true){
    cnvs.push_front(cnv);
    dirs.push_front(frw);
  }

  /// add a conversion at the end of the list
  void push_back(const ConvBase *cnv, bool frw=true){
    cnvs.push_back(cnv);
    dirs.push_back(frw);
  }

  /// redefine a forward point conversion
  void frw_pt(dPoint & p) const{
    p*=ConvBase::sc_src; // use sc_src/sc_dst from the base class
    std::list<const ConvBase *>::const_iterator c;
    std::list<bool>::const_iterator f;
    for (c=cnvs.begin(), f=dirs.begin(); c!=cnvs.end(); c++, f++)
      if (*f) (*c)->frw(p); else (*c)->bck(p);
    p*=ConvBase::sc_dst;
  }

  /// redefine a backward point conversion
  void bck_pt(dPoint & p) const{
    p/=ConvBase::sc_dst; // use sc_src/sc_dst from the base class
    std::list<const ConvBase *>::const_reverse_iterator c;
    std::list<bool>::const_reverse_iterator f;
    for (c=cnvs.rbegin(), f=dirs.rbegin(); c!=cnvs.rend(); c++, f++)
      if (*f) (*c)->bck(p); else (*c)->frw(p);
    p/=ConvBase::sc_src;
  }

};

///@}
#endif
