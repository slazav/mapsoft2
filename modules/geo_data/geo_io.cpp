#include <iomanip>

#include "tmpdir/tmpdir.h"
#include "filename/filename.h"
#include "geo_io.h"

using namespace std;

void
ms2opt_add_geo_i(ext_option_list & opts){
  int m = MS2OPT_GEO_I;
  ext_option_list add = {
    {"in_fmt", 1, 0, m,  "geodata input format (json, gu, gpx, kml, kmz, ozi, zip)"}
  };
  opts.insert(opts.end(), add.begin(), add.end());
}

void
ms2opt_add_geo_io(ext_option_list & opts){
  int m = MS2OPT_GEO_IO;
  ext_option_list add = {
    {"gu_enc",  1, 0, m, "Garmin Utils format charset (default KOI8-R)"},
    {"ozi_enc", 1, 0, m, "OziExplorer format charset (default Windows-1251)"}
  };
  opts.insert(opts.end(), add.begin(), add.end());
}

void
ms2opt_add_geo_o(ext_option_list & opts){
  int m = MS2OPT_GEO_O;
  ext_option_list add = {
    {"out_fmt",        1, 0, m, "geodata output format (json, gu, gpx, kml, kmz, ozi, zip)"},
    {"xml_compr",      1, 0, m, "compress xml output (GPX and KML formats), default 0"},
    {"xml_indent",     1, 0, m, "use xml indentation (GPX and KML formats), default 1"},
    {"xml_ind_str",    1, 0, m, "xml indentation string (GPX and KML formats), default '  '"},
    {"xml_qchar",      1, 0, m, "xml quoting character (GPX and KML formats), default '\''"},
    {"gpx_write_rte",  1, 0, m, "write waypoint lists as routes (GPX format), default 0"},
    {"json_sort_keys", 1, 0, m, "sort json objects by keys (GeoJSON format), default 1"},
    {"json_compact",   1, 0, m, "write compact json (GeoJSON format), default 0"},
    {"json_indent",    1, 0, m, "use json indentation (GeoJSON format), default 1"},
    {"geo_skip_zt",    1, 0, m, "skip altitude and time information when writing GeoJSON, default 0"},
    {"ozi_map_grid",   1, 0, m, "write grid coordinates in map reference points (OziExplorer format), default 0"},
    {"ozi_map_wgs",    1, 0, m, "always use wgs84 datum for map coordinates (OziExplorer format), default 0"}
  };
  opts.insert(opts.end(), add.begin(), add.end());
}

/**********************************************************/
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

  if (opt.get("in_fmt","") != "") fmt = opt.get("in_fmt", "");

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

  if (opt.get("out_fmt","") != "") fmt = opt.get("out_fmt", "");

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

