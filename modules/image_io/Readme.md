### GIF format support

* image_size_gif() -- Supported, gif screen size is returned.

* image_load_gif() -- Supported. First image only,
image size should match gif screen size. Image scaling is not supported.
All images are loaded as 32-bpp.

* image_save_gif() -- Supported, all images are converted to 8 bpp.
image_colormap options are used for this conversion except cmap_alpha=full
which is substituted by cmap_alpha=gif (no semi-transparent colors).

* Not full support of different libgif versions (now it works with libgif
<4.2, but sume incomplete support is done for 4.2 and 5.0 versions)

### JPEG format support

* image_size_jpeg() -- Supported.

* image_load_jpeg() -- Supported, except image scaling and jpeg denominators.
All images are loaded as 32-bpp.

* image_save_jpeg() -- Supported.

### PNG format support

* image_size_png() -- Supported.

* image_load_png() -- Supported. No scaling. No ADAM7 interlace support.
All images are loaded as 32-bpp.

* image_save_png() -- All image types are supported. PNG type is chosen
according to image type, but can be changed using png_format option
(possible values: argb, rgb, grey, agrey, pal). Only 8 bit per channel
png files can be written: for 16-bit grey images only first byte is
written; all palette images are saved with 1-byte per point.

### TIFF format support

* image_size_tiff() -- Supported.

* image_load_tiff() -- Supported. All images are loaded as 32-bpp.

* image_save_tiff() -- All image types are supported. TIFF type is chosen
according to image type, but can be changed using png_format option
(possible values: argb, rgb, grey, pal). 8 bit per channel tiff files are
written except for IMAGE_16 image type (as in SRTM TIFFs). All palette
images are saved with 1-byte per point. No transparency is supported in
palette images.

### tiled images

Not supported yet.

### tiled TIFF

Not supported yet.
