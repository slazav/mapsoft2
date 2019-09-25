=head1 NAME

ms2xyz -- writing track in a text form


=head1 USAGE

	ms2view <options> <input files>


=head1 GENERAL OPTIONS:

include(`opt_general.m4')

=over 2

=item B<<  --tshift <arg> >>

Time shift, hours.

=item B<<  -t, --tfmt <arg> >>

Time format string for %T field (default: "%F %T").

=item B<<  -f, --fmt <arg> >>

Format string (default: "%x %y %z %T %D %S").

=item B<<  -w, --win <arg> >>

Window for speed calculation, sec (default: 120).

=item B<<  -b, --break <arg> >>

Place to break calculation and put empty line (none | day | track, default: none).

=item B<<  -o, --out <arg> >>

Output file. If output file is not specified the data is printed to C<stdout>.

=back


=head1 READING GEODATA

include(`opt_geo_in.m4')

=head1 FORMAT

	%% -- % sign
	%x -- wgs lon
	%y -- wgs lat
	%z -- altitude, m
	%t -- time (seconds since 1970-01-01 00:00:00 UTC)
	%T -- date and time in ISO 8601 form (<yyyy-mm-dd>T<HH:MM:SS.FFF>Z)
	%u -- time from the previous point, s
	%d -- % distance from part beginning, km
	%D -- % distance from data beginning, km
	%e -- % distance from the previous point, m
	%S -- % speed, km/h
	%n -- %  point number from part beginning
	%N -- %  point number from data beginning
	%p -- %  part number

=head1 EXAMPLES

Print track as a 3-column table with unix time (seconds), distance
from the beginning, and altitude:

	$ ms2xyz mytrack.plt -f "%t %D %z"
	1559876972 0.000 1238.8
	1559876995 0.031 1226.3
	1559877026 0.050 1185.4
	1559877060 0.051 1186.9
	...

include(`foot.m4')
