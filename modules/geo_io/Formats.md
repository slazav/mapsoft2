## Mapsoft geodata

Data structure used in mapsoft2. In some formats
additional fields can be kept in `opts` variable.

### `GeoWpt` -- a waypoint/route point
 * x,y -- longitude and latitude (degrees, WGS84)
 * z -- altitude (meters)
 * t -- time (unix milliseconds)
 * name  -- name
 * comm  -- comment

### `GeoTpt` -- a track point
 * x,y -- longitude and latitude (degrees, WGS84)
 * z -- altitude (meters)
 * t -- time (unix milliseconds)
 * start -- start segment flag (0|1)

### `GeoTrk` -- a track
 * name -- name
 * comm -- comment

### `GeoWptList` -- a waypoint list, a route
 * name -- name
 * comm -- comment

### `GeoMap` -- a map

----------
## GPX format

* GPX 1.1 format: http://www.topografix.com/GPX/1/1/
* GPX 1.0 format: http://www.topografix.com/gpx_manual.asp
* example with all fields: https://github.com/tkrajina/gpxgo/tree/master/test_files

### `GeoWpt` -- a waypoint/route point
 * `x,y`  -- read/write support, `<lat>` and `<lon>` tags (degrees, WGS84)
 * `z`    -- read/write support, `<ele>` tag (meters)
 * `t`    -- read/write support, `<time>` tag (ISO UTC time with millisecond precision)
 * `name` -- The GPS name of the waypoint. This field will be transferred to and from
   the GPS. GPX does not place restrictions on the length of this field or
   the characters contained in it. It is up to the receiving application to
   validate the field before sending it to the GPS.
 * `comm` -- <cmt> tag. GPS waypoint comment. Sent to GPS as comment.

Additional fields (read/write support, put in options):

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

### `GeoTpt` -- a track point
 * `x,y` -- read/write support, `<lat>` and `<lon>` tags (degrees, WGS84)
 * `z`   -- read/write support, `<ele>` tag (meters)
 * `t`   -- read/write support, `<time>` tag (ISO UTC time with millisecond precision)
 * `start` -- read/write support (track segments)

### `GeoTrk`, `GeoWptList` -- track, a waypoint list, route

When writing to GPX file by default all waypoint lists are joined and
written as waypoints. If `gpx_write_rte` option is used then waypoint
lists are written as routes.

Additional fields (read/write support, put in options):

 * `name` -- GPS name of route.
 * `comm` -- `<cmt>` tag. GPS comment for route.
 * `desc` -- Text description of route for user. Not sent to GPS.
 * `src` -- Source of data. Included to give user some idea of reliability
            and accuracy of data.
 * `link` -- Links to external information about the route.
 * `number` -- GPS route number.
 * `type` -- Type (classification) of route

`<extension>` tag is skipped. It could be useful to read color from there:
`<extensions><gpxx:TrackExtension><gpxx:DisplayColor>Cyan`
`</gpxx:DisplayColor></gpxx:TrackExtension></extensions>`

### `GeoMap` -- a map

Not supported by GPX format.

----------
## KML format

### `GeoWpt` -- a waypoint
Waypoint is written as KML Placemark.
 * `x, y, z` -- read/write support, `<coordinates>` tag.
 * `t`       -- read/write support, `<TimeStamp>` tag.
 * `name`    -- read/write support, `<name>` tag.
 * `comm`    -- read/write support, `<description>` tag.

### `GeoTpt` -- a track point
 * `x, y, z` -- read/write support, `<coordinates>` tag.
 * `t`       -- not supported by KML format.
 * `start`   -- read/write support (track segments).

### `GeoTrk` -- track
Track is written as KML Placemark. Coordinate and altitude
data are wrapped in `<MultiGeometry> + <LineString>/<Polygon>`
tags.
 * `name`    -- read/write support, `<name>` tag.
 * `comm`    -- read/write support, `<description>` tag.
 * `opt("type")` -- open/closed

### `GeoWptList` -- a waypoint list, a route
Waypoint lists are written as KML `<Folder>` tag.
 * `name`    -- read/write support, `<name>` tag.
 * `comm`    -- read/write support, `<description>` tag.

### `GeoMap` -- a map

Not supported.

----------
## Garmin Utils format

### `GeoWpt` -- a waypoint
 * `x, y, z` -- read/write support
 * `t`    -- not supported by Garmin Utils format
 * `name` -- read/write support. Spaces will be replaced by '_'.
 * `comm` -- read/write support

### `GeoTpt` -- a track point
 * `x, y, z, t, start` -- read/write support

### `GeoTrk`, `GeoWptList`  -- track, a waypoint list
 * `name`    -- not supported.
 * `comm`    -- not supported.

### `GeoMap` -- a map

Not supported by Garmin Utils format.

----------
## OziExplorer format
### `GeoWpt` -- a waypoint
 * `x, y, z` -- write support
 * `name` -- write support.
 * `comm` -- write support, cut to 40 symbols.

Other fields:

----------
## GeoJSON format

Mapsoft2 supports reading and writing of GeoJSON files. All data fields
are read and written (should be no data loss except skipping empty
waypoint lists).

Waypoint lists are written in separate FeatureCollections. When reading
a FeatureCollection (including the topmost one) is converted to a waypoint
list only if there is at least one waypoint inside.

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

----------
## Mapsoft XML format




