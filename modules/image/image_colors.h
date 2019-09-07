#ifndef IMAGE_COLORS_H
#define IMAGE_COLORS_H

#include <stdint.h>
#include <vector>
#include "image.h"
#include "opt/opt.h"

// Create a colormap.
// Based on pnmcolormap.c from netpbm package.
std::vector<uint32_t> image_colormap(const Image & img, const Opt & opt = Opt());

// Reduce number of colors
Image image_remap(const Image & img, const std::vector<uint32_t> & cmap,
                  const Opt & opt = Opt());

// Image tranparency (only for 32bpp images)
// returns 0: fully non-transparent image.
// returns 1: image contains transparent (but not semi-transparent) pixels.
// returns 2: image contains semi-transparent pixels.
int image_classify_alpha(const Image & img);

// Image colors (only for 32bpp images)
// returns 0: grayscale
// returns 1: color,     <=clen colors.
// returns 2: color,     >clen colors.
// colors[clen] array is filled with the color palette.
int image_classify_color(const Image & img, uint32_t *colors, int clen=256);

#endif
