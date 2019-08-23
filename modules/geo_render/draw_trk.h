#ifndef DRAW_TRK_H
#define DRAW_TRK_H

#include "cairo/cairo_wrapper.h"
#include "conv/conv_base.h"
#include "geo_data/geo_data.h"
#include "opt/opt.h"

/*

Supported track options:
  thickness, closed, color


Options:

--trk_draw_mode normal
  --trk_draw_color  color
  --trk_draw_dots   0
  --trk_draw_arrows 0

--trk_draw_mode speed
  --trk_draw_min 0
  --trk_draw_max 10
  --trk_draw_grad BCGYRM

--trk_draw_mode height
  --trk_draw_min -200
  --trk_draw_max 8000
  --trk_draw_grad BCGYRM

*/

void
draw_trk(CairoWrapper & cr, const iPoint & origin,
         const ConvBase & cnv, const GeoTrk & trk,
         const Opt & opt);

#endif
