#ifndef IMAGE_JPEG_H
#define IMAGE_JPEG_H

#include <string>
#include "geom/point.h"
#include "image/image.h"

// getting file dimensions
iPoint image_size_jpeg(const std::string & file);

// load the whole image
Image image_load_jpeg(const std::string & file, const double scale=1);

// save the whole image
void image_save_jpeg(const Image & im, const char * file,
                     const Opt & opt = Opt());

#endif
