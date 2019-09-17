#include "panel_wpts.h"

void
PanelWpts::add(const std::shared_ptr<GeoWptList> & wpts) {
  // depth is set to 0 to evoke refresh!
  std::shared_ptr<GObjWpts> gobj(new GObjWpts(*wpts.get()));
  GObjMulti::add(0, gobj);

  Gtk::TreeModel::iterator it = store->append();
  Gtk::TreeModel::Row row = *it;
  // note: signal_row_changed() is emitted three times from here:
  row[columns.checked] = true;
  row[columns.name]    = wpts->name;
  row[columns.weight]  = Pango::WEIGHT_NORMAL;
  row[columns.data]    = wpts;
  row[columns.gobj]    = gobj;
}

/*
int
PanelWpts::find_wpt(const iPoint & p, GObjWpts ** gobj, int radius) const {
  for (auto const & row:store->children()){
    if (!row[columns.checked]) continue;
    std::shared_ptr<GObjWpts> current_gobj = row[columns.gobj];
    *gobj = current_gobj.get();
    int d = current_gobj->find_waypoint(p, radius);
    if (d >= 0) return d;
  }
  *gobj = NULL;
  return -1;
}

std::map<GObjWpts*, std::vector<int> >
PanelWpts::find_wpts(const iRect & r) const {
  std::map<GObjWpts*, std::vector<int> > ret;
  for (auto const & row:store->children()){
    if (!row[columns.checked]) continue;
    std::shared_ptr<GObjWpts> gobj = row[columns.gobj];
    std::vector<int> pts = gobj->find_waypoints(r);
    if (pts.size()>0)
      ret.insert(pair<GObjWpts*, std::vector<int> >(gobj.get(), pts));
  }
  return ret;
}
*/

bool
PanelWpts::upd_name(GObjWpts * sel_gobj, bool dir){
  bool ret=false;
  for (auto const & row:store->children()){
    std::string name = row[columns.name];
    std::shared_ptr<GObjWpts> gobj = row[columns.gobj];
    if (!gobj) continue;
    // select gobj if sel_gobj!=NULL
    if (sel_gobj && sel_gobj!=gobj.get()) continue;

    std::shared_ptr<GeoWptList> wpts = row[columns.data];
    if (name!=wpts->name){
      if (dir) wpts->name = name;
      else row[columns.name] = wpts->name;
      ret = true;
    }
  }
  if (ret) signal_data_changed().emit();
  return ret;
}
