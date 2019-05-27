#ifndef VIEWER_IFACE_H
#define VIEWER_IFACE_H

#include <gtkmm.h>
#include "geom/point.h"

///\addtogroup gred
///@{
///\defgroup viewer
///@{

/** Abstract class to be used with ActionManager and Rubber objects.
*/
class Viewer : public Gtk::DrawingArea {
  public:

    virtual void set_origin (iPoint new_origin) = 0;
    virtual iPoint get_origin (void) const = 0;

    virtual bool is_on_drag() const = 0;

    /// These signals are used in Rubber.
    /// They must be called just before and after
    /// actual drawings on Gtk::DrawingArea.
    virtual sigc::signal<void> & signal_before_draw() = 0;
    virtual sigc::signal<void> & signal_after_draw() = 0;

    /// These signals can be used to monitor viewer activity.
    /// They emited before drowing starts and after all tiles have been drawn
    virtual sigc::signal<void> & signal_busy() = 0;
    virtual sigc::signal<void> & signal_idle() = 0;

    virtual sigc::signal<void, double> & signal_on_rescale() = 0;
    virtual sigc::signal<void, iPoint> & signal_ch_origin() = 0;

    virtual sigc::signal<void, iPoint, int, const Gdk::ModifierType&> & signal_click() = 0;
};

#endif
