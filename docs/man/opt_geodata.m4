=over 2

=item B<<  --in_fmt <arg> >>

Explicitly set geodata input format (json, gu, gpx, kml, kmz, ozi, zip).
By default format is determined by file extension.

=item B<<  --out_fmt <arg> >>

Explicitly set geodata output format (json, gu, gpx, kml, kmz, ozi, zip).
By default format is determined by file extension.

=item B<<  --gu_enc <arg> >>

Garmin Utils format charset (default KOI8-R).

=item B<<  --ozi_enc <arg> >>

OziExplorer format charset (default Windows-1251).

=item B<<  --xml_compr <arg> >>

Compress xml output (GPX and KML formats), default 0.

=item B<<  --xml_indent <arg> >>

Use XML indentation (GPX and KML formats), default 1.

=item B<<  --xml_ind_str <arg> >>

XML indentation string (GPX and KML formats), default '  '.

=item B<<  --xml_qchar <arg> >>

XML quoting character (GPX and KML formats), default '''.

=item B<<  --gpx_write_rte <arg> >>

Write waypoint lists as routes (GPX format), default 0.

=item B<<  --json_sort_keys <arg> >>

Sort json objects by keys (GeoJSON format), default 1.

=item B<<  --json_compact <arg> >>

Write compact json (GeoJSON format), default 0.

=item B<<  --json_indent <arg> >>

Use json indentation (GeoJSON format), default 1.

=item B<<  --geo_skip_zt <arg> >>

Skip altitude and time information when writing GeoJSON, default 0.

=item B<<  --ozi_map_grid <arg> >>

Write map reference points in grid coordinates (OziExplorer format), default 0.

=item B<<  --ozi_map_wgs <arg> >>

Always use wgs84 datum for map coordinates (OziExplorer format), default 0.
By default map coordinates are written in map datum if it is supported.

=back
