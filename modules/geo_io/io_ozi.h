#ifndef IO_OZI_H
#define IO_OZI_H

#include <iostream>

#include "geo_data/geo_data.h"
#include "opt/opt.h"

void read_ozi      (const char *fname, GeoData & data, const Opt & opt = Opt());
void write_ozi_plt (const char *fname, const GeoTrk & trk, const Opt & opt = Opt());
void write_ozi_wpt (const char *fname, const GeoWptList & wpt, const Opt & opt = Opt());
void write_ozi_map (const char *fname, const GeoMap & map, const Opt & opt = Opt());

#endif
