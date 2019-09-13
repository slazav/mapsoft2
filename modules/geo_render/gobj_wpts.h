#ifndef GOBJ_WPTS_H
#define GOBJ_WPTS_H

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
         ConvBase & cnv, GeoWptList & wpts,
         const Opt & opt);

#include "viewer/gobj.h"

/************************************************/
class GObjWpts : public GObj {
private:

  // Waypoint template for fast drawing: coorinates are converted,
  // text box is placed correctly, all parameters are set...
  struct WptDrawTmpl : dPoint{
    double text_size, size, linewidth;
    dPoint text_pt;
    dRect  text_box; // relative to text_pt!
    dRect  bbox;
    int color, bgcolor;
    std::string text_font;
    std::string name;
    GeoWpt * src;
  };

  WptDrawTmpl wt0;    // default waypoint template
  int wpt_bar_length; // default bar length
  int wpt_text_pad;
  bool do_adj_pos;    // adjust text positions to avoid collisions
  bool do_adj_brd;    // adjust text positions fit into picture

  // Original data. It may be edited through the GObj interface.
  GeoWptList & wpts;

  // Templates. Should be syncronized with the data.
  std::vector<WptDrawTmpl> tmpls;

public:
  // constructor
  GObjWpts(ConvBase & cnv, GeoWptList & wpts, const Opt & opt);

  /************************************************/
  // drawing waypoints on the image
  int draw(const CairoWrapper & cr, const iPoint &origin) override;

  /************************************************/
  // These functions modify drawing templates, but
  // do not have any locking. They should be called
  // only from locked functions (on_set_opt, on_set_cnv, on_rescale)

  // Update template coordinates for a waypoint template (including bbox!).
  void update_pt_crd(WptDrawTmpl & t);

  // Update bbox for a waypoint template (after changing coordinates)
  void update_pt_bbox(WptDrawTmpl & t);

  // Update name and flag dimensions for a waypoint template.
  void update_pt_name(const CairoWrapper & cr, WptDrawTmpl & t);

  // Update range
  // Should be done after update_pt_crd() and update_pt_name()
  // and before adjust_text_pos() or any drawing.
  void update_range();

  // Adjust text positions to prevent collisions between points
  void adjust_text_pos();

  // Adjust text positions to fit into rng
  void adjust_text_brd(const dRect & rng);

  /************************************************/
  // These functions update drawing templates.
  // They have proper multi-thread locking.

  // update parameters form options.
  void on_set_opt(const Opt & opt);

  // update point coordinates
  void on_set_cnv() override;

  // rescale point coordinates, update range
  void on_rescale(double k) override;

};

#endif
