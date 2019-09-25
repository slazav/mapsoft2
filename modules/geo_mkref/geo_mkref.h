#ifndef MK_REF_H
#define MK_REF_H

#include "geo_data/geo_data.h"
#include "opt/opt.h"

/********
Create map reference from options.

Following map fields are set:
  map.name -- map name
  map.proj -- map projection (string of libproj parameters)
  map.image_dpi -- map resolution, dots per inch
  map.image_size -- map image size (in pixels)
  map.border -- map border (in pixels)
  map.ref -- four reference points
*/

/********************************************************************/
#include "getopt/getopt.h"

// add MS2OPT_GEO_MKREF options
void ms2opt_add_mkref(ext_option_list & opts);

/********************************************************************/

GeoMap geo_mkref(const Opt & o);

// try to get some information from GeoData if there is
// no mkref option
GeoMap geo_mkref(const GeoData & data, const Opt & o);

#endif
