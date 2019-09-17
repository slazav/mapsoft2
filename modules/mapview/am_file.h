#ifndef AM_FILE_H
#define AM_FILE_H

/* Action modes for File menu */
#include "action_mode.h"

/**********************************************************/
// Start new project

class New : public ActionMode{
  public:
    New (Mapview * mapview) : ActionMode(mapview){ }
    std::string get_name() { return "New"; }
    Gtk::StockID get_stockid() { return Gtk::Stock::NEW; }
    Gtk::AccelKey get_acckey() { return Gtk::AccelKey("<control>n"); }
    bool is_radio() { return false; }
    void activate() { mapview->new_project(); }
};

/**********************************************************/
// Add new file
/*
class AddFile : public ActionMode, public Gtk::FileSelection{
  public:
    AddFile (Mapview * mapview) :
           ActionMode(mapview), Gtk::FileSelection(get_name()){

      Glib::RefPtr<Gtk::FileFilter> filter(new Gtk::FileFilter);
      filter->add_pattern("*.gu");
      filter->add_pattern("*.gpx");
      filter->add_pattern("*.plt");
      filter->add_pattern("*.wpt");
      filter->add_pattern("*.map");
      filter->add_pattern("*.js");
//      set_filter(filter);

      set_select_multiple();
      get_ok_button()->signal_clicked().connect(
          sigc::mem_fun (this, &AddFile::on_ok));
      get_cancel_button()->signal_clicked().connect(
          sigc::mem_fun(this, &Gtk::Window::hide));
    }

    std::string get_name() { return "Load File"; }
    Gtk::StockID get_stockid() { return Gtk::Stock::ADD; }
    Gtk::AccelKey get_acckey() { return Gtk::AccelKey("<control>a"); }
    bool is_radio() { return false; }

    void activate() { show(); }

    void on_ok(){
      mapview->add_files(get_selections());
      hide();
    }
};
*/

/**********************************************************/
// Quite the program

class Quit : public ActionMode{
public:
    Quit (Mapview * mapview) : ActionMode(mapview){ }
    std::string get_name() { return "Quit"; }
    Gtk::StockID get_stockid() { return Gtk::Stock::QUIT; }
    Gtk::AccelKey get_acckey() { return Gtk::AccelKey("<control>q"); }
    bool is_radio() { return false; }
    void activate() { mapview->exit(); }
};

#endif
