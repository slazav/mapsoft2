## Mapsoft2 Image library

------
### Image class

class `Image` in mapsoft2 represents a 2D array or data. Usually it is 
color images. Data can be stored in different forms, some methods for
converting data are also provided. `Image` can be also used as a base class
for implementing images with non-standard data storage.

* type() method returns data type:

  * IMAGE_32ARGB  -- 4 bytes per pixel data: ARGB colors. Prescaled semi-transparent pixels are used.
                     Data is compatable with Cairo::Image Surface data.
  * IMAGE_24RGB   -- 3 bytes per pixel data: RGB colors
  * IMAGE_16      -- 2 bytes per pixel, gryscale, 0=black, 0xFFFF=white
  * IMAGE_8       -- 1 byte per pixel, gryscale, 0=black, 0xFF=white
  * IMAGE_8PAL    -- 1 byte per pixel + color palette
  * IMAGE_1       -- 1 bit per pixel, black and white
  * IMAGE_FLOAT   -- float-value pixel
  * IMAGE_DOUBLE  -- double-value pixel
  * IMAGE_UNKNOWN -- unknown data format

------
### Color handling functions (image.h)

* distance between two colors
``` c++
double color_dist(const uint32_t c1, const uint32_t c2);
```

* Assemble 32-bit color from a,r,g,b components.
Prescaled semi-transparent colors are used.
``` c++
uint32_t color_argb(const uint8_t a, const uint8_t r,
                    const uint8_t g, const uint8_t b);
```

* Remove transparency (with color scaling).
  if gifmode = true, then keep fully transparent colors.
``` c++
uint32_t color_rem_transp(const uint32_t c, const bool gifmode);
```

* Convert RGB color to 8-bit greyscale
``` c++
uint8_t color_rgb_to_grey8(const uint32_t c);
```

* Convert RGB color to 16-bit greyscale
``` c++
uint16_t color_rgb_to_grey16(const uint32_t c);
```

------
### Image handling functions (image_color.h)

* Create a colormap. Based on `pnmcolormap.c` from `netpbm` package.
``` c++
std::vector<uint32_t> image_colormap(const Image & img, const Opt & opt = Opt());
```
Options:
  * `cmap_alpha`        -- Use transparency channel: none (default), full, gif.
  * `cmap_colors`       -- Number of colors, or 0 for all colors (default: 256)
  * `cmap_dim_method`   -- Method for calculating color box dimentions: norm (default), lumin.
  * `cmap_rep_method`   -- Method for color box representation: meanpix (default), meancol, center.
  * `cmap_split_method` -- Method for box splitting: maxdim (default), maxpix, maxcol.


* Reduce number of colors
``` c++
Image image_remap(const Image & img, const std::vector<uint32_t> & cmap,
                  const Opt & opt = Opt());
```
Options:
  * `cmap_alpha` -- Use transparency channel: none (default), full, gif.

* Image tranparency (only for 32bpp images).
Return values: 0 - fully non-transparent image,
1 -- image contains transparent (but not semi-transparent) pixels,
2 -- image contains semi-transparent pixels.
``` c++
int image_classify_alpha(const Image & img);
```
(NOT USED?)

* Image colors (only for 32bpp images).
Return values: 0 -- grayscale, 1 -- color image, number of colors is less or equal ti `clen`.
2 -- color image, more then `clen` colors.
`colors[clen]` array is filled with the color palette.

``` c++
int image_classify_color(const Image & img, uint32_t *colors, int clen=256);
```
(NOT USED?)

* Change image color outside border line.
If border line is empty, set color in the whole image
``` c++
void image_apply_border(const Image & img, const iLine & brd = iLine(),
                        const uint32_t color = 0);
```

------

### Reading raster formats (image_io.h)
* get image size
``` c++
iPoint image_size(const std::string & file, const Opt & opt = Opt());
```

* load the whole image at some scale
``` c++
Image image_load(const std::string & file, const double scale=1, const Opt & opt = Opt());
```

* save the whole image
``` c++
void image_save(const Image & im, const std::string & file, const Opt & opt = Opt());
```
#### Options:

* `png_format`        -- How to save PNG files: argb, rgb, grey, agrey, pal (default depends on the image format)
* `tiff_format`       -- How to save TIFF files: argb, rgb, grey, pal (default depends on the image format)
* `tiff_minwhite`     -- Write IMAGE_8 and IMAGE_16 data to TIFF as MINISWHITE image (default: false)

When converting full-color images to palette images (writing GIF, PNG with `png_format=pal`, or
TIFF with `tiff_format=pal`) colormap options can be used:
* `cmap_alpha`        -- Use transparency channel: none (default), full, gif.
* `cmap_colors`       -- Number of colors, or 0 for all colors (default: 256)
* `cmap_dim_method`   -- Method for calculating color box dimentions: norm (default), lumin.
* `cmap_rep_method`   -- Method for color box representation: meanpix (default), meancol, center.
* `cmap_split_method` -- Method for box splitting: maxdim (default), maxpix, maxcol.

