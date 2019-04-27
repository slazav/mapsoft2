#ifndef IO_KML_H
#define IO_KML_H

#include "geo_data/geo_data.h"
#include "opt/opt.h"

void
read_kml (const char* filename, GeoData & data, const Opt & opt = Opt());

void
write_kml (const char* filename, const GeoData & data, const Opt & opt = Opt());

#endif
