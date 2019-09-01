#ifndef GEO_IO_H
#define GEO_IO_H

#include "opt/opt.h"
#include "geo_data.h"

/*
Input options:
  verbose        write some information (default: 0)
  fmt            input format (json, gu, gpx, kml, kmz, ozi), default ""
  gu_enc         GarminUtils format: encoding (default KOI8-R)
  ozi_enc        OziExplorer format: encoding (default: Windows-1251)

Output options:
  verbose        write some information (default: 0)
  fmt            output format (json, gu, gpx, kml, kmz, ozi), default ""
  xml_compr      GPX, KML formats: compress the output? 0|1, default 0;
  xml_indent     GPX, KML formats: use indentation? 0|1, default 1;
  xml_ind_str    GPX, KML formats: indentation string, default "  ";
  xml_qchar      GPX, KML formats: quoting character for attributes, default \'
  gpx_write_rte  GPX format: write waypoint lists as routes, 0|1, default 0
  gu_enc         GarminUtils format: encoding (default KOI8-R)
  json_sort_keys (default 1) GeoJSON format: sort json objects by keys
  json_compact   (default 1) GeoJSON format: write compact json
  json_indent    (default 0) GeoJSON format: use json indentation
  geo_skip_zt    (default 0) GeoJSON format: skip altitude and time information
  ozi_enc        OziExplorer format: encoding (default: Windows-1251)
  ozi_map_grid   OziExplorer format: write grid coordinates in reference points (default 0)
  ozi_map_wgs    OziExplorer format: always use wgs84 datum for map coordinates (default 0)
*/

// Detect file format (by file extension and --fmt option) and read/write data.
// Return false if format is unknown.
bool read_geo (const char* filename, GeoData & data, const Opt & opt = Opt());
bool write_geo (const char* filename, const GeoData & data, const Opt & opt = Opt());


// GPX format
void read_gpx (const char* filename, GeoData & data, const Opt & opt = Opt());
void write_gpx (const char* filename, const GeoData & data, const Opt & opt = Opt());

// GarminUtils format

void read_gu (const char *fname, GeoData & data, const Opt & opt = Opt());
void write_gu (const char *fname, const GeoData & data, const Opt & opt = Opt());

// GeoJSON format
void read_json  (const char* filename, GeoData & data, const Opt & opt = Opt());
void write_json (const char* filename, const GeoData & data, const Opt & opt = Opt());

// KML format
void read_kml (const char* filename, GeoData & data, const Opt & opt = Opt());
void write_kml (const char* filename, const GeoData & data, const Opt & opt = Opt());

// OziExplorer formats
void read_ozi      (const char *fname, GeoData & data, const Opt & opt = Opt());
void write_ozi_plt (const char *fname, const GeoTrk & trk, const Opt & opt = Opt());
void write_ozi_wpt (const char *fname, const GeoWptList & wpt, const Opt & opt = Opt());
void write_ozi_map (const char *fname, const GeoMap & map, const Opt & opt = Opt());

#endif
