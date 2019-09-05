#ifndef IMAGE_COLORS_H
#define IMAGE_COLORS_H

#include <stdint.h>
#include "image.h"

// Reshape color bits to have more sugnificant ones first:
// like 1234 1234 1234 -> 1112 2233 3444
uint32_t image_reduce_color_enc(const uint32_t c);

// Inverse transformation
uint32_t image_reduce_color_dec(const uint32_t c);

// distance between two colors
double color_dist(const uint32_t c1, const uint32_t c2);

// Remove transparency (for scaled colors).
// if gifmode = true, then keep fully transparent colors.
uint32_t color_rem_transp(const uint32_t c, const bool gifmode);

// Create image color palette colors[clen] (0<clen<=256) for reducing color number.
// mode: 0 - keep transparency channel
//       1 - remove transparency
//       2 - remove transparency except fully transparent pixels (GIF mode)
void image_color_mkpal(const Image & img, uint32_t *colors, int clen=256, int mode=0);

// Reduce number of colors using color pallette colors[clen] (0<clen<=256).
// New 8-bpp image is returned. Mode parameter is same as in image_color_mkpal.
Image image_color_reduce(const Image & img, uint32_t *colors, int clen=256, int mode=0);

// Image tranparency (only for 32bpp images)
// returns 0: fully non-transparent image.
// returns 1: image contains transparent (but not semi-transparent) pixels.
// returns 2: image contains only semi-transparent pixels.
int image_classify_alpha(const Image & img);

// Image colors (only for 32bpp images)
// returns 0: grayscale
// returns 1: color,     <=clen colors.
// returns 2: color,     >clen colors.
// colors[clen] array is filled with the color palette.
int image_classify_color(const Image & img, uint32_t *colors, int clen=256);

#endif
