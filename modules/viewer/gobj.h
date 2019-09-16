#ifndef GRED_GOBJ_H
#define GRED_GOBJ_H

#include "geom/rect.h"
#include "opt/opt.h"
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

- Object has its own coordinate system. It should use `cnv` to convert it
  to viewer coordinates (cnv.frw transforms viewer coordinates to object
  coordinates). For the `draw` method the caller should provide
  `draw_range` in viewer coordinates and translate Cairo::Context to
  viewer coordinate origin.
- There is no need to save/restore Cairo::Context in GObj,
  it should be done by caller if needed.
- There is no need to set clip to `draw_range`, it should be done
  by coller if needed.
- `draw` method can be run in a separate thread. To prevent collisions
  use get_lock() method. Method `draw` should be locked by multy-thread
  caller, methods `on_rescale` and `on_set_cnv` are locked in GObj class,
  other functions which modify data should be locked in GObj implementations.
- Usually, `draw` method does not modify data, and read-only access
  from other functions can be done without locking.
  If `draw` method modifies data, read-only functions should be locked as well.
- `stop_drawing` flag is set when current drawing should be aborted.
  Method `draw` can check this flag and stop drawing as soon as possible.
  If one wants to modify data and update everything, following steps are
  needed:
  - set stop_drawing flag
  - get lock object with `get_lock`
  - modify data
  - unset stop_drawing flag
  - emit signal_redraw_me()
*/
class GObj{
protected:
  // reference to drawing options
  std::shared_ptr<Opt> opt;

  // refernce to coordinate transformation:
  // viewer coordinates -> object coordinates.
  std::shared_ptr<ConvBase> cnv;

  // Object coordinate range
  dRect range;
public:

  const static int FILL_NONE = 0; // object draws nothing
  const static int FILL_PART = 1; // object draws some points
  const static int FILL_ALL  = 2; // object fills in the whole image with opaque colors
  const static iRect MAX_RANGE;

  GObj():
    cnv(std::shared_ptr<ConvBase>(new ConvBase)),
    opt(std::shared_ptr<Opt>(new Opt)),
    range(MAX_RANGE), stop_drawing(false) { }

  /** Draw with CairoWrapper.
   \return one of:
   - GObj::FILL_NONE  -- nothing has been drawn
   - GObj::FILL_PART  -- something has been drawn
   - GObj::FILL_ALL   -- all image has been covered with a non-dransparent drawing
   NOTE:
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
    signal_redraw_me_.emit(dRect());
  }

  // change cnv
  virtual void set_cnv(std::shared_ptr<ConvBase> c) {
    stop_drawing = true;
    auto lock = get_lock();
    cnv = c;
    on_set_cnv();
    stop_drawing = false;
    signal_redraw_me_.emit(dRect());
  }
  std::shared_ptr<ConvBase> get_cnv() const {return cnv;}

  // change options
  virtual void set_opt(std::shared_ptr<Opt> o) {
    stop_drawing = true;
    auto lock = get_lock();
    opt = o;
    on_set_opt();
    stop_drawing = false;
    signal_redraw_me_.emit(dRect());
  }
  std::shared_ptr<Opt> get_opt() const {return opt;}

  // This methods show to the caller if picture should be
  // repeated periodically in x or y direction
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
  // there is no need to do it.
  virtual void on_set_opt() {}
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
