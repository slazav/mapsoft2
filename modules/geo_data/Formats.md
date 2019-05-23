## Mapsoft geodata

`GeoData` structure in mapsoft contains lists of tracks (`GeoTrk`),
waypoint lists (`GeoWptList`), and map lists (`GeoMapList`). Tracks
contain trackpoints (`GeoTpt`), waypoint lists contain waypoints
(`GeoWpt`), and map lists contain maps (`GeoMap`).

Each structure has some mandatory fields and (except for track point)
optional fields inside `opts` variable.

### Mandatory fields:

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
fields are put in `opts` and can be saved to GPX again:

 * `magvar` -- Magnetic variation (in degrees) at the point.
 * `geoidheight` -- Height (in meters) of geoid (mean sea level) above WGS84
 * `desc`  -- A text description of the element. Holds additional information
   about the element intended for the user, not the GPS.
 * `src` -- Source of data. Included to give user some idea of reliability
   and accuracy of data. "Garmin eTrex", "USGS quad Boston North", e.g.
 * `link` -- Link to additional information about the waypoint.
 * `sym` -- Text of GPS symbol name. For interchange with other programs,
   use the exact spelling of the symbol as displayed on the GPS. If the GPS
   abbreviates words, spell them out.
 * `type` -- Type (classification) of the waypoint.
 * `fix` -- Type of GPX fix. (none, 2d, 3d, dgps, pps)
 * `sat` -- Number of satellites used to calculate the GPX fix.
 * `hdop` -- Horizontal dilution of precision.
 * `vdop` -- Vertical dilution of precision.
 * `pdop` -- Position dilution of precision.
 * `ageofdgpsdata` -- Number of seconds since last DGPS update.
 * `dgpsid` -- ID of DGPS station used in differential correction.

`<extension>` tag is skipped.

By default when writing to GPX file all waypoint lists are joined and
written as waypoints. If `gpx_write_rte` option is used then waypoint
lists are written as routes. When reading all waypoints are put to one
waypoint list, and all routes to other waypoint lists.

For tracks and waypoint lists For waypoints mandatory
fields (`name`, `comm`) are supported. Some additional fields
are put in `opts` and can be saved to GPX again:

 * `desc` -- Text description of route for user. Not sent to GPS.
 * `src` -- Source of data. Included to give user some idea of reliability
            and accuracy of data.
 * `link` -- Links to external information about the route.
 * `number` -- GPS route number.
 * `type` -- Type (classification) of route

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

Tracks are read or written as a KML Placemark. Coordinates and altitude
are wrapped in `<MultiGeometry> + <LineString>/<Polygon>` tags. To
control this `opt(type)` is used (can be `open` or `closed`). All
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
Mapsoft2 supports reading and writing OziExplorer files (tracks and
waypoints). All data fields of Mapsoft structures are supported.

Each waypoint list and each track is written to a separate file.
Format is detected as usual, by file extension (`.wpt`, `.trk`, or `.map`)
or `--fmt ozi` option, but wriiten files have extensions accorting to the data:
`<name>[<number>].wpt` for waypoint lists, `<name>[<number>].trk` for tracks,
`<name>[<number>][_<number>].map` for maps.

All OziExplorer-specific fields are stored in `opts` and can be saved to
a OziExplorer file again.

TODO: map support!

Input options:
 - verbose (default: 0)
 - ozi_enc (default: Windows-1251)

Output pptions:
 - verbose (default 0)
 - ozi_enc (default: Windows-1251)
----------
## Mapsoft XML format

In original mapsoft there was a self-made xml-like format for data. There is
no plan to support it.






