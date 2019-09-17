#ifndef DLG_ERR_H
#define DLG_ERR_H

#include <gtkmm.h>
#include "err/err.h"

// dialog for error messages

class DlgErr : public Gtk::MessageDialog{
  public:
    DlgErr();
    void call(const Err & e);
};

#endif
