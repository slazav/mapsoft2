#include "panel_trks.h"

void
PanelTrks::add(const std::shared_ptr<GeoTrk> & trk) {
  // depth is set to 0 to evoke refresh!
  std::shared_ptr<GObjTrk> gobj(new GObjTrk(*trk.get()));
  GObjMulti::add(0, gobj);

  Gtk::TreeModel::iterator it = store->append();
  Gtk::TreeModel::Row row = *it;
  // note: signal_row_changed() is emitted three times from here:
  row[columns.checked] = true;
  row[columns.name]    = trk->name;
  row[columns.weight]  = Pango::WEIGHT_NORMAL;
  row[columns.data]    = trk;
  row[columns.gobj]    = gobj;
}

/*
std::map<GObjTrk*, std::vector<int> >
PanelTrks::find_tpts(const iRect & r) const {
  std::map<GObjTrk*, std::vector<int> > ret;
  Gtk::TreeNodeChildren::const_iterator i;
  for (i  = store->children().begin();
       i != store->children().end(); i++){
    if (!(*i)[columns.checked]) continue;
    std::shared_ptr<GObjTrk> gobj = (*i)[columns.gobj];
    std::vector<int> pts = gobj->find_trackpoints(r);
    if (pts.size()>0)
      ret.insert(pair<GObjTrk*, std::vector<int> >(gobj.get(), pts));
  }
  return ret;
}

int
PanelTrks::find_tpt(const iPoint & p, GObjTrk ** gobj,
             const bool segment, int radius) const{
  Gtk::TreeNodeChildren::const_iterator i;
  for (i  = store->children().begin();
       i != store->children().end(); i++){
    if (!(*i)[columns.checked]) continue;
    std::shared_ptr<GObjTrk> current_gobj = (*i)[columns.gobj];
    *gobj = current_gobj.get();
    int d;
    if (segment) d = current_gobj->find_track(p, radius);
    else d = current_gobj->find_trackpoint(p, radius);
    if (d >= 0) return d;
  }
  *gobj = NULL;
  return -1;
}
*/

bool
PanelTrks::upd_name(GObjTrk * sel_gobj, bool dir){
  bool ret=false;
  for (auto const & row:store->children()){
    std::string name = row[columns.name];
    std::shared_ptr<GObjTrk> gobj = row[columns.gobj];
    if (!gobj) continue;
    // select gobj if sel_gobj!=NULL
    if (sel_gobj && sel_gobj!=gobj.get()) continue;
    std::shared_ptr<GeoTrk> trk = row[columns.data];

    if (name!=trk->name){
      if (dir) trk->name = name;
      else row[columns.name] = trk->name;
      ret = true;
    }
  }
  if (ret) signal_data_changed().emit();
  return ret;
}
