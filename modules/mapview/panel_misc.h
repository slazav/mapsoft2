#ifndef MAPVIEW_PANEL_MISC_H
#define MAPVIEW_PANEL_MISC_H

#include "workplane.h"
#include "img_io/gobj_srtm.h"
#include "img_io/gobj_srtmv.h"
#include "img_io/gobj_grid_pulk.h"


/* panel for srtm, grids etc. */
class PanelMISC: public Gtk::VBox{
  Workplane wpd,wph; // deep/high workplanes
public:
  PanelMISC():
    srtm("",20),
    gobj_srtm(&srtm),
    gobj_srtmv(&srtm)
  {
    srtm_status      = manage(new Gtk::CheckButton("SRTM data"));
    srtmv_status     = manage(new Gtk::CheckButton("SRTM contours"));
    grid_pulk_status = manage(new Gtk::CheckButton("Pulkovo-1942 grid"));

    pack_start(*srtm_status, false, true, 0);
    pack_start(*srtmv_status, false, true, 0);
    pack_start(*grid_pulk_status, false, true, 0);

    srtm_status->signal_toggled().connect(
      sigc::mem_fun(this, &PanelMISC::ch_srtm));
    srtmv_status->signal_toggled().connect(
      sigc::mem_fun(this, &PanelMISC::ch_srtmv));
    grid_pulk_status->signal_toggled().connect(
      sigc::mem_fun(this, &PanelMISC::ch_grid_pulk));
  }

  Options get_opt() const {
    Options o = gobj_srtm.get_opt();
    o.put<string>("srtm_dir", srtm.get_dir());
    return o;}

  void set_opt(const Options & o){
    srtm.set_dir(o.get<string>("srtm_dir", ""));
    gobj_srtm.set_opt(o);
    gobj_srtmv.set_opt(o);
    if (wpd.exists(&gobj_srtm))  wpd.refresh_gobj(&gobj_srtm);
    if (wph.exists(&gobj_srtmv)) wph.refresh_gobj(&gobj_srtmv);
  }

  Workplane * get_wpd(){return &wpd;}
  Workplane * get_wph(){return &wph;}

  void ch_srtm(){
//    if (!mapview->have_reference) return;
    bool show = srtm_status->get_active();
    bool state = wpd.exists(&gobj_srtm);
    if (state && !show){
      wpd.remove_gobj(&gobj_srtm);
      wpd.signal_redraw_me().emit(iRect());
    }
    else if (!state && show){
      wpd.add_gobj(&gobj_srtm, 0);
      wpd.refresh_gobj(&gobj_srtm);
    }
  }

  void ch_srtmv(){
//    if (!mapview->have_reference) return;
    bool show = srtmv_status->get_active();
    bool state = wph.exists(&gobj_srtmv);
    if (state && !show){
      wph.remove_gobj(&gobj_srtmv);
      wph.signal_redraw_me().emit(iRect());
    }
    else if (!state && show){
      wph.add_gobj(&gobj_srtmv, 0);
      wph.refresh_gobj(&gobj_srtmv);
    }
  }

  void ch_grid_pulk(){
//    if (!mapview->have_reference) return;
    bool show = grid_pulk_status->get_active();
    bool state = wph.exists(&gobj_grid_pulk);
    if (state && !show){
      wph.remove_gobj(&gobj_grid_pulk);
      wph.signal_redraw_me().emit(iRect());
    }
    else if (!state && show){
      wph.add_gobj(&gobj_grid_pulk, 0);
      wph.refresh_gobj(&gobj_grid_pulk);
    }
  }

  void hide_all(){
    srtm_status->set_active(false);
    srtmv_status->set_active(false);
    grid_pulk_status->set_active(false);
  }

  SRTM3 srtm;
  GObjSRTM     gobj_srtm;
  GObjSRTMv    gobj_srtmv;
  GObjGridPulk gobj_grid_pulk;
  Gtk::CheckButton * srtm_status,
                   * srtmv_status,
                   * grid_pulk_status;
};

#endif
