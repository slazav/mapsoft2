=head1 NAME

ms2nom -- Soviet nomenclature map calculations

=head1 USAGE

	ms2nom [-E] -p <point> <scale> -- maps at the point
	ms2nom [-E] -r <range> <scale> -- maps at the range
	ms2nom [-E] -n <name> -- map range
	ms2nom [-E] -c <name> -- map center
	ms2nom [-E] -s <name> x_shift y_shift -- adjacent map
	ms2nom [-E] -S <name> <scale> -- maps of different scale
	ms2nom [-E] -t <name> <range>  -- check if the map touches the range

Order of options is important.

=head1 DESCRIPTION

Program C<ms2nom> does some calculations with standard Soviet
nomenclature map names.

Option C<-E> turns on I<extended mode>: single sheets (like C<Q10-001>) are
allowed on input and always returned on output; for a single sheet suffix
C<< .<N>x<M> >> is allowed to multiply the range (like C<n49-001.3x2>).

At the moment combination of C<-E> and C<-s> options with such a
"multiplyed" name returns non-multiplied adjecent sheets. This is not
very useful and maybe changed later.

Soviet nomenclature maps are drawn in Krassovsky-1942 datum
C<(+ellps=krass +towgs84=28,-130,-95)>. The program does not do
any coordinate conversions.

Supported scales are: C<1:1000000, 1:500000, 1:200000, 1:100000, 1:50000>.
Scale can be written in following forms:

=over 1

=item * 1:1000000, 1:1'000'000, 1000000, 1M, 10km/cm

=item * 1:500000, 1:500'000, 500000, 500k, 5km/cm

=item * 1:200000, 1:200'000, 200000, 100k, 1km/cm

=item * 1:100000, 1:100'000, 100000, 100k, 1km/cm

=item * 1:50000, 1:50'000, 50000, 50k, 500m/cm

=back

=head1 EXAMPLES

Which 1:100000 map is covering a point lon=69, lat=36:

	$ ms2nom -p "[69,36]" 1:100000
	j42-139

Which 1:100000 maps are touching range lon=36.0-36.5, lat=70.0-70.5:

	$ ms2nom -r "[36,70,0.5,0.5]" 1:100000
	r37-049,050
	r37-061,062

Coordinate range and center of j42-1 map:

	$ ms2nom -n j42-1
	[66,38,3,2]

	$ ms2nom -c j42-1
	[67.5,39]

Right neighbour of j42-112-2 map:

	$ ms2nom -s j42-112-2 1 0
	j42-113-1

List of 1:50'000 maps for p42-111,112 map:
	$ ms2nom -S p42-111,112 50k
	p42-111-1,2
	p42-111-3,4
	p42-112-1,2
	p42-112-3,4

Test if map touches a coordinate range:

	$ ms2nom -t n37 "[36,52,1,1]"; echo $?
	0
	$ ms2nom -t n37 "[26,52,1,1]"; echo $?
	1

include(`foot.m4')
