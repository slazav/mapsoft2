#ifndef MAPSOFT_IO
#define MAPSOFT_IO

#include "mapsoft_data/mapsoft_data.h"
#include "opt/opt.h"

void mapsoft_read(const char *fname, MapsoftData & data, const Opt & opt);

void mapsoft_write(const char *fname, const MapsoftData & data, const Opt & opt);

#endif
