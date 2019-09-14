#ifndef CONV_MULTI_H
#define CONV_MULTI_H

#include "conv_base.h"
#include <list>
#include <memory>

///\addtogroup libmapsoft
///@{

/// Composite conversion
/// User must keep all parts until this conversion is used
class ConvMulti : public ConvBase {

  std::list<std::shared_ptr<ConvBase> > cnvs;
  std::list<bool> dirs;

public:

  /// constructor - trivial transformation
  ConvMulti(){}

  /// constructor - 2 conversions
  ConvMulti(const std::shared_ptr<ConvBase> & cnv1,
            const std::shared_ptr<ConvBase> & cnv2,
            bool frw1=true, bool frw2=true){
    cnvs.push_back(cnv1);
    cnvs.push_back(cnv2);
    dirs.push_back(frw1);
    dirs.push_back(frw2);
  }

  /// add a conversion in front of the list
  void push_front(const std::shared_ptr<ConvBase> & cnv, bool frw=true){
    cnvs.push_front(cnv);
    dirs.push_front(frw);
  }

  /// add a conversion at the end of the list
  void push_back(const std::shared_ptr<ConvBase> & cnv, bool frw=true){
    cnvs.push_back(cnv);
    dirs.push_back(frw);
  }

  /// rescale source coordinates
  void rescale_src(const double s) override {
    if (cnvs.size()>0){
      if (*dirs.begin()) (*cnvs.begin())->rescale_src(s);
      else (*cnvs.begin())->rescale_dst(s);
    }
  }

  /// rescale destination coorinates
  /// Childs can use this parameter in frw/bck or redefine rescale_dst()
  void rescale_dst(const double s) override {
    if (cnvs.size()>0){
      if (*dirs.rbegin()) (*cnvs.rbegin())->rescale_dst(1.0/s);
      else (*cnvs.rbegin())->rescale_src(1.0/s);
    }
  }

  /// redefine a forward point conversion
  void frw_pt(dPoint & p) const override {
    p*=ConvBase::sc_src; // use sc_src/sc_dst from the base class
    std::list<std::shared_ptr<ConvBase> >::const_iterator c;
    std::list<bool>::const_iterator f;
    for (c=cnvs.begin(), f=dirs.begin(); c!=cnvs.end(); c++, f++)
      if (*f) (*c)->frw(p); else (*c)->bck(p);
    p*=ConvBase::sc_dst;
  }

  /// redefine a backward point conversion
  void bck_pt(dPoint & p) const override {
    p/=ConvBase::sc_dst; // use sc_src/sc_dst from the base class
    std::list<std::shared_ptr<ConvBase> >::const_reverse_iterator c;
    std::list<bool>::const_reverse_iterator f;
    for (c=cnvs.rbegin(), f=dirs.rbegin(); c!=cnvs.rend(); c++, f++)
      if (*f) (*c)->bck(p); else (*c)->frw(p);
    p/=ConvBase::sc_src;
  }

};

///@}
#endif
