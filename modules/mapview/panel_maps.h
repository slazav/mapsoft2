#ifndef MAPVIEW_PANEL_MAPS_H
#define MAPVIEW_PANEL_MAPS_H

#include "geo_render/gobj_maps.h"
#include "panel.h"

/* Control panel for map gobjects */

class PanelMaps : public Panel<GObjMaps, GeoMapList> {
public:
  PanelMaps(){ set_name("MAPS"); }

  void add(const std::shared_ptr<GeoMapList> & maps) override;

//  dPoint get_sel_point(){
//    GObjMaps * O = find_selected();
//    if (!O || !O->get_data()->size())
//      return dPoint(nan(""), nan(""));
//    return (*O->get_data())[0].center();
//  }

  /* find top map in the point p */
/*
  int find_map(const iPoint & p, GObjMaps ** gobj) const;
*/
  bool upd_name(GObjMaps * sel_gobj=NULL, bool dir=true);
};

#endif
