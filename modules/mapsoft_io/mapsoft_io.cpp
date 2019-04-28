#include "mapsoft_io.h"
#include "geo_io/io_gpx.h"
#include "geo_io/io_kml.h"
#include "file_ext/file_ext.h"

void
mapsoft_read(const char *fname, MapsoftData & data, const Opt & opt){
  std::string fmt = opt.get("fmt", std::string());
  if (fmt == "gpx" || (fmt == "" && file_ext(fname, ".gpx")))
    return read_gpx(fname, (GeoData &) data, opt);
  if (fmt == "kml" || (fmt == "" && file_ext(fname, ".kml")))
    return read_kml(fname, (GeoData &) data, opt);
  throw Err() << "Can't determine input format for file: " << fname;
  return;
}

void
mapsoft_write(const char *fname, const MapsoftData & data, const Opt & opt){
  std::string fmt = opt.get("fmt", std::string());
  if (fmt == "gpx" || (fmt == "" && file_ext(fname, ".gpx")))
    return write_gpx(fname, (GeoData &) data, opt);
  if (fmt == "kml" || (fmt == "" && file_ext(fname, ".kml")))
    return write_kml(fname, (GeoData &) data, opt);
  throw Err() << "Can't determine output format for file: " << fname;
  return;
}

