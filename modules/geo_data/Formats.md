## Mapsoft geodata

`GeoData` structure in mapsoft contains lists of tracks (`GeoTrk`),
waypoint lists (`GeoWptList`), and map lists (`GeoMapList`). Tracks
contain trackpoints (`GeoTpt`), waypoint lists contain waypoints
(`GeoWpt`), and map lists contain maps (`GeoMap`).

Each structure has some mandatory fields and (except for track point)
optional fields inside `opts` variable.

### Fields:

#### `GeoWptList` -- a waypoint list, a route
 * `std::string name` -- name
 * `std::string comm` -- comment
 * `Opt opts` -- optional parameters

#### `GeoWpt` -- a waypoint/route point
 * `double x,y` -- longitude and latitude (degrees, WGS84)
 * `double z`   -- altitude (meters)
 * `time_t t`   -- time (unix milliseconds)
 * `std::string name` -- name
 * `std::string comm` -- comment
 * `Opt opts`         -- optional parameters

#### `GeoTpt` -- a track point
 * `double x,y` -- longitude and latitude (degrees, WGS84)
 * `double z`   -- altitude (meters)
 * `time_t t`   -- time (unix milliseconds)
 * `int start`  -- start segment flag (0|1)

#### `GeoTrk` -- a track
 * `std::string name` -- name
 * `std::string comm` -- comment
 * `Opt opts`         -- optional parameters

#### `GeoMapList` -- a map list
 * `std::string name` -- name
 * `std::string comm` -- comment
 * `Opt opts`         -- optional parameters

#### `GeoMap` -- a map
 * `std::map<dPoint,dPoint> ref` -- reference points, mapping from geo to image coordinates
 * `dLine border`      -- map border (in image coordinates), should be always set
 * `std::string proj`  -- map projection (option string for libproj)
 * `std::string image` -- image file for the map (folder for tile maps)
 * `dRect image_bbox`  -- image boundary box (in image coordinates)
 * `dRect image_tsize` -- image tile size (for tiled maps)
 * `std::string image_tfmt` -- image tile format (for tiled maps)
 * `bool image_tswap`  -- are image tiles swapped in y (for tiled maps)

----------
## GPX format

Mapsoft2 supports reading and writing KML files (tracks and
waypoints).

For waypoints all mandatory
fields (`x`, `y`, `z`, `t`, `name`, `comm`) are supported. Some additional
fields are put in `opts` and can be saved to GPX back:

 * `gpx_magvar` -- Magnetic variation (in degrees) at the point.
 * `gpx_geoidheight` -- Height (in meters) of geoid (mean sea level) above WGS84
 * `gpx_desc`  -- A text description of the element. Holds additional information
   about the element intended for the user, not the GPS.
 * `gpx_src` -- Source of data. Included to give user some idea of reliability
   and accuracy of data. "Garmin eTrex", "USGS quad Boston North", e.g.
 * `gpx_link` -- Link to additional information about the waypoint.
 * `gpx_sym` -- Text of GPS symbol name. For interchange with other programs,
   use the exact spelling of the symbol as displayed on the GPS. If the GPS
   abbreviates words, spell them out.
 * `gpx_type` -- Type (classification) of the waypoint.
 * `gpx_fix` -- Type of GPX fix. (none, 2d, 3d, dgps, pps)
 * `gpx_sat` -- Number of satellites used to calculate the GPX fix.
 * `gpx_hdop` -- Horizontal dilution of precision.
 * `gpx_vdop` -- Vertical dilution of precision.
 * `gpx_pdop` -- Position dilution of precision.
 * `gpx_ageofdgpsdata` -- Number of seconds since last DGPS update.
 * `gpx_dgpsid` -- ID of DGPS station used in differential correction.

`<extension>` tag is skipped.

By default when writing to GPX file all waypoint lists are joined and
written as waypoints. If `gpx_write_rte` option is used then waypoint
lists are written as routes. When reading all waypoints are put to one
waypoint list, and all routes to other waypoint lists.