------
### Image cache (image_cache.h)

A cache of raster images (came from mapsoft1 without any changes, not used yet).

------
### GIF format (image_gif.h)

* `image_size_gif()` -- Supported, gif screen size is returned.

* `image_load_gif()` -- Supported. First image only,
image size should match gif screen size. All images are loaded as
`IMAGE_8PAL`.

* `image_save_gif()` -- Supported, all images are converted to 8 bpp.
Standard `image_colormap()` options are used for this conversion except
`cmap_alpha=full` which is substituted by cmap_alpha=gif (no
semi-transparent colors supported).

* Not full support of different libgif versions (now it works with `libgif`
<4.2, but sume incomplete support is done for 4.2 and 5.0)

### JPEG format (image_jpeg.h)

* `image_size_jpeg()` -- Supported.

* `image_load_jpeg()` -- Supported, all images are loaded as IMAGE_24RGB.

* `image_save_jpeg()` -- Supported.

### PNG format (image_png.h)

* `image_size_png()` -- Supported.

* `image_load_png()` -- Supported. No ADAM7 interlace support.

Supported PNG types for loading:

|PNG_COLOR_TYPE| bit/pixel | image type
|--------------|-----------------------
| PALETTE      | any?      | IMAGE_8PAL, 8 bit/pixel, transparency support
| RGB          | 8, 16     | IMAGE_24RGB, 8 bit/pixel
| GRAY         | 1,2,4,8   | IMAGE_8
| GRAY         | 16        | IMAGE_16
| GREY_ALPHA   | any       | IMAGE_32RGBA
| RGB_ALPHA    | 16        | IMAGE_32RGBA


* `image_save_png()` -- All image types are supported. PNG type is chosen
according to image type, but can be changed using `png_format` option
(possible values: `argb`, `rgb`, `grey`, `agrey`, `pal`). Only 8 bit per
channel are used except for 16-bit grey images. All palette images are
saved with 1-byte per point with transparency support.

Supported PNG types for saving:

|  image type   | png_format  | PNG_COLOR_TYPE
|---------------|-------------|-------------
|IMAGE_32ARGB   | argb        | RGB_ALPHA
|IMAGE_24RGB    | rgb         | RGB
|IMAGE_16       |             | GREY, 16 bit per pixel
|IMAGE_8        | grey        | GREY
|-              | agrey       | GREY_ALPHA
|IMAGE_1        |             | PALETTE, 1/pixel
|IMAGE_8PAL     |             | PALETTE, 1/pixel, alpha channel is supported
|IMAGE_FLOAT    | pal         | RGB
|IMAGE_DOUBLE   |             | RGB
|IMAGE_UNKNOWN  |             | RGB

### TIFF format (image_tiff.h)

* `image_size_tiff()` -- Supported.

* `image_load_tiff()` -- Supported.

Supported TIFF types for loading:

| photometric  | samples | bits/samp.| image type
|--------------|---------|-----------|-------
| 0.MINISWHITE |   1     |    16     | IMAGE_16, black is 0
| 0.MINISWHITE |   1     |     8     | IMAGE_8,  black is 0
| 1.MINISBLACK |   1     |    16     | IMAGE_16, black is 0
| 1.MINISBLACK |   1     |     8     | IMAGE_8,  black is 0
| 2.RGB        |   3     |     8     | IMAGE_24RGB
| 2.RGB        |   4     |     8     | IMAGE_32ARGB
| 3.PALETTE    |   1     |     8     | IMAGE_8PAL

* `image_save_tiff()` -- All image types are supported. TIFF type is chosen
according to image type, but can be changed using `tiff_format` option
(possible values: `argb`, `rgb`, `grey`, `pal`). Only 8 bit per channel are used
except for IMAGE_16 image type. All palette images are saved with 1-byte
per point. No transparency is supported in palette images. Writing of
MINISWHITE or MINISBLACK is controlled by `tiff_minwhite` option (default:
false).

Supported TIFF types for saving:

|  image type  | tiff_format | samples | bits/samp.| photometric
|--------------|-------------|---------|-----------|-------------
|IMAGE_32ARGB  | argb        |   4     |    8      | RGB
|IMAGE_24RGB   | rgb         |   3     |    8      | RGB
|IMAGE_16      |             |   1     |   16      | MINISWHITE/MINISBLACK
|IMAGE_8       | grey        |   1     |    8      | MINISWHITE/MINISBLACK
|IMAGE_1       |             |   1     |    8      | MINISWHITE/MINISBLACK
|IMAGE_8PAL    |             |   1     |    8      | PALETTE
|IMAGE_FLOAT   | pal         |   3     |    8      | RGB
|IMAGE_DOUBLE  |             |   3     |    8      | RGB
|IMAGE_UNKNOWN |             |   4     |    8      | RGB


### tiled images

Not supported yet.

### tiled TIFF

Not supported yet.
