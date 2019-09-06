#ifndef IMAGE_TIFF_H
#define IMAGE_TIFF_H

#include <string>
#include "geom/point.h"
#include "image/image.h"

// getting file dimensions
iPoint image_size_tiff(const std::string & file);

Image image_load_tiff(const std::string & file, const int scale=1);

// save the whole image
//void image_save_tiff(const Image & im, const std::string & file);

#endif
