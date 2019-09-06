#ifndef GRED_GOBJ_H
#define GRED_GOBJ_H

#include "geom/rect.h"
#include "image/image.h"
#include <sigc++/sigc++.h>

///\addtogroup gred
///@{
///\defgroup gobj
///@{

/**
An object which know how to draw itself on an Image.

Important: draw() method may be called from a separate thread
(if DThreadViewer is used). A locking maybe needed (data modification
should not be done during drawing).
*/
class GObj{
public:
  const static int FILL_NONE = 0; // object draws nothing
  const static int FILL_PART = 1; // object draws some points
  const static int FILL_ALL  = 2; // object fills in the whole image with opaque colors
  const static iRect MAX_RANGE;

  GObj() {}

  /** Draw on an <img> with <origin> shift.
   \return one of:
   - GObj::FILL_NONE  -- nothing has been drawn
   - GObj::FILL_PART  -- something has been drawn
   - GObj::FILL_ALL   -- all image has been covered with a non-dransparent drawing
   NOTE:
    - range() returns range in viewer coords
  */
  virtual int draw(Image &img, const iPoint &origin) = 0;

  virtual Image get_image (iRect src){
    if (intersect(range(), src).empty()) return Image();
    Image ret(src.w, src.h, IMAGE_32ARGB);
    ret.fill32(0);
    if (draw(ret, src.tlc()) == GObj::FILL_NONE) return Image();
    return ret;
  }

  virtual iRect range(void) const {return GObj::MAX_RANGE;}
  virtual void rescale(double k) {} ///< change scale (refresh must be inside)

  virtual bool get_xloop() const {return false;};
  virtual bool get_yloop() const {return false;}

  sigc::signal<void, iRect> & signal_redraw_me()  {return signal_redraw_me_;}

private:
  sigc::signal<void, iRect> signal_redraw_me_;
};

#endif
