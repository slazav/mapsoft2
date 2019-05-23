#ifndef IO_JSON_H
#define IO_JSON_H

#include "geo_data/geo_data.h"
#include "opt/opt.h"


/*
Options:
 - verbose (default 0)
*/

void read_json  (const char* filename, GeoData & data, const Opt & opt = Opt());

/*
Options:
 - verbose (default 0)
 - json_sort_keys (default 1)
 - json_compact (default 1)
 - json_indent (default 0)
 - json_skip_zt (default 0)
*/
void write_json (const char* filename, const GeoData & data, const Opt & opt = Opt());

#endif
