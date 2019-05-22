#ifndef IO_GPX_H
#define IO_GPX_H

#include "geo_data/geo_data.h"
#include "opt/opt.h"

/* Read GeoData from a gpx file

 - Options:
   verbose -- write warnings about file structure

*/

void
read_gpx (const char* filename, GeoData & data, const Opt & opt = Opt());

/*  Write data to a gpx file (waypoints, tracks).

 - Waypoint lists are mixed together or written as 
   separate routes (controlled by gpx_write_rte option).

 - Options:
   xml_compr:   compress the output? 0|1, default 0;
   xml_indent:  use indentation? 0|1, default 1;
   xml_ind_str: indentation string, default "  ";
   xml_qchar:   quoting character for attributes, default \'
   gpx_write_rte: write waypoint lists as routes, 0|1, default 0

*/

void
write_gpx (const char* filename, const GeoData & data, const Opt & opt = Opt());

#endif
