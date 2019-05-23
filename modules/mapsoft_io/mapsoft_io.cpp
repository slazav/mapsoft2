#include <iomanip>

#include "mapsoft_io.h"
#include "tmpdir/tmpdir.h"
#include "geo_data/geo_io.h"
#include "file_ext/file_ext.h"

using namespace std;

void
mapsoft_read(const char *fname, MapsoftData & data, const Opt & opt){
  string fmt;
  if (file_ext_check(fname, ".zip"))  fmt="zip";
  if (opt.exists("fmt")) fmt = opt.get("fmt", string());

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

  if (read_geo(fname, data, opt)) return;

  throw Err() << "Can't determine input format for file: " << fname;
  return;
}

void
mapsoft_write(const char *fname, const MapsoftData & data, const Opt & opt){

  if (write_geo(fname, data, opt)) return;

  throw Err() << "Can't determine output format for file: " << fname;
  return;
}

