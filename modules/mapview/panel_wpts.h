#ifndef MAPVIEW_PANEL_WPTS_H
#define MAPVIEW_PANEL_WPTS_H

#include "geo_render/gobj_wpts.h"
#include "panel.h"

/* Control panel for waypoint gobjects. */

class PanelWpts : public Panel<GObjWpts, GeoWptList> {
public:
    PanelWpts() {}

    void add(const std::shared_ptr<GeoWptList> & wpts) override;


/*
    // find waypoint in all gobjs
    int find_wpt(const iPoint & p, GObjWpts ** gobj, int radius = 3) const;

    // ???
    dPoint get_sel_point(){
      GObjWpts * O = find_selected();
      if (!O || !O->get_data()->size())
        return dPoint(nan(""), nan(""));
      return (*O->get_data())[0];
    }

    // find waypoints in a rectangular area
    std::map<GObjWpts*, std::vector<int> > find_wpts(const iRect & r) const;
*/
    bool upd_name(GObjWpts * sel_gobj=NULL, bool dir=true);
};

#endif
