#ifndef MAPVIEW_PANEL_TRKS_H
#define MAPVIEW_PANEL_TRKS_H

#include "geo_render/gobj_trk.h"
#include "panel.h"

/* Control panel for track gobjects */

class PanelTrks : public Panel<GObjTrk, GeoTrk> {
public:

    PanelTrks () { set_name("TRKS"); }

    void add(const std::shared_ptr<GeoTrk> & trk) override;

//    dPoint get_sel_point(){
//      GObjTrk * O = find_selected();
//      if (!O || !O->get_data()->size())
//        return dPoint(nan(""), nan(""));
//      return (*O->get_data())[0];
//    }

    /* find track points in a rectangular area */
//    std::map<GObjTrk*, std::vector<int> > find_tpts(const iRect & r) const;

    /* find trackpoint in all gobjs */
    // segment=true: find track point, returns its number 0..size()-1
    // segment=true: find track segment, return its
    //               first point 0..size()-2
//    int find_tpt(const iPoint & p, GObjTrk ** gobj,
//                 const bool segment = false, int radius = 3) const;

    bool upd_name(GObjTrk * sel_gobj=NULL, bool dir=true);

};

#endif
