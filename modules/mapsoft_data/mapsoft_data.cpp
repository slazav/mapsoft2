#include <iomanip>

#include "mapsoft_data.h"
#include "tmpdir/tmpdir.h"
#include "geo_data/geo_io.h"
#include "filename/filename.h"

using namespace std;

void
mapsoft_read(const string &fname, MapsoftData & data, const Opt & opt){
  string fmt;
  if (file_ext_check(fname, ".zip"))  fmt="zip";
  if (opt.exists("fmt")) fmt = opt.get("fmt", "");

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

  read_geo(fname, data, opt);
}

void
mapsoft_write(const string & fname, const MapsoftData & data, const Opt & opt){
  write_geo(fname, data, opt);
}

