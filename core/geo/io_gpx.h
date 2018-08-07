#ifndef IO_GPX_H
#define IO_GPX_H

#include "geo_data.h"
#include "opt/opt.h"

/// Read GeoData from a gpx file
/// Reurn 0 on success, -1 otherwise
/// if opt.verbose>0 write warnings about file structure
int
read_gpx (const char* filename, GeoData & data, const Opt & opt = Opt());

/// Write data to a gpx file (waypoints, tracks).
/// - All waypoint lists are mixed together.
/// - Throw Err() if file can not be written.
/// - Options:
///   xml_compr:   compress the output? 0|1, default 0;
///   xml_indent:  use indentation? 0|1, default 1;
///   xml_ind_str: indentation string, default "  ";
///   xml_qchar:   quoting character for attributes, default \'
void
write_gpx (const char* filename, const GeoData & data, const Opt & opt = Opt());

#endif
