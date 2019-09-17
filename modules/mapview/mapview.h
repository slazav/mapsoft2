#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <gtkmm.h>
#include <map>
#include <string>
#include <memory>

#include "viewer/dthread_viewer.h"
#include "viewer/rubber.h"
#include "viewer/gobj_multi.h"
#include "geo_data/geo_io.h"
#include "geom/rect.h"
//#include "actions/action_manager.h"
#include "dlg_chconf.h"
#include "dlg_err.h"

#include "panel_wpts.h"
#include "panel_trks.h"
#include "panel_maps.h"
//#include "panel_vmap.h"
//#include "panel_misc.h"
#include "panel_status.h"

#define DATADIR    "mapsoft2"
#define ACCEL_FILE "mapsoft2.acc"
#define CSS_FILE   "mapsoft2.css"

class Mapview : public Gtk::Window {
public:
    /// Mapview components (order is important for constructing):
    GObjMulti     gobj;            // Main object
    DThreadViewer viewer;          // Viewer, gtk widget which shows main_gobj
    Rubber        rubber;          // Rubber lines
    std::shared_ptr<Opt> opts;     // Command-line options

    // Right panel, a Gtk::Notebook with separate
    // panels for waypoints, tracks, maps ...
    Gtk::Notebook * panels;
    std::shared_ptr<PanelTrks> panel_trks;
    std::shared_ptr<PanelWpts> panel_wpts;
    std::shared_ptr<PanelMaps> panel_maps;
//    PanelVMAP panel_vmap;
//    PanelMISC panel_misc;
    PanelStatus spanel; // status bar
//    ActionManager action_manager; // menus and action handling


    DlgChConf dlg_chconf; // exit confirmation
    DlgErr dlg_err;     // error dialog

private:
    // Project filename.
    std::string project_fname;

    // Changed flag:
    // has the project been changed since last saving/loading?
    bool changed;

    // Have reference flag
    bool haveref;

public:

    // Constructor.
    Mapview(const std::shared_ptr<Opt> & opts);

    /**********************************/

    // Get "has reference" flag.
    bool get_haveref() const {return haveref;}

    // Get project changed status.
    bool get_changed() const {return changed;}

    // Set project changed status, modify window title.
    void set_changed(const bool c=true);

    // Get project filename.
    std::string get_project_fname() const {return project_fname;}

    // Set project filename, do set_changed().
    void set_project_fname(const std::string & f);

    /**********************************/

    // Add data from GeoData object. Scroll to data
    // position if `scroll==true`.
    void add_data(const GeoData & data, bool scroll=false);

    // Remove all data from the current project.
    void clear_data();

    // Fill GeoData object with all/visible data
    // from the current project.
    void get_data(GeoData & data, bool visible=true);

    /**********************************/

    // Add data from files.
    void add_files(const std::vector<std::string> & files);

    // Load new project (confirmation dialog will appear
    // if previous project has been changed and force==false).
    void load_project(const std::string & file, bool force=false);

    // Start new project (confirmation dialog will appear
    // if previous project has been changed and force==false).
    void new_project(bool force=false);

    // Exit Mapview application (confirmation dialog will appear
    // if previous project has been changed and force==false).
    void exit(bool force=false);

//    void add_vmap(const vmap::world & W, bool scroll=false);

    /**********************************/

    // Set new coordinate transformation (viewer -> WGS84).
    void set_cnv(const std::shared_ptr<ConvBase> & c);

    // get shared link to the coordinate transformation (viewer -> WGS84).
    std::shared_ptr<ConvBase> get_cnv() {return gobj.get_cnv();}

    // Scroll the viewer window to get given coordinates in the center.
    void goto_wgs(dPoint p);

    /**********************************/

    // Load accelerator map from file $HOME/.mapsoft2/mapsoft2.acc
    // If file does not exists then it is ignored silently.
    // In case of parse error a message is printed to stderr.
    // This file is rewrited every time when mapsoft2 program
    // is closed. This function is called in the Mapview constructor.
    void load_acc();

    // Save accelerator map to file $HOME/.mapsoft2/mapsoft2.acc
    // This function is called when Mapview is exiting.
    void save_acc();

    // Read CSS styles. Two files are checked: /usr/share/mapsoft2/mapsoft2.css
    // and $HOME/.mapsoft2/mapsoft2.css. If file does not exists then
    // it is ignored silently. In case of parse error a message is printed to
    // stderr. This function is called in the Mapview constructor.
    void load_css();

};


#endif /* MAPVIEW_H */
