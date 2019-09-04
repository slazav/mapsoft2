### GIF format support

* image_size_gif() -- Supported, gif screen size is returned.

* image_load_gif() -- Supported. No transparent color, first image only,
image size should match gif screen size. Image scaling is not supported.
All images are loaded as 32-bpp.

* image_save_gif() -- Supported, all images are converted to 8 bpp (not
in a good way, see image/image_colors.h).

* Not full support of different libgif versions (now it works with libgif
<4.2, but sume incomplete support is done for 4.2 and 5.0 versions)

### JPEG format support

* image_size_jpeg() -- Supported.

* image_load_jpeg() -- Supported, except image scaling and jpeg denominators.

* image_save_jpeg() -- Supported.

### PNG format support

* image_size_png() -- Supported.

* image_load_png() -- Supported. No scaling. No ADAM7 interlace support. All images are loaded as 32-bpp.

* image_save_png() -- Not supported

### TIFF format support

Not supported yet.

### tiled images

Not supported yet.

### tiled TIFF

Not supported yet.
