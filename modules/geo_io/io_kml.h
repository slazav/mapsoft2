#ifndef IO_KML_H
#define IO_KML_H

#include "geo_data/geo_data.h"
#include "opt/opt.h"

/* Read GeoData from a KML file

Options:
   verbose: write file name

*/

void
read_kml (const char* filename, GeoData & data, const Opt & opt = Opt());


/* Write GeoData to a KML file (waypoints, tracks).

Options:
   verbose: write file name
   xml_compr:   compress the output? 0|1, default 0;
   xml_indent:  use indentation? 0|1, default 1;
   xml_ind_str: indentation string, default "  ";
   xml_qchar:   quoting character for attributes, default \'
   gpx_write_rte: write waypoint lists as routes, 0|1, default 0

*/
void
write_kml (const char* filename, const GeoData & data, const Opt & opt = Opt());

#endif
