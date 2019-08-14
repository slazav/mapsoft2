#ifndef MK_REF_H
#define MK_REF_H

#include "geo_data/geo_data.h"
#include "opt/opt.h"

/********
Create map reference from options.

In many cases it is needed to draw some geodata on a raster image. For
this one have to specify the image reference (GeoMap object):
datum and projection, position, scale, border etc. This function can
build certain types of references using test options.

Options:

--ref nom -- make Soviet nomenclature map.
  --name <name> -- map name (mapsoft2 extensions are allowed).
  --dpi <double> -- map resolution in dots per inch (default 300).
  --mag <factor> -- rescale map without changing dpi setting (default 1).

--ref tms_tile
  --tile <tile-point>
  --tile_range <tile-rect>
  --wgs_range <wgs84 rect>
  --wgs_border <wgs84 line>
  --wgs_border <wgs84 multiline>

--ref google_tile
  --tile <tile-point>
  --tile_range <tile-rect>
  --wgs_range <wgs84 rect>
  --wgs_border <wgs84 line>
  --wgs_border <wgs84 multiline>

--ref proj
  --proj <proj string>
  --range <range in proj coords>
  --border <line in proj coords>
  --border <multiline in proj coords>
  --wgs_range  <wgs84 range>
  --wgs_border <wgs84 multiline>
  --wgs_border <wgs84 line>
  --dpi
  --rscale

*/
GeoMap geo_mkref(Opt & o);

#endif
