=head1 NAME

ms2conv -- mapsoft2 converter for geodata and raster maps


=head1 USAGE

	ms2conv <options> <input files> -o <output file>

=head1 GENERAL OPTIONS

include(`opt_general.m4')

=over 1

=item B<<  -o, --out <arg> >>

output file

=back


=head1 READING AND WRITING GEODATA

include(`opt_geodata.m4')

=head1 RENDERING IMAGES

B<ms2conv> can produce images with raster maps, tracks and waypoints.
Supported formats: B<jpeg, png, gif, tiff, ps, pdf, svg>. Format is
selected by output file extension or by B<--out_fmt> option. Option
B<--map> can be used to save map reference for the image (at the moment
only OziExplorer map format is supported).

=head2 Options for saving images

include(`opt_image.m4')

=over 2

=item B<<  --bgcolor <arg> >>

Image background color (default 0xFFFFFFFF).

=item B<<  -m, --map <arg> >>

Write map file in OziExprorer format. See section I<READING AND WRITING GEODATA>
for options controlling OziExprorer map file saving.

=back

=head2 Setting up map reference for images

include(`opt_mkref.m4')

include(`opt_render.m4')

include(`foot.m4')
