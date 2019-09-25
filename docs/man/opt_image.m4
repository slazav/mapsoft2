=over 2

=item B<<  --tiff_format <arg> >>

When writing tiff, convert image to one of following forms: argb, rgb, grey, pal (default depends on the image).

=item B<<  --tiff_minwhite <arg> >>

When writing greyscale tiff, invert colors (dafault: 0).

=item B<<  --jpeg_quality <arg> >>

Set jpeg quality (default 95).

=item B<<  --png_format <arg> >>

When writing png, convert image to one of following forms: argb, rgb,
grey, agrey, pal (default depends on the image).

=item B<<  --cmap_colors <arg> >>

Colormap size for reducing image colors (2..256, default 256). Image
colors are reduced when saving to GIF, to PNG with B<--png_format=pal>,
to TIFF with B<--tiff_format=pal>

=item B<<  --cmap_alpha <arg> >>

What to do with alpha channel. Values: B<none> (default) -- remove it,
B<full> -- treat it equally with other channels, B<gif> -- keep only one
fully-transparent color if needed. When saving GIF file (which supports
only fully transparent color) B<full> works in the same way as B<gif>.

=item B<<  --cmap_dim_method <arg> >>

Analog of pnmcolormap -spreadbrightness|-spreadluminosity. When a box
in the color space is measured, color dimensions can be
treated equally or with luminosity factors. Values: norm
(default) or lumin.

=item B<<  --cmap_rep_method <arg> >>

Analog of pnmcolormap -center|-meanpixel|-meancolor. When a box in the
color space is chosen, it can be represented by its geometrical center,
or by averaging pixels or colors in it. Values: center, meanpix
(default), meancol.

=item B<<  --cmap_split_method <arg> >>

Analog of pnmcolormap -splitpix|-splitcol|-splitdim. How to choose a box
in the color space for splitting: by its maximum dimension, maximum
number of pixels or colors in it. Values: maxdim (default), maxpix,
maxcol.

=back
