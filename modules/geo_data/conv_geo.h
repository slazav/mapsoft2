#ifndef CONV_GEO_H
#define CONV_GEO_H

#include <memory>
#include <string>
#include "conv/conv_aff.h"
#include "conv/conv_multi.h"
#include "geo_data.h"

///\addtogroup libmapsoft
///@{

/// Geo transformation, libproj wrapper
class ConvGeo: public ConvBase {
public:

  /// Create a transformation. `src` and `dst` are libproj parameters.
  ConvGeo(const std::string & src, const std::string & dst =
    "+datum=WGS84 +proj=lonlat", const bool use2d = true);

  /// Forward point conversion.
  void frw_pt(dPoint & p) const;

  /// Backward point conversion.
  void bck_pt(dPoint & p) const;

  // are source/destination coordinates in degrees (not meters)
  bool is_src_deg() const;
  bool is_dst_deg() const;

  // get/set 2d flag
  bool get_2d() const {return cnv2d;}
  void set_2d(const bool v = true) {cnv2d = v;}

private:
  std::shared_ptr<void> pj_src, pj_dst;
  bool cnv2d; // Do 2D or 3D conversion
};


/// Geo transformation from GeoMap
class ConvMap: public ConvMulti {
public:

  /// Create a transformation. `src` is a GeoMap and `dst` is
  /// libproj parameter string.
  ConvMap(const GeoMap & m, const std::string & dst =
    "+datum=WGS84 +proj=lonlat");

private:
  ConvAff2D img2map;
  ConvGeo map2dst;
};


///@}
#endif
