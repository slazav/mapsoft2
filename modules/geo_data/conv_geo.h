#ifndef CONV_GEO_H
#define CONV_GEO_H

#include <memory>
#include <string>
#include "conv/conv_aff.h"
#include "conv/conv_multi.h"
#include "geo_data.h"

///\addtogroup libmapsoft
///@{


/**
Geo transformation, libproj wrapper.

Conversions are constructed using <src> and <dst> strings
which are libproj option string (such as "+datum=WGS84 +proj=lonlat").

A few aliases are supported:
 WGS -- default projection: lon-lat in WGS84 datum
 WEB -- web mercator
 FI  -- transverse mercator projection for Finnish maps
 CH  -- transverse mercator projection for Swiss maps
 SU(N) -- transverse mercator projection for Soviet maps
          (N is central meridian: 3, 9, 15, etc.)

Additional parameter 2d switches altitude conversions
(by default altitude is not converted).

*/

// expand proj aliases (such as "WGS", "WEB", "FI", "SU(39)")
std::string expand_proj_aliases(const std::string & pars);


class ConvGeo: public ConvBase {
public:

  /// Create a transformation. `src` and `dst` are libproj parameters.
  ConvGeo(const std::string & src, const std::string & dst = "WGS", const bool use2d = true);

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
  ConvMap(const GeoMap & m, const std::string & dst = "WGS");

};


///@}
#endif
