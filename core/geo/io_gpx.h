#ifndef IO_GPX_H
#define IO_GPX_H

#include "geo_data.h"
#include "opt/opt.h"

/// Read GeoData from a gpx file
/// Reurn 0 on success, -1 otherwise. No exceptions.
/// if opt.verbose>0 write warnings about file structure
int
read_gpx (const char* filename, GeoData & data, const Opt & opt = Opt());

/// Read data to a gpx file
int
write_gpx (const char* filename, const GeoData & data, const Opt & opt = Opt());

#endif
