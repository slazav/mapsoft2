#ifndef MAPSOFT_DATA
#define MAPSOFT_DATA

#include "geo_data/geo_data.h"
#include "opt/opt.h"

/* Structure for all data and state information of mapsoft program */
struct MapsoftData : public GeoData {
};

void mapsoft_read(const char *fname, MapsoftData & data, const Opt & opt);

void mapsoft_write(const char *fname, const MapsoftData & data, const Opt & opt);

#endif
