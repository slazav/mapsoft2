#ifndef CONV_AFF_H
#define CONV_AFF_H

#include "conv_base.h"
#include <map>

///\addtogroup libmapsoft
///@{

/// 2D affine transformation
class ConvAff2D : public ConvBase {

  std::vector<double> k_frw; ///< transformation parameters (6 numbers)
  std::vector<double> k_bck; ///< parameters of inverse transformation
  void bck_recalc(); ///< recalculate k_bck matrix

public:
  /// constructor - trivial transformation
  ConvAff2D() {reset();}

  /// constructor - from a point-to-point reference
  ConvAff2D(const std::map<dPoint, dPoint> & ref) {reset(ref);}

  /// reset to trivial
  void reset();

  /// reset from a point-to-point reference
  void reset(const std::map<dPoint, dPoint> & ref);

  /// point transformation
  void frw_pt(dPoint & p) const{
    double x = k_frw[0]*p.x + k_frw[1]*p.y + k_frw[2];
    double y = k_frw[3]*p.x + k_frw[4]*p.y + k_frw[5];
    p.x=x; p.y=y;
  }

  /// point transformation
  void bck_pt(dPoint & p) const{
    double x = k_bck[0]*p.x + k_bck[1]*p.y + k_bck[2];
    double y = k_bck[3]*p.x + k_bck[4]*p.y + k_bck[5];
    p.x=x; p.y=y;
  }

  /// forward conversion determinant
  double det() const { return k_frw[0] * k_frw[4] - k_frw[1] * k_frw[3];}

  /// shift before the transformation
  void shift_src(const dPoint & p);

  /// shift after the transformation
  void shift_dst(const dPoint & p);

  /// scale x and y before the transformation
  void rescale_src(const double kx, const double ky);

  /// scale x and y before the transformation
  void rescale_dst(const double kx, const double ky);

  /// scale before the transformation
  void rescale_src(const double s){ rescale_src(s,s); }

  /// scale before the transformation
  void rescale_dst(const double s){ rescale_dst(s,s);}

};

///@}
#endif
