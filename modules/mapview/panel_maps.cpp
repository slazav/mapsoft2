#include "panel_maps.h"

void
PanelMaps::add(const std::shared_ptr<GeoMapList> & maps) {
  // depth is set to 0 to evoke refresh!
  std::shared_ptr<GObjMaps> gobj(new GObjMaps(*maps.get()));
  GObjMulti::add(0, gobj);

  // find name
  std::string name = maps->name;
  Pango::Weight weight=Pango::WEIGHT_BOLD;
  if (maps->size() == 1){
    name=(*maps)[0].name;
    weight=Pango::WEIGHT_NORMAL;
  }

  Gtk::TreeModel::iterator it = store->append();
  Gtk::TreeModel::Row row = *it;
  row[columns.checked] = true;
  row[columns.data]    = maps;
  row[columns.name]    = name;
  row[columns.weight]  = weight;
  row[columns.gobj]    = gobj;
}
/*
int
PanelMaps::find_map(const iPoint & p, GObjMaps ** gobj) const{
  Gtk::TreeNodeChildren::const_iterator i;
  for (i  = store->children().begin();
       i != store->children().end(); i++){
    if (!(*i)[columns.checked]) continue;
    std::shared_ptr<GObjMaps> current_gobj = (*i)[columns.gobj];
    *gobj = current_gobj.get();
    int d = current_gobj->find_map(p);
    if (d >= 0) return d;
  }
  *gobj = NULL;
  return -1;
}
*/
bool
PanelMaps::upd_name(GObjMaps * sel_gobj, bool dir){
  bool ret=false;
  for (auto const row:store->children()){
    std::shared_ptr<GObjMaps> gobj = row[columns.gobj];
    if (!gobj) continue;
    // select gobj if sel_gobj!=NULL
    if (sel_gobj && sel_gobj!=gobj.get()) continue;
    // update names
    std::string name = row[columns.name];
    std::shared_ptr<GeoMapList> maps = row[columns.data];
    if (maps->size()==1){
      if (name!=(*maps)[0].name){
        if (dir) (*maps)[0].name = name;
        else row[columns.name] = (*maps)[0].name;
        ret = true;
      }
    }
    else {
      if (name!=maps->name){
        if (dir) maps->name = name;
        else row[columns.name] = maps->name;
        ret = true;
      }
    }
  }
  if (ret) signal_data_changed().emit();
  return ret;
}

