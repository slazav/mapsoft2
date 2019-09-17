#ifndef VIEWER_PANEL_H
#define VIEWER_PANEL_H

#include <gtkmm.h>
#include "viewer/gobj_multi.h"

/* PanelRecord -- TreeModelColumnRecord for WPT/TRK/MAP panels. */

template <typename Tl, typename Td>
class PanelRecord : public Gtk::TreeModelColumnRecord {
public:
    Gtk::TreeModelColumn<bool> checked;
    Gtk::TreeModelColumn<std::string> name;
    Gtk::TreeModelColumn<Pango::Weight> weight;
    Gtk::TreeModelColumn<std::shared_ptr<Tl> > gobj;
    Gtk::TreeModelColumn<std::shared_ptr<Td> > data;

    PanelRecord() {
      add(checked); add(name); add(weight); add(gobj); add(data);
    }
};

/* Panel -- Abstract class for WPT/TRK/MAP panels,
 * child of Gtk::TreeView and GObjMulti.
 */

template <typename Tl, typename Td>
class Panel : public Gtk::TreeView, public GObjMulti {
  sigc::signal<void> signal_data_changed_;
public:

  // This signal is emitted when data is changed
  // (everything we want to save in the file:
  //  name or order, but maybe not visibility).
  sigc::signal<void> & signal_data_changed(){
    return signal_data_changed_;}


  // Constructor.
  Panel() {
    store = Gtk::ListStore::create(columns);
    set_model(store);
    append_column_editable("V", columns.checked);
    int name_cell_n = append_column_editable("V", columns.name);

    Gtk::TreeViewColumn* name_column = get_column(name_cell_n - 1);
    Gtk::CellRendererText* name_cell =
      (Gtk::CellRendererText*)get_column_cell_renderer(name_cell_n - 1);
    if (name_column && name_cell)
      name_column->add_attribute(
        name_cell->property_weight(), columns.weight);

    store->signal_row_changed().connect (
      sigc::mem_fun (this, &Panel::on_panel_edited));

    set_enable_search(false);
    set_headers_visible(false);
    set_reorderable(false);
  }

  // Add data
  virtual void add(const std::shared_ptr<Td> & data) = 0;

  // Remove all data
  void remove_all() {
    store->clear();
    GObjMulti::clear();
  }

  // Remove selected object
  void remove_selected(){
    auto it = get_selection()->get_selected();
    if (!it) return;
    store->erase(it);
    GObjMulti::del(it->get_value(columns.gobj));
  }

  // Remove object
  void remove(const std::shared_ptr<Tl> & obj){
    for (auto i = store->children().begin();
         i != store->children().end(); i++){
      if (i->get_value(columns.gobj) != obj) continue;
      store->erase(i);
      break;
    }
    GObjMulti::del(obj);
  }

  // Hide/Show all
  void show_all(bool show=true){
    for (auto row:store->children())
      row[columns.checked] = show;
  }

  // Invert visibility
  void invert_all(){
    for (auto row:store->children())
      row[columns.checked] = !row[columns.checked];
  }

  // Join visible/all objects
  void join(bool visible) {
    std::shared_ptr<Td> newd(new Td);

    auto i = store->children().begin();
    while (i != store->children().end()){
      if (visible && !(*i)[columns.checked]){
        ++i; continue;
      }

      auto curr = i->get_value(columns.gobj);
      if (!curr){ ++i; continue; }

      newd->insert(newd->end(), curr->begin(), curr->end());
      if (newd->size()) newd->name = "JOIN";
      else newd->name = curr->name;

      i = store->erase(i);
      GObjMulti::del(curr);
    }
    if (newd->size())
    GObjMulti::add(newd);
  }

  // Find selected object
  std::shared_ptr<Tl> find_selected() {
    auto const it = get_selection()->get_selected();
    if (!it) return NULL;
    return it->get_value(columns.gobj);
  }

  // Find first visible object
  std::shared_ptr<Tl> find_first() const {
    for (auto row:store->children()) {
      if (row[columns.checked])
        return row[columns.gobj];
    }
    return NULL;
  }

  // Get all/visible data
  void get_data(GeoData & data, bool visible) const {
    for (auto row:store->children()) {
       if (visible && !row[columns.checked]) continue;
       std::shared_ptr<Td> d = row[columns.data];
       data.push_back(*d.get());
    }
  }

  // Get selected data
  void get_sel_data(GeoData & data) {
    auto i = get_selection()->get_selected();
    if (!i) return;
    auto data1 = (*i)[columns.gobj]->get_data();
    data.push_back(*data1);
  }

  /* move object (up/down/top/bottom) */
  void move(bool up, bool onestep){
    auto it1 = get_selection()->get_selected();
    auto it2 = it1;
    if (!it1) return;
    if (up && (it1 == store->children().begin())) return;

    if (!onestep){
      if (up) it2--; else it2++;
      if (!it2) return;
    }
    else{
      if (up) it2 = store->children().begin();
      else {it2 = store->children().end(); it2--;}
    }
    store->iter_swap(it1, it2);
    upd_wp();
  }

  void on_panel_edited (const Gtk::TreeModel::Path& path,
                        const Gtk::TreeModel::iterator& iter) {
    // update gobj depth and visibility in GObjMulti
    upd_wp();
    // update names in data (no need to redraw)
    upd_name();
  }

  virtual bool upd_name(Tl * sel_gobj = NULL, bool dir=true) = 0;

  /* Update visibility and depths of objects
     according to TreeView */
  bool upd_wp(){
    bool ret=false;
    Gtk::TreeNodeChildren::const_iterator i;
    int d=1;

    for (auto const & row:store->children()){
      auto gobj = row.get_value(columns.gobj);
      if (!gobj) continue;
      // update depth
      if (get_depth(gobj) != d){
        set_depth(gobj, d);
        ret = true;
      }
      d++;
    }
    if (ret) signal_data_changed().emit();

    for (auto const & row:store->children()){
      auto gobj = row.get_value(columns.gobj);
      if (!gobj) continue;
      // update visibility
      bool act = row[columns.checked];
      if (get_visibility(gobj) != act){
        set_visibility(gobj, act);
        ret = true;
      }
    }
    if (ret) signal_redraw_me().emit(iRect());
    return ret;
  }

protected:
  Glib::RefPtr<Gtk::ListStore> store;
  PanelRecord<Tl, Td> columns;
};

#endif
