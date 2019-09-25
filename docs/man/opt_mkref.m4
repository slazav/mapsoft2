Map reference is set similarly in C<ms2view>, C<ms2conv>, and C<ms2mkref>
programs. If B<--mkref> option exists then projection is created from
options. If not then C<ms2view> and C<ms2conv> get projection of the
first map to be shown or set to some default value. Following options can
be used:

=over 2

=item B<< --mkref <type> >>

Choose map type. Possible values are: B<nom> -- a
Soviet nomenclature map, B<proj> -- a rectangular area in some
coordinates, B<google_tile> and B<tms_tile> -- a single google/TMS tile
or rectangular area with multiple tiles.

=item B<< --name <string> >>

Set map name.

=item B<< --dpi <value> >>

Map resolution, pixels per inch (B<nom> and B<proj> maps)

=item B<< --mag <value> >>

Map magnification (B<nom> maps).

=item B<< --margins <value> >>

Map margins, pixels (B<nom> and B<proj> maps).

=item B<< --top_margin <value> >>

=item B<< --left_margin <value> >>

=item B<< --right_margin <value> >>

=item B<< --bottom_margin <value> >>

Override top/left/right/bottom margin values (B<nom> and B<proj> maps).

=item B<< --zindex <value> >>

Tile zindex (B<google_tile> and B<tms_tile> maps).

=item B<< --tiles <value> >>

Tile or tile range (B<google_tile> and B<tms_tile> maps)

=item B<< --coords <value> >>

Figure in projection coordinates (rectangle or line) to be covered by the
map (B<proj> maps). Figure can be a rectangle written as C<[x,y,w,h]>,
or a line, C<[[x1,y1],[x2,y2], ...]>, or a multi-segment line, C<< [<line>, <line>, ...] >>.

=item B<< --border <value> >>

Map border in projection coordinates (B<proj> maps), a line or a multi-segment line.

=item B<< --coords_wgs <value> >>

Figure in wgs84 coordinates to be covered by the map (B<google_tile>,
B<tms_tile>, or B<proj> maps).

=item B<< --border_wgs <value> >>

Map border in wgs84 coordinates (B<google_tile>, B<tms_tile>, or B<proj> maps).

=item B<< --proj <value> >>

Projection setting (for B<proj> maps), B<libproj> parameter string (e.g.
I<+datum=WGS84 +proj=lonlat>) or mapsoft2 alias (B<WGS>, B<WEB>, B<FI>,
B<CH>, B<SU39>, etc.). Default is B<WGS>.

=item B<< --scale <value> >>

Map scale (for B<proj> maps), projection units per map cm (B<proj> maps).
Default value is 0.01 degree/cm for degree projections, 1000m/cm for
metric projections.

=back
