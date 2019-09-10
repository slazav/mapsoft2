### GIF format support

* image_size_gif() -- Supported, gif screen size is returned.

* image_load_gif() -- Supported. First image only,
image size should match gif screen size. All images are loaded as
IMAGE_8PAL.

* image_save_gif() -- Supported, all images are converted to 8 bpp.
Standard image_colormap() options are used for this conversion except
cmap_alpha=full which is substituted by cmap_alpha=gif (no
semi-transparent colors supported).

* Not full support of different libgif versions (now it works with libgif
<4.2, but sume incomplete support is done for 4.2 and 5.0)

### JPEG format support

* image_size_jpeg() -- Supported.

* image_load_jpeg() -- Supported, all images are loaded as IMAGE_24RGB.

* image_save_jpeg() -- Supported.

### PNG format support

* image_size_png() -- Supported.



* image_load_png() -- Supported. No ADAM7 interlace support.

Supported PNG types for loading:

|PNG_COLOR_TYPE| bit/pixel | image type
|--------------|-----------------------
| PALETTE      | any?      | IMAGE_8PAL, 8 bit/pixel, transparency support
| RGB          | 8, 16     | IMAGE_24RGB, 8 bit/pixel
| GRAY         | 1,2,4,8   | IMAGE_8
| GRAY         | 16        | IMAGE_16
| GREY_ALPHA   | any       | IMAGE_32RGBA
| RGB_ALPHA    | 16        | IMAGE_32RGBA


* image_save_png() -- All image types are supported. PNG type is chosen
according to image type, but can be changed using png_format option
(possible values: argb, rgb, grey, agrey, pal). Only 8 bit per channel
are used except for 16-bit grey images. All palette images
are saved with 1-byte per point with transparency support.

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

### TIFF format support

* image_size_tiff() -- Supported.

* image_load_tiff() -- Supported.

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

* image_save_tiff() -- All image types are supported. TIFF type is chosen
according to image type, but can be changed using tiff_format option
(possible values: argb, rgb, grey, pal). Only 8 bit per channel are used
except for IMAGE_16 image type. All palette images are saved with 1-byte
per point. No transparency is supported in palette images. Writing of
MINISWHITE or MINISBLACK is controlled by tiff_minwhite option (default:
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
