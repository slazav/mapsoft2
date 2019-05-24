#ifndef CONV_GEO_H
#define CONV_GEO_H

#include <memory>
#include <string>
#include "conv_base.h"

///\addtogroup libmapsoft
///@{

/// Geo transformation, libproj wrapper
class ConvGeo: public ConvBase {
public:

  /// Create a transformation. `src` and `dst` are libproj parameters.
  ConvGeo(const std::string & src, const std::string & dst =
    "+datum=WGS84 +proj=lonlat");

  /// Forward point conversion.
  void frw_pt(dPoint & p) const;

  /// Backward point conversion.
  void bck_pt(dPoint & p) const;

private:
  std::shared_ptr<void> pj_src, pj_dst;
};

///@}
#endif
