#include "mapsoft_io.h"
#include "tmpdir/tmpdir.h"
#include "geo_io/io_gpx.h"
#include "geo_io/io_kml.h"
#include "geo_io/io_gu.h"
#include "geo_io/io_json.h"
#include "geo_io/io_ozi.h"
#include "file_ext/file_ext.h"

void
mapsoft_read(const char *fname, MapsoftData & data, const Opt & opt){
  std::string fmt = opt.get("fmt", std::string());

  // JSON format
  if (fmt == "json" || (fmt == "" && file_ext_check(fname, ".json")))
    return read_json(fname, (GeoData &) data, opt);

  // Garmin Utils format
  if (fmt == "gu" || (fmt == "" && file_ext_check(fname, ".gu")))
    return read_gu(fname, (GeoData &) data, opt);

  // GPX format
  if (fmt == "gpx" || (fmt == "" && file_ext_check(fname, ".gpx")))
    return read_gpx(fname, (GeoData &) data, opt);

  // KML format
  if (fmt == "kml" || (fmt == "" && file_ext_check(fname, ".kml")))
    return read_kml(fname, (GeoData &) data, opt);

  // KMZ format
  if (fmt == "kmz" || (fmt == "" && file_ext_check(fname, ".kmz"))) {
    TmpDir tmpdir("mapsoft_read_kmz_XXXXXX");
    tmpdir.unzip(fname);
    std::vector<std::string> paths = tmpdir.get_paths();
    for (int i=0; i<paths.size(); i++){
      if (*paths[i].rbegin() == '/') continue;
      read_kml(paths[i].c_str(), (GeoData &) data, opt);
    }
    return;
  }

  // ZIP format
  if (fmt == "zip" || (fmt == "" && file_ext_check(fname, ".zip"))) {
    TmpDir tmpdir("mapsoft_read_zip_XXXXXX");
    tmpdir.unzip(fname);
    std::vector<std::string> paths = tmpdir.get_paths();
    for (int i=0; i<paths.size(); i++){
      if (*paths[i].rbegin() == '/') continue;
      mapsoft_read(paths[i].c_str(), data, opt);
    }
    return;
  }

  throw Err() << "Can't determine input format for file: " << fname;
  return;
}

void
mapsoft_write(const char *fname, const MapsoftData & data, const Opt & opt){
  std::string fmt = opt.get("fmt", std::string());

  // JSON format
  if (fmt == "json" || (fmt == "" && file_ext_check(fname, ".json")))
    return write_json(fname, (GeoData &) data, opt);

  // Garmin Utils format
  if (fmt == "gu" || (fmt == "" && file_ext_check(fname, ".gu")))
    return write_gu(fname, (GeoData &) data, opt);

  // GPX format
  if (fmt == "gpx" || (fmt == "" && file_ext_check(fname, ".gpx")))
    return write_gpx(fname, (GeoData &) data, opt);

  // KML format
  if (fmt == "kml" || (fmt == "" && file_ext_check(fname, ".kml")))
    return write_kml(fname, (GeoData &) data, opt);

  // KMZ format
  if (fmt == "kmz" || (fmt == "" && file_ext_check(fname, ".kmz"))) {
    TmpDir tmpdir("mapsoft_write_kmz_XXXXXX");
    std::string fpath = tmpdir.add(file_ext_repl(fname, ".kml").c_str());
    write_kml(fpath.c_str(), data, opt);
    tmpdir.zip(fname);
    return;
  }


  throw Err() << "Can't determine output format for file: " << fname;
  return;
}

