#ifndef IMAGE_GIF_H
#define IMAGE_GIF_H

#include <string>
#include "geom/point.h"
#include "image/image.h"

// getting file dimensions
iPoint image_size_gif(const std::string & file);

// load the whole image
Image image_load_gif(const std::string & file, const int scale=1);

// save the whole image
void image_save_gif(const Image & im, const std::string & file);

#endif
