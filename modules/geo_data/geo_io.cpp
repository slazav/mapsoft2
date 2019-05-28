#include <iomanip>

#include "tmpdir/tmpdir.h"
#include "filename/filename.h"
#include "geo_io.h"

using namespace std;

bool
read_geo (const char* fname, GeoData & data, const Opt & opt){
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
  if (fmt == "json"){
    read_json(fname, (GeoData &) data, opt);
    return true;
  }

  // Garmin Utils format
  if (fmt == "gu"){
    read_gu(fname, (GeoData &) data, opt);
    return true;
  }

  // GPX format
  if (fmt == "gpx"){
    read_gpx(fname, (GeoData &) data, opt);
    return true;
  }

  // KML format
  if (fmt == "kml"){
    read_kml(fname, (GeoData &) data, opt);
    return true;
  }

  // KMZ format
  if (fmt == "kmz") {
    TmpDir tmpdir("mapsoft_read_kmz_XXXXXX");
    tmpdir.unzip(fname);
    vector<string> paths = tmpdir.get_paths();
    for (int i=0; i<paths.size(); i++){
      if (*paths[i].rbegin() == '/') continue;
      read_kml(paths[i].c_str(), (GeoData &) data, opt);
    }
    return true;
  }

  // OxiExplorer format
  if (fmt == "ozi"){
    read_ozi(fname, (GeoData &) data, opt);
    return true;
  }

  // ZIP format
  if (fmt == "zip") {
    TmpDir tmpdir("mapsoft_read_zip_XXXXXX");
    tmpdir.unzip(fname);
    vector<string> paths = tmpdir.get_paths();
    for (auto p:paths){
      if (*p.rbegin() == '/') continue;
      read_geo(p.c_str(), data, opt);
    }
    return true;
  }

  return false;
}

bool
write_geo (const char* fname, const GeoData & data, const Opt & opt){

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
  if (fmt == "json"){
    write_json(fname, (GeoData &) data, opt);
    return true;
  }

  // Garmin Utils format
  if (fmt == "gu"){
    write_gu(fname, (GeoData &) data, opt);
    return true;
  }

  // GPX format
  if (fmt == "gpx"){
    write_gpx(fname, (GeoData &) data, opt);
    return true;
  }

  // KML format
  if (fmt == "kml"){
    write_kml(fname, (GeoData &) data, opt);
    return true;
  }

  // KMZ format
  if (fmt == "kmz") {
    TmpDir tmpdir("mapsoft_write_kmz_XXXXXX");
    string fpath = tmpdir.add(file_ext_repl(fname, ".kml").c_str());
    write_kml(fpath.c_str(), data, opt);
    tmpdir.zip(fname);
    return true;
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

    int n=0;
    for (auto wpl:data.wpts){
      n++;
      ostringstream f;
      if (ww>0) f << base << setw(ww) << setfill('0') << n << ".wpt";
      else      f << base << ".wpt";
      write_ozi_wpt(f.str().c_str(), wpl, opt);
      files.push_back(f.str());
    }

    n=0;
    for (auto trk:data.trks){
      n++;
      ostringstream f;
      if (tw>0) f << base << setw(tw) << setfill('0') << n << ".plt";
      else      f << base << ".plt";

      write_ozi_plt (f.str().c_str(), trk, opt);
      files.push_back(f.str());
    }

    n=0;
    for (auto mapl:data.maps){
      n++;
      int mmn = mapl.size(); // map count in this maplist
      int mmw=0;
      if (mmn > 1) mmw = (int) floor (log (1.0 * mmn) / log (10.0)) + 1;

      int nn=0;
      for (auto map:mapl) {
        nn++;
        ostringstream f;
        f << base;
        if (mw > 0)  f << setw(mw) << setfill('0') << n;
        if ((mmw > 0) && (mw > 0))  f << "_";
        if (mmw > 0)  f << setw(mmw) << setfill('0') << nn;
        f << ".map";

        write_ozi_map(f.str().c_str(), map, opt);
        files.push_back(f.str());
      }
    }
    return true;
  }

  return false;
}

