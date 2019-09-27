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
// Load new file

class LoadFile : public ActionMode, public Gtk::FileChooserDialog{
  std::string folder; // current folder
  public:
    LoadFile (Mapview * mapview) :
           ActionMode(mapview),
           Gtk::FileChooserDialog(get_name()),
           folder("./"){

      auto fg = Gtk::FileFilter::create();
      fg->add_pattern("*.gu");
      fg->add_pattern("*.gpx");
      fg->add_pattern("*.kml");
      fg->add_pattern("*.kmz");
      fg->add_pattern("*.plt");
      fg->add_pattern("*.wpt");
      fg->add_pattern("*.map");
      fg->add_pattern("*.js");
      fg->add_pattern("*.zip");
      fg->set_name("Geodata");
      add_filter(fg);

      auto fa = Gtk::FileFilter::create();
      fa->add_pattern("*");
      fa->set_name("All files");
      add_filter(fa);

      set_select_multiple();
      add_button("_Cancel", GTK_RESPONSE_CANCEL);
      add_button("_Open",   GTK_RESPONSE_ACCEPT);

      set_transient_for(*mapview);
      set_position(Gtk::WIN_POS_CENTER_ON_PARENT);
    }

    std::string get_name() { return "Load File"; }
    Gtk::StockID get_stockid() { return Gtk::Stock::ADD; }
    Gtk::AccelKey get_acckey() { return Gtk::AccelKey("<control>l"); }
    bool is_radio() { return false; }

    void activate() {
      set_current_folder(folder);
      if (run() == GTK_RESPONSE_ACCEPT){
         mapview->add_files(get_filenames());
         folder = get_current_folder();
      }
      hide();
    }
};

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
