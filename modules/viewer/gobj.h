#ifndef GRED_GOBJ_H
#define GRED_GOBJ_H

#include "geom/rect.h"
#include "cairo/cairo_wrapper.h"
#include "conv/conv_base.h"
#include <sigc++/sigc++.h>

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
  ConvBase & cnv; //< coversion from viewer coordinates to object
  dRect range;

  const static int FILL_NONE = 0; // object draws nothing
  const static int FILL_PART = 1; // object draws some points
  const static int FILL_ALL  = 2; // object fills in the whole image with opaque colors
  const static iRect MAX_RANGE;

  GObj(ConvBase & c): cnv(c), range(MAX_RANGE) { }

  /** Draw on an <img> with <origin> shift.
   \return one of:
   - GObj::FILL_NONE  -- nothing has been drawn
   - GObj::FILL_PART  -- something has been drawn
   - GObj::FILL_ALL   -- all image has been covered with a non-dransparent drawing
   NOTE:
    - range() returns range in viewer coords
  */
  virtual int draw(const CairoWrapper & cr, const iPoint &origin) = 0;

  // return data bounding box
  virtual iRect bbox(void) const {return range;}

  // change scale (refresh must be inside)
  virtual void rescale(double k) {
    cnv.rescale_src(k);
    on_rescale(k);
  }

  virtual bool get_xloop() const {return false;};
  virtual bool get_yloop() const {return false;}

  // signal_redraw_me should be emitted when data was changed and the
  // object has to be redrawn. Normally it is attached to Viewer::redraw
  // method.
  sigc::signal<void, iRect> & signal_redraw_me()  {return signal_redraw_me_;}

private:
  sigc::signal<void, iRect> signal_redraw_me_;

protected:
  virtual void on_change_cnv() {}
  virtual void on_rescale(double k) {}

};

#endif
