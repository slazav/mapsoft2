#ifndef IMAGE_PNG_H
#define IMAGE_PNG_H

#include <string>
#include "geom/point.h"
#include "opt/opt.h"
#include "image/image.h"


// getting file dimensions
iPoint image_size_png(const std::string & file);

// load the whole image
Image image_load_png(const std::string & file, const double scale=1);

// save the whole image
void image_save_png(const Image & im, const std::string & file,
                    const Opt & opt = Opt());

#endif
