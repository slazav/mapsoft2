#ifndef IO_GU_H
#define IO_GU_H

#include "iostream"
#include "geo_data/geo_data.h"
#include "opt/opt.h"

/* Read GeoData from a GarminUtils file

Options:
   verbose -- write file name
   gu_enc -- encoding (default KOI8-R)

*/

void read_gu (const char *fname, GeoData & data, const Opt & opt = Opt());

/* Write data to a KML file (waypoints, tracks).

Options:
   verbose -- write file name
   gu_enc -- encoding (default KOI8-R)

*/

void write_gu (const char *fname, const GeoData & data, const Opt & opt = Opt());

#endif
