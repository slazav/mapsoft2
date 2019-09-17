#ifndef MAPVIEW_PANEL_STATUS_H
#define MAPVIEW_PANEL_STATUS_H

// Panel with busy lamp and statusbar

#include <gtkmm.h>
#include <string>

class PanelStatus : public Gtk::HBox{
private:
  Gtk::Statusbar *statusbar;
  Gtk::Image *busy_icon;
public:
  // constructur -- no argumants
  PanelStatus(){
    busy_icon = manage(new Gtk::Image());
    statusbar = manage(new Gtk::Statusbar());
    busy_icon->set_tooltip_text("Viewer acivity");
    busy_icon->set_size_request(20,16);
    pack_start(*busy_icon, false, true, 0);
    pack_start(*statusbar, true, true, 0);
    // see https://discourse.gnome.org/t/migrating-to-gtkmm3-css-statusbar/1806
    statusbar->set_property("margin", 0);
  }
  // put message on the statusbar
  void message(const std::string & msg){
    statusbar->push(msg,0);
  }
  // set busy icon
  void set_busy(){
    auto win = get_window();
    if (win) busy_icon->set(Gtk::Stock::MEDIA_RECORD,Gtk::ICON_SIZE_MENU);
    busy_icon->set_size_request(20,16);
  }
  // unset busy icon
  void set_idle(){
    auto win = get_window();
    if (win) busy_icon->clear();
  }
};

#endif
