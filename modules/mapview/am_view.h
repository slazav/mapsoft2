#ifndef AM_VIEW_H
#define AM_VIEW_H

/* Action modes for View menu */
#include "action_mode.h"

/**********************************************************/
// Toggle fullscreen mode

class FullScreen : public ActionMode{
    int state;
  public:
    FullScreen (Mapview * mapview) : ActionMode(mapview), state(0){ }
    std::string get_name() { return "Fullscreen mode"; }
    Gtk::StockID get_stockid() { return Gtk::Stock::FULLSCREEN; }
    Gtk::AccelKey get_acckey() { return Gtk::AccelKey("<control>f"); }
    bool is_radio() { return false; }

    void activate() {
      state = (state+1)%2;
      if (state==0) mapview->unfullscreen();
      else mapview->fullscreen();
    }
};

/**********************************************************/
// Hide panels

class HidePanels : public ActionMode{
    int state;
  public:
    HidePanels (Mapview * mapview) : ActionMode(mapview), state(0){ }
    std::string get_name() { return "Hide/Show Panels"; }
    Gtk::AccelKey get_acckey() { return Gtk::AccelKey("<control>h"); }
    bool is_radio() { return false; }
    void activate() {
      state = (state+1)%2;
      if (state==0){
        mapview->spanel.show();
        mapview->panels->show();
      }
      else {
        mapview->spanel.hide();
        mapview->panels->hide();
      }
    }
};

#endif
