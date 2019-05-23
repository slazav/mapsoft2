#include <iomanip>

#include "mapsoft_io.h"
#include "tmpdir/tmpdir.h"
#include "geo_data/geo_io.h"
#include "file_ext/file_ext.h"

using namespace std;

void
mapsoft_read(const char *fname, MapsoftData & data, const Opt & opt){
  string fmt;
  if      (file_ext_check(fname, ".json")) fmt="json";
  else if (file_ext_check(fname, ".gu"))   fmt="gu";
  else if (file_ext_check(fname, ".gpx"))  fmt="gpx";
  else if (file_ext_check(fname, ".kml"))  fmt="kml";
  else if (file_ext_check(fname, ".kmz"))  fmt="kmz";
  else if (file_ext_check(fname, ".wpt"))  fmt="ozi";
  else if (file_ext_check(fname, ".plt"))  fmt="ozi";
  else if (file_ext_check(fname, ".map"))  fmt="ozi";
  else if (file_ext_check(fname, ".zip"))  fmt="zip";

  if (opt.exists("fmt")) fmt = opt.get("fmt", string());

  // JSON format
  if (fmt == "json") return read_json(fname, (GeoData &) data, opt);

  // Garmin Utils format
  if (fmt == "gu")   return read_gu(fname, (GeoData &) data, opt);

  // GPX format
  if (fmt == "gpx")  return read_gpx(fname, (GeoData &) data, opt);

  // KML format
  if (fmt == "kml")  return read_kml(fname, (GeoData &) data, opt);

  // KMZ format
  if (fmt == "kmz") {
    TmpDir tmpdir("mapsoft_read_kmz_XXXXXX");
    tmpdir.unzip(fname);
    vector<string> paths = tmpdir.get_paths();
    for (int i=0; i<paths.size(); i++){
      if (*paths[i].rbegin() == '/') continue;
      read_kml(paths[i].c_str(), (GeoData &) data, opt);
    }
    return;
  }

  // ZIP format
  if (fmt == "zip") {
    TmpDir tmpdir("mapsoft_read_zip_XXXXXX");
    tmpdir.unzip(fname);
    vector<string> paths = tmpdir.get_paths();
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
  string fmt;
  if      (file_ext_check(fname, ".json")) fmt="json";
  else if (file_ext_check(fname, ".gu"))   fmt="gu";
  else if (file_ext_check(fname, ".gpx"))  fmt="gpx";
  else if (file_ext_check(fname, ".kml"))  fmt="kml";
  else if (file_ext_check(fname, ".kmz"))  fmt="kmz";
  else if (file_ext_check(fname, ".wpt"))  fmt="ozi";
  else if (file_ext_check(fname, ".plt"))  fmt="ozi";
  else if (file_ext_check(fname, ".map"))  fmt="ozi";

  if (opt.exists("fmt")) fmt = opt.get("fmt", string());

  // JSON format
  if (fmt == "json") return write_json(fname, (GeoData &) data, opt);

  // Garmin Utils format
  if (fmt == "gu")   return write_gu(fname, (GeoData &) data, opt);

  // GPX format
  if (fmt == "gpx")  return write_gpx(fname, (GeoData &) data, opt);

  // KML format
  if (fmt == "kml")  return write_kml(fname, (GeoData &) data, opt);

  // KMZ format
  if (fmt == "kmz") {
    TmpDir tmpdir("mapsoft_write_kmz_XXXXXX");
    string fpath = tmpdir.add(file_ext_repl(fname, ".kml").c_str());
    write_kml(fpath.c_str(), data, opt);
    tmpdir.zip(fname);
    return;
  }

  // OZI format (multiple files)
  if (fmt == "ozi") {

    string base = file_ext_repl(fname, "");
    vector<string> files;

    // number of files with tracks, waypoints and maps
    int wn = data.wpts.size();
    int tn = data.trks.size();
    int mn = data.maps.size();

    int ww=0, tw=0, mw=0;
    if (wn > 1) ww = (int) floor (log (1.0 * wn) / log (10.0)) + 1;
    if (tn > 1) tw = (int) floor (log (1.0 * tn) / log (10.0)) + 1;
    if (mn > 1) mw = (int) floor (log (1.0 * mn) / log (10.0)) + 1;

    for (size_t n = 0; n < wn; ++n){
      ostringstream oef;
      if (ww>0)
        oef << base << setw(ww) << setfill('0') << n+1 << ".wpt";
      else
        oef << base << ".wpt";
      write_ozi_wpt(oef.str().c_str(), data.wpts[n], opt);
      files.push_back(oef.str());
    }

    for (size_t n = 0; n < tn; ++n){
      ostringstream oef;
      if (tw > 0)
        oef << base << setw(tw) << setfill('0') << n+1 << ".plt";
      else
        oef << base << ".plt";

      write_ozi_plt (oef.str().c_str(), data.trks[n], opt);
      files.push_back(oef.str());
    }

    for (size_t n = 0; n < mn; ++n){
      int mmn = data.maps[n].size(); // map count in this maplist
      int mmw=0;
      if (mmn > 1) mmw = (int) floor (log (1.0 * mmn) / log (10.0)) + 1;

      for (size_t nn = 0; nn != mmn; ++nn) {
        ostringstream oef;
        oef << base;
        if (mw > 0)  oef << setw(mw) << setfill('0') << n+1;
        if ((mmw > 0) && (mw > 0))  oef << "_";
        if (mmw > 0)  oef << setw(mmw) << setfill('0') << nn+1;
        oef << ".map";

        write_ozi_map(oef.str().c_str(), data.maps[n][nn], opt);
        files.push_back(oef.str());
      }
    }
  }

  throw Err() << "Can't determine output format for file: " << fname;
  return;
}

