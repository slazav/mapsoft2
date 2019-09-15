#ifndef CONV_MULTI_H
#define CONV_MULTI_H

#include "conv_base.h"
#include <list>
#include <memory>

///\addtogroup libmapsoft
///@{

/// Composite conversion
class ConvMulti : public ConvBase {

  std::list<std::shared_ptr<const ConvBase> > cnvs;
  std::list<bool> dirs;

public:

  /// constructor - trivial transformation
  ConvMulti(){}

  /// constructor - 2 conversions
  ConvMulti(const std::shared_ptr<const ConvBase> & cnv1,
            const std::shared_ptr<const ConvBase> & cnv2,
            bool frw1=true, bool frw2=true){
    cnvs.push_back(cnv1);
    cnvs.push_back(cnv2);
    dirs.push_back(frw1);
    dirs.push_back(frw2);
  }

  // reset to trivial conversion
  void reset(){
    cnvs.clear();
    dirs.clear();
  }

  /// add a conversion in front of the list
  void push_front(const std::shared_ptr<const ConvBase> & cnv, bool frw=true){
    cnvs.push_front(cnv);
    dirs.push_front(frw);
  }

  /// add a conversion at the end of the list
  void push_back(const std::shared_ptr<const ConvBase> & cnv, bool frw=true){
    cnvs.push_back(cnv);
    dirs.push_back(frw);
  }

  /// redefine a forward point conversion
  void frw_pt(dPoint & p) const override {
    p.x*=sc_src; p.y*=sc_src;
    std::list<std::shared_ptr<const ConvBase> >::const_iterator c;
    std::list<bool>::const_iterator f;
    for (c=cnvs.begin(), f=dirs.begin(); c!=cnvs.end(); c++, f++)
      if (*f) (*c)->frw(p); else (*c)->bck(p);
    p.x*=sc_dst; p.y*=sc_dst;
  }

  /// redefine a backward point conversion
  void bck_pt(dPoint & p) const override {
    p.x/=sc_dst; p.y/=sc_dst;
    std::list<std::shared_ptr<const ConvBase> >::const_reverse_iterator c;
    std::list<bool>::const_reverse_iterator f;
    for (c=cnvs.rbegin(), f=dirs.rbegin(); c!=cnvs.rend(); c++, f++)
      if (*f) (*c)->bck(p); else (*c)->frw(p);
    p.x/=sc_src; p.y/=sc_src;
  }

  /// Try to substitude all conversions by a single ConvAff.
  //  Algorythm:
  //  - make NxN points in the src_box, convert src->dst
  //  - build ConvAff using these points
  //  - convert points back to src coordinates
  //  - measure error (mean square distance, in src coordinates)
  //  - if error < E substitude all conversions with the new one
  //  - return true if substitution was none, false otherwise.
  bool simplify(const dRect & src_box, int N, double E = 1);

  int size() const {return cnvs.size();}

};

///@}
#endif
