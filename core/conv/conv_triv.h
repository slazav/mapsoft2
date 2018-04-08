#ifndef CONV_TRIV_H
#define CONV_TRIV_H

#include "conv_base.h"
#include <map>

///\addtogroup libmapsoft
///@{

/// Trivial transformation with scaling
class ConvTriv : public ConvBase {
  double scale;
public:
  /// constructor - trivial transformation
  ConvTriv(double sc=1.0):scale(sc){}

  // redefine forward point conversion
  void frw_pt(dPoint & p) const {p*=scale;}

  // redefine backward point conversion
  void bck_pt(dPoint & p) const {p/=scale;}

  /// rescale_src is redifined for use with only one scale
  void rescale_src(const double sc) { scale*=sc; }

  /// rescale_src is redifined for use with only one scale,
  /// now it is same as rescale_src
  void rescale_dst(const double sc) { scale*=sc; }
};

///@}
#endif
