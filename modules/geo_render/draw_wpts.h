#ifndef DRAW_WPTS_H
#define DRAW_WPTS_H

#include "cairo/cairo_wrapper.h"
#include "conv/conv_base.h"
#include "geo_data/geo_data.h"
#include "opt/opt.h"

/********************************************************************/
#include "getopt/getopt.h"

// add MS2OPT_DRAWWPT options
void ms2opt_add_drawwpt(ext_option_list & opts);

/********************************************************************/

void
draw_wpts(CairoWrapper & cr, const iPoint & origin,
         const ConvBase & cnv, const GeoWptList & wpts,
         const Opt & opt);

/***********************************************************/

/* Drawing can be splitted into two parts:
   Creating small drawing template (can be slow)
   and then drawing it.
   Also, while creating template we go through
   all wpts and can calculate the total range */

/* waypoint templates */
struct WptDrawTmpl : dPoint{
  double text_size, text_pad, size, linewidth;
  dPoint text_pt;
  dRect  text_box;
  int color, bgcolor;
  std::string text_font;
  std::string name;
};

struct WptsDrawTmpl{
  private:
    std::vector<WptDrawTmpl> data;
    dRect bbox;
  public:


  /* make template and calculate bbox */
  WptsDrawTmpl(CairoWrapper & cr, const iPoint & origin,
               const ConvBase & cnv, const GeoWptList & wpts,
               const Opt & opt);

  /* adjust text positions to prevent point collisions */
  void adjust();

  /* adjust text positions to avoid text outside rng */
  void adjust_brd(const dRect & rng);

  /* plot template */
  void draw(CairoWrapper & cr) const;
};


#endif
