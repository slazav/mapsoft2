#include "dlg_err.h"

DlgErr::DlgErr():
     Gtk::MessageDialog("", false,
                        Gtk::MESSAGE_WARNING, Gtk::BUTTONS_CLOSE){
  signal_response().connect(
      sigc::hide(sigc::mem_fun(this, &DlgErr::hide)));
  set_position(Gtk::WIN_POS_CENTER_ON_PARENT);
}

void
DlgErr::call(const Err & e){
  std::string msg = std::string("Error: ") + e.str() + "\n";
  if (get_visible()) msg = property_text().get_value() + msg;
  set_message(msg);
  show_all();
}
