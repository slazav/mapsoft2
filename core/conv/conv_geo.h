#ifndef CONV_GEO_H
#define CONV_GEO_H

#include <string>
#include "conv_base.h"
#include <proj_api.h>


///\addtogroup libmapsoft
///@{

//typedef void* projPJ;

/// Geo transformation, libproj wrapper
class ConvGeo: public ConvBase {
public:

  /// Create a transformation. `src` and `dst` are libproj parameters.
  ConvGeo(const std::string & src, const std::string & dst =
    "+datum=WGS84 +proj=lonlat");

  /// Copy constructor
  ConvGeo(const ConvGeo & other){ copy(other); }

  /// assignment
  ConvGeo & operator=(const ConvGeo & other);

  /// destructor
  ~ConvGeo(){ destroy(); }

  /// point conversion
  void frw_pt(dPoint & p) const{
    if (sc_src!=1.0) p*=sc_src;  // this if increases speed...
    if (pj_src!=pj_dst) pj_transform(pj_src, pj_dst, 1, 1, &p.x, &p.y, NULL);
    if (sc_dst!=1.0) p*=sc_dst;
  }

  /// point conversion
  void bck_pt(dPoint & p) const{
    if (sc_dst!=1.0) p/=sc_dst;
    if (pj_src!=pj_dst) pj_transform(pj_dst, pj_src, 1, 1, &p.x, &p.y, NULL);
    if (sc_src!=1.0) p/=sc_src;
  }

private:
  projPJ pj_src, pj_dst;
  void copy(const ConvGeo & other);
  void destroy(void);
  int * refcounter;
};

///@}
#endif
