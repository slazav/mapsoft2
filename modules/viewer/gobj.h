#ifndef GRED_GOBJ_H
#define GRED_GOBJ_H

#include "geom/rect.h"
#include "cairo/cairo_wrapper.h"
#include "conv/conv_base.h"
#include <sigc++/sigc++.h>
#include <glibmm.h> // Mutex, Lock
#include <memory> // shared_ptr

///\addtogroup gred
///@{
///\defgroup gobj
///@{

/**
An object which know how to draw itself using Cairo::Context.

Important: draw() method may be called from a separate thread
(if DThreadViewer is used). A locking maybe needed (data modification
should not be done during drawing).
*/
class GObj{
public:
  std::shared_ptr<ConvBase> cnv; //< coversion from viewer coordinates to object
  dRect range;

  const static int FILL_NONE = 0; // object draws nothing
  const static int FILL_PART = 1; // object draws some points
  const static int FILL_ALL  = 2; // object fills in the whole image with opaque colors
  const static iRect MAX_RANGE;

  GObj(std::shared_ptr<ConvBase> c):
    cnv(c), range(MAX_RANGE), stop_drawing(false) { }

  /** Draw with CairoWrapper.
   \return one of:
   - GObj::FILL_NONE  -- nothing has been drawn
   - GObj::FILL_PART  -- something has been drawn
   - GObj::FILL_ALL   -- all image has been covered with a non-dransparent drawing
   NOTE:
    - There is no need to save/restore Cairo::Context in GObj,
      it is done in Viewer.
  */
  virtual int draw(const CairoWrapper & cr, const dRect & draw_range) = 0;

  // return data bounding box (in viewer coords)
  virtual iRect bbox(void) const {return range;}

  // change scale
  virtual void rescale(double k) {
    stop_drawing = true;
    auto lock = get_lock();
    cnv->rescale_src(1.0/k);
    on_rescale(k);
    stop_drawing = false;
  }

  // change cnv
  virtual void set_cnv(std::shared_ptr<ConvBase> c) {
    stop_drawing = true;
    auto lock = get_lock();
    cnv = c;
    on_set_cnv();
    stop_drawing = false;
  }

  virtual bool get_xloop() const {return false;};
  virtual bool get_yloop() const {return false;}

  // signal_redraw_me should be emitted when data was changed and the
  // object has to be redrawn. Normally it is attached to Viewer::redraw
  // method.
  sigc::signal<void, iRect> & signal_redraw_me() {
    return signal_redraw_me_;
  }

private:
  sigc::signal<void, iRect> signal_redraw_me_;

  // Mutex for locking multi-thread operations.
  Glib::Mutex draw_mutex;

public:

  // Can be redefined in the Object implementation to
  // modify data after changing cnv.
  // If cnv is used directly in the draw() method then
  // there is no need to use these methods.
  virtual void on_set_cnv() {}
  virtual void on_rescale(double k) {}

  // If GObj is used from a DThreadViewer then the draw() method
  // is called from a sepereate thread. In this case all modifications
  // of data used in draw() should be locked.
  // - DThreadViewer locks draw() operation,
  // - Gobj locks rescale() and set_cnv() operations.
  // - everything else sould be locked inside the object implementation.
  //
  // Method get_lock() returns the lock object.
  Glib::Mutex::Lock get_lock() {
    return Glib::Mutex::Lock(draw_mutex);
  }

  // stop_drawing flag shows that drawing should be stopped as soon as
  // possible. We set it before doing get_lock() when we want to do
  // any change in sub-objects.
  bool stop_drawing;

};

#endif
