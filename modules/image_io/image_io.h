#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include <string>
#include "image/image.h"
#include "opt/opt.h"
#include "geom/point.h"

// get image size
iPoint image_size(const std::string & file, const Opt & opt = Opt());

// load the whole image at some scale
Image image_load(const std::string & file, const double scale=1, const Opt & opt = Opt());

// save the whole image
void image_save(const Image & im, const std::string & file, const Opt & opt = Opt());


#endif
