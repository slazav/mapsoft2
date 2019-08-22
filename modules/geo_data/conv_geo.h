#ifndef CONV_GEO_H
#define CONV_GEO_H

#include <memory>
#include <string>
#include "conv/conv_aff.h"
#include "conv/conv_multi.h"
#include "geo_data.h"

///\addtogroup libmapsoft
///@{

// default datum and projection.
#define GEO_PROJ_DEF  "+datum=WGS84 +proj=lonlat"

// Web tiles projection.
// before libproj-5.1.0 it was "+proj=merc +a=6378137 +b=6378137 +nadgrids=@null +no_defs";
#define GEO_PROJ_WEB  "+proj=webmerc +datum=WGS84"

// Finland
#define GEO_PROJ_FI  "+proj=tmerc +lon0=27 +ellps=intl"\
                     " +towgs84=-90.7,-106.1,-119.2,4.09,0.218,-1.05,1.37"

// Switzerland
#define GEO_PROJ_CH  "+proj=somerc +lat_0=46.95240555555556"\
                     " +lon_0=7.439583333333333 +x_0=600000 +y_0=200000"\
                     " +ellps=bessel +towgs84=674.374,15.056,405.346,0,0,0,0"\
                     " +units=m +no_defs"

// USSR
#define GEO_PROJ_SU(lon0, zone)  "+ellps=krass +towgs84=+28,-130,-95 +proj=tmerc"\
                                 " +lon_0=" #lon0 " +x_0=" #zone "500000"


/// Geo transformation, libproj wrapper
class ConvGeo: public ConvBase {
public:

  /// Create a transformation. `src` and `dst` are libproj parameters.
  ConvGeo(const std::string & src, const std::string & dst =
    GEO_PROJ_DEF, const bool use2d = true);

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
  ConvMap(const GeoMap & m, const std::string & dst = GEO_PROJ_DEF);

private:
  ConvAff2D img2map;
  ConvGeo map2dst;
};


///@}
#endif
