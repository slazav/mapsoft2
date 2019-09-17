#ifndef MAPVIEW_DLG_CHCONF_H
#define MAPVIEW_DLG_CHCONF_H

#include <gtkmm.h>

// dialog for "data changed' confirmations
// (on exit, starting new project)

class DlgChConf : public Gtk::MessageDialog{
  public:
    DlgChConf();
    void call(const sigc::slot<void> & slot);

  private:
    sigc::signal<void> signal_ok_;
    sigc::slot<void> current_slot;
    void on_result(int r);
};

#endif
