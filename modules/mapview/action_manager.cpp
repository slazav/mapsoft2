#include "action_manager.h"
#include "mapview.h"

/**** Images *****/

//#include "../images/gps_download.h"
//#include "../images/gps_upload.h"

/**** ActionModes *****/

#include "action_mode.h"

#include "am_file.h"
#include "am_view.h"
//#include "am_open.h"
//#include "am_add_file.h"
//#include "am_save.h"
//#include "am_save_all.h"
//#include "am_save_vis.h"
//#include "am_download.h"
//#include "am_quit.h"

//#include "am_add_wpt.h"
//#include "am_edit_wpt.h"
//#include "am_move_wpt.h"
//#include "am_delete_wpt.h"

//#include "am_add_track.h"
//#include "am_edit_track.h"
//#include "am_del_trk.h"
//#include "am_add_tpt.h"
//#include "am_edit_tpt.h"
//#include "am_move_tpt.h"
//#include "am_delete_tpt.h"
//#include "am_mark_trk.h"
//#include "am_trk_filter.h"
//#include "am_trk_opt.h"

//#include "am_edit_map.h"
//#include "am_setref.h"
//#include "am_llref.h"

//#include "srtm_riv.h"
//#include "srtm_area.h"
//#include "am_srtm_opt.h"
//#include "am_pano.h"

//#include "am_save_image.h"
//#include "am_show_pt.h"
//#include "am_nav.h"

//#include "am_fullscreen.h"
//#include "am_hide_panels.h"

//#include "panel_actions.h"

/*********/

// add action to the menu
#define ADD_ACT(name, group) AddAction(new name(mapview),\
  std::string("Mode") + #name, group);

