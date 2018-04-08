#ifndef CONV_AFF_H
#define CONV_AFF_H

#include "conv.h"
#include <map>

///\addtogroup lib2d
///@{
///\defgroup conv_aff
///@{

/// Affine transformation
class ConvAff : public Conv {

  std::vector<double> k_frw;
  std::vector<double> k_bck;
  void bck_recalc(); ///< recalculate k_bck matrix

public:
  /// constructor - trivial transformation
  ConvAff();

  /// constructor - from g_ref
  ConvAff(const std::map<dPoint, dPoint> & ref);

  /// reset from g_ref
  void set_from_ref(const std::map<dPoint, dPoint> & ref);

  void frw(dPoint & p) const{
    double x = k_frw[0]*p.x + k_frw[1]*p.y + k_frw[2];
    double y = k_frw[3]*p.x + k_frw[4]*p.y + k_frw[5];
    p.x=x; p.y=y;
  }

  void bck(dPoint & p) const{
    double x = k_bck[0]*p.x + k_bck[1]*p.y + k_bck[2];
    double y = k_bck[3]*p.x + k_bck[4]*p.y + k_bck[5];
    p.x=x; p.y=y;
  }

  /// reset to trivial
  void reset();

  double det() const { return k_frw[0] * k_frw[4] - k_frw[1] * k_frw[3];}
  void shift(const dPoint & p);
  void scale(const double kx, const double ky);

  void rescale_src(const double s);
  void rescale_dst(const double s);

};

#endif
