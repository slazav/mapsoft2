#ifndef GEO_IO_H
#define GEO_IO_H

#include <string>
#include "getopt/getopt.h"
#include "opt/opt.h"
#include "geo_data.h"


/********************************************************************/
#include "getopt/getopt.h"

// add MS2OPT_GEO_I options
void ms2opt_add_geo_i(ext_option_list & opts);

// add MS2OPT_GEO_O options
void ms2opt_add_geo_o(ext_option_list & opts);

// add MS2OPT_GEO_IO options
void ms2opt_add_geo_io(ext_option_list & opts);

/********************************************************************/

// append option information to ext_option_list
void geo_io_ms2opts(ext_option_list & list);

// Detect file format (by file extension and --fmt option) and read/write data.
// Return false if format is unknown.
bool read_geo (const std::string &filename, GeoData & data, const Opt & opt = Opt());
bool write_geo (const std::string &filename, const GeoData & data, const Opt & opt = Opt());


// GPX format
void read_gpx (const std::string &filename, GeoData & data, const Opt & opt = Opt());
void write_gpx (const std::string &filename, const GeoData & data, const Opt & opt = Opt());

// GarminUtils format

void read_gu (const std::string &fname, GeoData & data, const Opt & opt = Opt());
void write_gu (const std::string &fname, const GeoData & data, const Opt & opt = Opt());

// GeoJSON format
void read_json  (const std::string &filename, GeoData & data, const Opt & opt = Opt());
void write_json (const std::string &filename, const GeoData & data, const Opt & opt = Opt());

// KML format
void read_kml (const std::string &filename, GeoData & data, const Opt & opt = Opt());
void write_kml (const std::string &filename, const GeoData & data, const Opt & opt = Opt());

// OziExplorer formats
void read_ozi      (const std::string &fname, GeoData & data, const Opt & opt = Opt());
void write_ozi_plt (const std::string &fname, const GeoTrk & trk, const Opt & opt = Opt());
void write_ozi_wpt (const std::string &fname, const GeoWptList & wpt, const Opt & opt = Opt());
void write_ozi_map (const std::string &fname, const GeoMap & map, const Opt & opt = Opt());

#endif
