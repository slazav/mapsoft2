#ifndef GOBJ_MAPS_H
#define GOBJ_MAPS_H

#include "cairo/cairo_wrapper.h"
#include "conv/conv_multi.h"
#include "conv/conv_base.h"
#include "geo_data/geo_data.h"
#include "cache/cache.h"
#include "image/image_cache.h"
#include "opt/opt.h"
#include "viewer/gobj.h"

/********************************************************************/
#include "getopt/getopt.h"

// add MS2OPT_DRAWMAP options
void ms2opt_add_drawmap(GetOptSet & opts);

/********************************************************************/

class GObjMaps : public GObj {
private:

  // Original data. It may be edited through the GObj interface.
  GeoMapList & maps;

  struct MapData{
    ConvMulti cnv;  // conversion from viewer coordinates to the map coordinates
    bool   simp;    // true if conversion was simplified (then manual rescaling is needed)
    double scale;   // map scale (map pixels / viewer pixels)
    double load_sc; // scale for image loading
    dMultiLine brd; // map border (in viewer coordinates)
    dLine refs;     // map refpoints (in viewer coordinates)
    dRect bbox;     // map bbox (in viewer coordinates)
    dRect src_bbox; // map original bbox (it may take some time to get it)
    const GeoMap * src;   // pointer to the map
  };
  ImageCache img_cache;
  Cache<iRect, Image> tiles;
  std::vector<MapData> data;

  bool smooth;   // smooth map drawing
  bool clip_brd; // clip map to its border
  int  draw_brd; // draw map border (color)
  int  draw_refs;// draw map reference points (color)
  int  fade;     // map fade level 0..100

public:
  // constructor
  GObjMaps(GeoMapList & maps);

  /************************************************/
  // drawing waypoints on the image
  int draw(const CairoWrapper & cr, const dRect &box) override;

  /************************************************/
  // These functions update drawing templates.
  // They have proper multi-thread locking.

  // update information from options
  void on_set_opt() override;

  // update point coordinates
  void on_set_cnv() override;

  // rescale point coordinates, update range
  void on_rescale(double k) override;

};

#endif