For tracks and waypoint lists mandatory
fields (`name`, `comm`) are supported. Some additional fields
are put in `opts` and can be saved to GPX again:

 * `gpx_desc` -- Text description of route for user. Not sent to GPS.
 * `gpx_src` -- Source of data. Included to give user some idea of reliability
                and accuracy of data.
 * `gpx_link` -- Links to external information about the route.
 * `gpx_number` -- GPS route number.
 * `gpx_type` -- Type (classification) of route

TODO: `<extension>` tag is skipped. It could be useful to read color from
there: `<extensions><gpxx:TrackExtension><gpxx:DisplayColor>Cyan`
`</gpxx:DisplayColor></gpxx:TrackExtension></extensions>`

Input options:
 - verbose -- write file name

Output options:
 - verbose       -- write file name
 - xml_compr     -- compress the output? 0|1, default 0;
 - xml_indent    -- use indentation? 0|1, default 1;
 - xml_ind_str   -- indentation string, default "  ";
 - xml_qchar     -- quoting character for attributes, default \'
 - gpx_write_rte -- write waypoint lists as routes, 0|1, default 0

----------
## KML format

Mapsoft2 supports reading and writing KML files (tracks and
waypoints).

Waypoints are written as a KML Placemark, all mandatory
fields (`x`, `y`, `z`, `t`, `name`, `comm`) are supported.

Tracks are read or written as a KML Placemark. Open/closed tracks are
supported (`type` field with `open` or `closed` value). All
mandatory fields for tracks and trackpoints are supported except time in
track points.

Waypoint lists are written as KML `<Folder>` tag with all mandatory
fields supported (`name` and `comm`). When reading only Folders with at
least one waypoint are kept as waypoint lists. Empty waypoint list can
not be read from KML (same as in GeoJSON format).

TODO: map support?

Input options:
 - verbose -- write file name

Output options:
 - verbose       -- write file name
 - xml_compr     -- compress the output? 0|1, default 0;
 - xml_indent    -- use indentation? 0|1, default 1;
 - xml_ind_str   -- indentation string, default "  ";
 - xml_qchar     -- quoting character for attributes, default \'

----------
## Garmin Utils format

Mapsoft2 supports reading and writing Garmin Utils files (tracks and
waypoints). It is a very old format, it is not recommended to use it. I
just have many tracks from 199? in this form.

Garmin Utils file can contain a few waypoint lists and a few tracks
without names, comments or any additional information.

Each waypoint is written in a single line. Only `x`, `y`, `name` and
`comm` fields are supported (Remember that Garmin-12 gps was writing time
in the comment field). Spaces in `name` are substituted by `_`.

Each track point is written in a single line. All mandatory fields
(`x, y, z, t, start`) are supported.

Input options:
- verbose -- write file name
- gu_enc -- encoding (default KOI8-R)

Output options:
- verbose -- write file name
- gu_enc -- encoding (default KOI8-R)

----------
## GeoJSON format

Mapsoft2 supports reading and writing GeoJSON files (tracks and
waypoints). All data fields of Mapsoft structures are supported (should
be no data loss except skipping empty waypoint lists).

Waypoint lists are written in separate FeatureCollections. When reading
a FeatureCollection (including the topmost one) is converted to a waypoint
list only if there is at least one waypoint inside. In GeoJSON
FeatureCollections can contain tracks or other FeatureCollections. Mapsoft
converts this to a "flat" stucture with non-empty waypoint lists and tracks.

Track is always written as a Feature with MultiLineString coordinates
(even if it contains one segment), but can be read also from a LineString.

Coordinates of each waypoint or track point are written in array `[lon,
lat, alt, time]`. (GeoJSON format requires only `lat` and `lon` fields
but allows mere values and recommends to have altitude in the fird
field). The array contains three values if time is zero and two values if
altitude is not defined. If time is defined but altitude is not then
`null` is written for the altitude.

Name and comment of track, waypoint, waypoint list are written in `name`
and `cmt` properties. All options are also written to properties.

Input options:
 - verbose (default 0)

