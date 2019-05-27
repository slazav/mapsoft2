#ifndef IMAGE_GDK_H
#define IMAGE_GDK_H

// Interface between Image and gdk-pixbuf.
// Data is not copied.

#include "image/image.h"
#include <gdkmm/pixbuf.h>

inline Glib::RefPtr<Gdk::Pixbuf> make_pixbuf_from_image (const Image & image){
  if (image.bpp()!=32) throw Err() << "make_pixbuf_from_image: not a 32-bpp image";
  return Gdk::Pixbuf::create_from_data (
          (guint8 *)(image.data()),
          Gdk::COLORSPACE_RGB,
          true, // bool has_alpha,
          8,    // int bits_per_sample
          image.width(), image.height(), 4*image.width());
}


#endif
