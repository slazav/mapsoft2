=head1 NAME

ms2tiles -- TMS/Google tile calculations

=head1 USAGE

	ms2tiles [-G] -p <point> <z> -- tiles at the point
	ms2tiles [-G] -r <range> <z> -- tiles covering the range
	ms2tiles [-G] -n <tile-point> -- tile range
	ms2tiles [-G] -c <tile-point> -- tile center
	ms2tiles [-G] -t <tile-point> <range>  -- check if the tile touches the range

Order of options is important.

=head1 DESCRIPTION

By default the program works with TMS tiles. Use -G option
for google tiles.

Parameters <point> and <range> are JSON arrays of WGS84
coordinates: [<lon>,<lat>], and [<lon>,<lat>,<width>,<height>].
Parameter <tile-point> is JSON array of three integers: [<x>,<y>,<z>]

=head1 EXAMPLES

A point with coordinates lon=69.1, lat=36.1 is located on the TMS tile
with z=13, x=5668, y=4977:

	$ ms2tiles -G -p "[69.1,36.1]" 13
	[5668,4977]

A coordinate range 1x1 degree starting at lon=69, lat=36 is covered by
23x29 Google tiles with z=13, starting with tile x=5666, y=3188:

	$ ms2tiles -G -r "[69,36,1,1]" 13
	[5666,3188,23,29]

Finding a coordinate range for a TMS tile x=5666, y=4975, z=13

	$ ms2tiles -n "[5666,4975,13]"
	[68.9941406,35.9957854,0.0439453125,0.0355463899]

Finding center of a tile:

	$ ms2tiles -c "[5666,4975,13]"
	[69.0161133,36.0135586]

Checking if a tile touches a coordinate range:

	$ ms2tiles -t "[5666,4975,13]" "[69,36,1,1]"; echo $?
	0
	$ ms2tiles -t "[5666,4975,13]" "[59,36,1,1]"; echo $?
	1

include(`foot.m4')