Output pptions:
 - verbose (default 0)
 - json_sort_keys (default 1) -- sort json objects by keys
 - json_compact (default 1) -- write compact json
 - json_indent (default 0)  -- use json indentation
 - geo_skip_zt (default 0) -- skip altitude and time information

----------
## OziExplorer format
Mapsoft2 supports reading and writing OziExplorer files (maps, tracks and
waypoints). All data fields of Mapsoft structures are supported.

Each waypoint list, track, and map is written to a separate file.
Format is detected as usual, by file extension (`.wpt`, `.trk`, or `.map`)
or `--fmt ozi` option, but written files have extensions accorting to the data:
`<name>[<number>].wpt` for waypoint lists, `<name>[<number>].trk` for tracks,
`<name>[<number>][_<number>].map` for maps. Multiple files can be created.

For waypoints and tracks all OziExplorer-specific fields are stored in
`opts` and can be saved to a OziExplorer file again.

#### Waypoint list
No fields supported by the format, just a container for waipoints.

#### Waypoint
Mandatory fields:
* `x`,`y`,`z`,`t`, `name`, `comm` -- suppored.
Some conversions are applied to text fields: comma is substituted by
character 209 (cyrillic S), newlines are replaced by spaces,
text length is cropped to 40 symbols.

Optional values (maybe used somewhere else):
* `color` -- integer, 0xRRGGBB
* `bgcolor` -- integer, 0xRRGGBB

Format-specific optional values:
* `ozi_symb` -- integer, 0 to number of symbols in GPS
* `ozi_map_displ` -- Map Display Format
* `ozi_pt_dir` -- Pointer Direction
* `ozi_displ` -- Garmin Display Format
* `ozi_prox_dist` -- Proximity Distance - 0 is off any other number is valid
* `ozi_font_size` -- Font Size - in points
* `ozi_font_style` -- Font Style - 0 is normal, 1 is bold.
* `ozi_symb_size` -- Symbol Size - 17 is normal size
* `ozi_prox_pos` -- Proximity Symbol Position
* `ozi_prox_time` -- Proximity Time
* `ozi_prox_route` -- Proximity or Route or Both
* `ozi_file` -- File Attachment Name
* `ozi_prox_file` -- Proximity File Attachment Name
* `ozi_prox_symb` -- Proximity Symbol Name

#### Track
Mandatory fields:
* `name` -- track name
* `comm` -- NOT SUPPORTED

Optional values:
* `thickness` -- track thickness, integer
* `color` -- integer, 0xRRGGBB
* `bgcolor` -- integer, 0xRRGGBB

Format-specific optional values:
* `ozi_skip` -- track skip value - reduces number of track points plotted, usually set to 1
* `ozi_type` -- track type: 0 - normal, 10 - closed polygon, 20 - Alarm Zone
* `ozi_fill` -- track fill style: 0 - bsSolid; 1 - bsClear; 2 - bsBdiagonal;
3 - bsFdiagonal; 4 - bsCross; 5 - bsDiagCross; 6 - bsHorizontal; 7 - bsVertical

#### Track point
* `x`,`y`,`z`,`t`,`start` -- suppored

#### Map list
Not supported by the format. On reading each map goes to a separate map list
with name same as name of the map. On writing map lists are ignored.

#### Map
- Only a few datums and projections (more can be added).
- Character encoding conversion is applied only to map name,
  not to filename.
- No support for user-defined datums (ellipsoid+shift).
- Full support for reference points, coordinates can be
  in degree or grid format. On output this can be controlled by
  `ozi_map_grid` option.
- Order of reference points is not preserved.
- Moving Map Parameters support.
- No support for Map features, Map comments, Attached files, Grids.

TODO: limit number of border points to 100 (Ozi Explorer limit).

Input options:
 - verbose (default: 0)
 - ozi_enc (default: Windows-1251)

Output pptions:
 - verbose (default 0)
 - ozi_enc (default: Windows-1251)
 - ozi_map_grid -- write grid coordinates in reference points (default 0)
----------
## Mapsoft XML format

In original mapsoft there was a self-made xml-like format for data. There is
no plan to support it.