// add icon
#define ADD_ICON(size, name) \
    icon_factory->add(\
      Gtk::StockID(#name),\
      Gtk::IconSet(\
        Gdk::Pixbuf::create_from_data (idata_##name, Gdk::COLORSPACE_RGB,\
        true /*alpha*/, 8 /*bps*/, size /*w*/, size /*h*/, (size)*4 /*rowstride*/))\
    );

ActionManager::ActionManager (Mapview * mapview_):
  mapview(mapview_), current_mode(0) {

    /***************************************/
    // Add my icons.
/*
    Glib::RefPtr<Gtk::IconFactory> icon_factory = Gtk::IconFactory::create();
    icon_factory->add_default();

    ADD_ICON(16, gps_download);
    ADD_ICON(16, gps_upload);
*/
    /***************************************/
    /// Menues
    actions = Gtk::ActionGroup::create();
    ui_manager = Gtk::UIManager::create();
    ui_manager->insert_action_group(actions);
    mapview->add_accel_group(ui_manager->get_accel_group());

    // empty mode in the begining
    modes.push_back(std::shared_ptr<ActionMode>(new ActionModeNone(mapview)));

    /***************************************/
    // Add actions to menus

    /* Main menu */

    ADD_ACT(New,             "File")
//    ADD_ACT(Open,            "File")
//    ADD_ACT(AddFile,         "File")
//    ADD_ACT(Download,        "File")

//    AddSep("File");

//    ADD_ACT(Save,            "File")
//    ADD_ACT(SaveAll,         "File")
//    ADD_ACT(SaveVis,         "File")
    AddSep("File");
    ADD_ACT(Quit,            "File")

    ADD_ACT(FullScreen,      "View")
    ADD_ACT(HidePanels,      "View")

/*
    ADD_ACT(AddWaypoint,     "Waypoints")
    ADD_ACT(EditWaypoint,    "Waypoints")
    ADD_ACT(MoveWaypoint,    "Waypoints")
    ADD_ACT(DeleteWaypoint,  "Waypoints")

    ADD_ACT(AddTrack,        "Tracks")
    ADD_ACT(EditTrack,       "Tracks")
    ADD_ACT(DelTrk,          "Tracks")
    AddSep("Tracks");
    ADD_ACT(AddTrackpoint,   "Tracks")
    ADD_ACT(EditTrackpoint,  "Tracks")
    ADD_ACT(MoveTrackpoint,  "Tracks")
    ADD_ACT(DeleteTrackpoint,"Tracks")
    AddSep("Tracks");
    ADD_ACT(MarkTrack,       "Tracks")
    ADD_ACT(FilterTrack,     "Tracks")
    ADD_ACT(DrawOpt,         "Tracks")

    ADD_ACT(EditMap,         "Maps")
    ADD_ACT(SetRef,          "Maps")
    ADD_ACT(LLRef,           "Maps")

    ADD_ACT(SrtmRiv,         "SRTM")
    ADD_ACT(SrtmArea,        "SRTM")
    ADD_ACT(Pano,            "SRTM")
    ADD_ACT(SrtmOpt,         "SRTM")

    ADD_ACT(SaveImage,       "Misc")
    ADD_ACT(ShowPt,          "Misc")
    ADD_ACT(Nav,             "Misc")
    AddSep("Misc");

    // Wpt panel menu
    ADD_ACT(PanelGoto,        "PopupWPTs")
    ADD_ACT(PanelSave,        "PopupWPTs")
    AddSep("PopupWPTs");
    ADD_ACT(PanelShowAll,     "PopupWPTs")
    ADD_ACT(PanelHideAll,     "PopupWPTs")
    ADD_ACT(PanelInvert,      "PopupWPTs")
    AddSep("PopupWPTs");
    ADD_ACT(PanelMoveTop,     "PopupWPTs")
    ADD_ACT(PanelMoveUp,      "PopupWPTs")
    ADD_ACT(PanelMoveDown,    "PopupWPTs")
    ADD_ACT(PanelMoveBottom,  "PopupWPTs")
    AddSep("PopupWPTs");
    ADD_ACT(PanelJoinVis,     "PopupWPTs")
    ADD_ACT(PanelJoinAll,     "PopupWPTs")
    AddSep("PopupWPTs");
    ADD_ACT(PanelDelSel,      "PopupWPTs")
    ADD_ACT(PanelDelAll,      "PopupWPTs")

    // Trk panel menu
    ADD_ACT(PanelGoto,        "PopupTRKs")
    ADD_ACT(PanelSave,        "PopupTRKs")
    AddSep("PopupTRKs");
    ADD_ACT(PanelShowAll,     "PopupTRKs")
    ADD_ACT(PanelHideAll,     "PopupTRKs")
    ADD_ACT(PanelInvert,      "PopupTRKs")
    AddSep("PopupTRKs");
    ADD_ACT(PanelMoveTop,     "PopupTRKs")
    ADD_ACT(PanelMoveUp,      "PopupTRKs")
    ADD_ACT(PanelMoveDown,    "PopupTRKs")
    ADD_ACT(PanelMoveBottom,  "PopupTRKs")
    AddSep("PopupTRKs");
    ADD_ACT(PanelJoinVis,     "PopupTRKs")
    ADD_ACT(PanelJoinAll,     "PopupTRKs")
    AddSep("PopupTRKs");
    ADD_ACT(PanelDelSel,      "PopupTRKs")
    ADD_ACT(PanelDelAll,      "PopupTRKs")
    AddSep("PopupTRKs");
    ADD_ACT(DrawOpt,          "PopupTRKs")

    // Map panel menu
    ADD_ACT(PanelGoto,        "PopupMAPs")
    ADD_ACT(PanelSave,        "PopupMAPs")
    AddSep("PopupMAPs");
    ADD_ACT(PanelShowAll,     "PopupMAPs")
    ADD_ACT(PanelHideAll,     "PopupMAPs")
    ADD_ACT(PanelInvert,      "PopupMAPs")
    AddSep("PopupMAPs");
    ADD_ACT(PanelMoveTop,     "PopupMAPs")
    ADD_ACT(PanelMoveUp,      "PopupMAPs")
    ADD_ACT(PanelMoveDown,    "PopupMAPs")
    ADD_ACT(PanelMoveBottom,  "PopupMAPs")
    AddSep("PopupMAPs");
    ADD_ACT(PanelJoinVis,     "PopupMAPs")
    ADD_ACT(PanelJoinAll,     "PopupMAPs")
    AddSep("PopupMAPs");
    ADD_ACT(PanelDelSel,      "PopupMAPs")
    ADD_ACT(PanelDelAll,      "PopupMAPs")

    // SRTM panel menu
    ADD_ACT(SrtmOpt,          "PopupSRTM")
*/
    /***************************************/

    /* Cleate menus */
    actions->add(Gtk::Action::create("MenuFile", "_File"));
    actions->add(Gtk::Action::create("MenuView", "_View"));
//    actions->add(Gtk::Action::create("MenuWaypoints", "_Waypoints"));
//    actions->add(Gtk::Action::create("MenuTracks", "_Tracks"));
//    actions->add(Gtk::Action::create("MenuMaps", "_Maps"));
//    actions->add(Gtk::Action::create("MenuSRTM", "_SRTM"));
//    actions->add(Gtk::Action::create("MenuMisc", "Mi_sc"));
    popup_wpts = (Gtk::Menu *)ui_manager->get_widget("/PopupWPTs");
    popup_trks = (Gtk::Menu *)ui_manager->get_widget("/PopupTRKs");
    popup_maps = (Gtk::Menu *)ui_manager->get_widget("/PopupMAPs");
//    popup_srtm = (Gtk::Menu *)ui_manager->get_widget("/PopupSRTM");

    // panels mouse button events -> popup menus
    mapview->panel_wpts->set_events(Gdk::BUTTON_PRESS_MASK);
    mapview->panel_trks->set_events(Gdk::BUTTON_PRESS_MASK);
    mapview->panel_maps->set_events(Gdk::BUTTON_PRESS_MASK);
//    mapview->panel_misc->set_events(Gdk::BUTTON_PRESS_MASK);
    mapview->panel_wpts->signal_button_press_event().connect(
      sigc::mem_fun (this, &ActionManager::on_panel_button_press), false);
    mapview->panel_trks->signal_button_press_event().connect(
      sigc::mem_fun (this, &ActionManager::on_panel_button_press), false);
    mapview->panel_maps->signal_button_press_event().connect(
      sigc::mem_fun (this, &ActionManager::on_panel_button_press), false);
//    mapview->panel_misc->signal_button_press_event().connect(
//      sigc::mem_fun (this, &ActionManager::on_panel_button_press), false);

    /// viewer mouse click -> action manager click
    mapview->viewer.signal_click().connect (
      sigc::mem_fun (this, &ActionManager::click));
}


void
ActionManager::AddSep(const std::string & menu){
  if (menu.substr(0,5) == "Popup"){
    ui_manager->add_ui_from_string(
      "<ui>"
      "  <popup name='" + menu + "'>"
      "    <separator/>"
      "  </popup>"
      "</ui>"
    );
  }
  else{
    ui_manager->add_ui_from_string(
      "<ui>"
      "  <menubar name='MenuBar'>"
      "    <menu action='Menu" + menu + "'>"
      "      <separator/>"
      "    </menu>"
      "  </menubar>"
      "</ui>"
    );
  }
}

void
ActionManager::AddAction(ActionMode *action,
                         const std::string & id, const std::string & menu){
  modes.push_back(std::shared_ptr<ActionMode>(action));
  int m = modes.size()-1;
  std::string  mname   = action->get_name();
  Gtk::StockID stockid = action->get_stockid();
  Gtk::AccelKey acckey = action->get_acckey();

  if (!actions->get_action(id)){
    // I do not know how to create empty editable AccelKey. So i use
    // these stupid ifs...
    if (acckey.is_null())
      actions->add(
        Gtk::Action::create(id, stockid, mname),
        sigc::bind (sigc::mem_fun(this, &ActionManager::set_mode), m));
    else
      actions->add(
        Gtk::Action::create(id, stockid, mname), acckey,
        sigc::bind (sigc::mem_fun(this, &ActionManager::set_mode), m));
  }

  if (menu.substr(0,5) == "Popup"){
    ui_manager->add_ui_from_string(
      "<ui>"
      "  <popup name='" + menu + "'>"
      "    <menuitem action='" + id + "'/>"
      "  </popup>"
      "</ui>"
    );
  }
  else{
    ui_manager->add_ui_from_string(
      "<ui>"
      "  <menubar name='MenuBar'>"
      "    <menu action='Menu" + menu + "'>"
      "      <menuitem action='" + id + "'/>"
      "    </menu>"
      "  </menubar>"
      "</ui>"
    );
  }
}

bool
ActionManager::on_panel_button_press (GdkEventButton * event) {
  if (event->button == 3) {
    Gtk::Menu * M = 0;
    switch (mapview->panels->get_current_page()){
      case 0: M = popup_wpts; break;
      case 1: M = popup_trks; break;
      case 2: M = popup_maps; break;
      case 3: M = popup_srtm; break;
    }
    if (M) M->popup(event->button, event->time);
    return true;
  }
  return false;
}

void
ActionManager::clear_state (){
  mapview->rubber.clear();
  modes[current_mode]->abort();
}

void
ActionManager::set_mode (int mode){
  if (modes[mode]->is_radio()){
    clear_state();
    mapview->spanel.message(modes[mode]->get_name());
    current_mode = mode;
  }
  modes[mode]->activate();
}
