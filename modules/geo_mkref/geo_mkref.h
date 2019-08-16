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
  --margins <pixels>
  --top_margin <pixels>
  --left_margin <pixels>
  --right_margin <pixels>
  --bottom_margin <pixels>
    Set margins between map border and image boundary.

--ref tms_tile
--ref google_tile -- create a single tile or rectangular area with multiple tiles

  --zindex <integer>  -- tile zindex

  --tiles <point or rectangle>
      Tile or tile range. If single point is given then non-zero z-coordinate
       overrides zindex option. --tiles "[1,2,3]" is same as --tiles "[1,2]" --zindex 0.

  --coords <wgs84 point, rectangle, line, or multiline>
      Build a map which covers given figure. If argument is Line or MultiLine then
      border is set to this line (can be ovrriden by --border option).

  --border <wgs84 line or multiline>
      Set map border.

--ref proj
  --proj <proj string> -- set projection (libproj parameter string)
  --coords <rect, line or multiline>
  --coords_wgs <rect, line or multiline>
  --border <multiline in proj coords>
  --border_wgs <multiline in proj coords>
  --dpi
  --scale
    Scale (projection units per map cm), default 1000 for non-degree projections,
    1/100 for degree projections.

*/
GeoMap geo_mkref(const Opt & o);

#endif
