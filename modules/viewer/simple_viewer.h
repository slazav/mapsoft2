#ifndef SIMPLE_VIEWER
#define SIMPLE_VIEWER

#include <gtkmm.h>
#include <cairomm/context.h>
#include "cairo/cairo_wrapper.h"
#include "gobj.h"

///\addtogroup gred
///@{
///\defgroup simple_viewer
///@{

/**
Simple viewer for GObj. One can drag and rescale the object.
Drawing is done on demand.
This viewer is good only for objects which can draw fast.
For slow objects see DThreadViewer.
*/

class SimpleViewer : public Gtk::DrawingArea {
  public:

    SimpleViewer(GObj * o = NULL);

    // Change object.
    virtual void   set_obj (GObj * o) {obj=o; redraw();}

    // Get pointer to the current object.
    virtual GObj * get_obj (void) const {return obj;}

    // Scroll to a position given by object
    // coordinates of the top-left corner of viewer window.
    virtual void   set_origin (iPoint new_origin);

    // Get object coordinates of top-left corner.
    virtual iPoint get_origin (void) const { return origin;}

    // Scroll to a position given by object
    // coordinates of the center of viewer window.
    virtual void   set_center (iPoint new_center){
      set_origin(new_center - iPoint(get_width(), get_height())/2);}

    // Get coordinates of the center.
    virtual iPoint get_center (void) const {
      return origin + iPoint(get_width(), get_height())/2;}

    // Set background color.
    virtual void set_bgcolor(int c) {bgcolor=c | 0xFF000000;}

    // Get background color.
    virtual int get_bgcolor(void) const {return bgcolor;}

    // Return range of object coordinates.
    virtual iRect range() const {
      return obj?obj->bbox():GObj::MAX_RANGE;}

    // Redraw the whole window or a rectangle in it
    virtual void redraw (const iRect & range = iRect());

    virtual void rescale(const double k, const iPoint & cnt);
    virtual void rescale(const double k){
      rescale(k,iPoint(get_width(), get_height())/2);
    }

    // Handler for GTK draw sidnal. It calsulates which parts should be redrawn
    // and calls draw() method.
    virtual bool on_draw (Cairo::RefPtr<Cairo::Context> const & cr) override;

    // Redraw part of the screen (will be overriden in DThreadViewer).
    virtual void draw(const CairoWrapper & crw, const iRect & r);

    virtual bool on_button_press_event (GdkEventButton * event);
    virtual bool on_button_release_event (GdkEventButton * event);
    virtual bool on_motion_notify_event (GdkEventMotion * event);
    virtual bool on_key_press (GdkEventKey * event); // read note in simple_viewer.cpp
    virtual bool on_scroll_event (GdkEventScroll * event);

    virtual bool is_on_drag() const {return on_drag;}

    sigc::signal<void> & signal_busy()        {return signal_busy_;}
    sigc::signal<void> & signal_idle()        {return signal_idle_;}
    sigc::signal<void, double> & signal_on_rescale()  {return signal_on_rescale_;}
    sigc::signal<void, iPoint> & signal_ch_origin()   {return signal_ch_origin_;}
    sigc::signal<void, iPoint, int, const Gdk::ModifierType&> & signal_click() {return signal_click_;}

    GObj * get_gobj() {return obj;}

  private:

    sigc::signal<void> signal_busy_;
    sigc::signal<void> signal_idle_;
    sigc::signal<void, double> signal_on_rescale_;
    sigc::signal<void, iPoint> signal_ch_origin_;
    sigc::signal<void, iPoint, int, const Gdk::ModifierType&> signal_click_;

    GObj * obj;
    iPoint origin;

    bool on_drag;
    iPoint drag_pos, drag_start;
    unsigned int waiting;

    int bgcolor;
    double sc;
};

#endif
