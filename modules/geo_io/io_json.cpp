#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>

#include <jansson.h>

#include "err/err.h"

#include "io_json.h"

using namespace std;

/* see:
 * http://geojson.org/
 * https://tools.ietf.org/html/rfc7946
 * https://leafletjs.com/examples/geojson/
 */

void
write_json (const char* filename, const GeoData & data, const Opt & opts){
  ofstream f(filename);

  if (opts.exists("verbose")) cerr <<
    "Writing data to JS file " << filename << endl;

  if (!f.good()) throw Err() << "Can't open file " << filename << " for writing";

  json_t *features = json_array();

  // tracks
  // Each track is a feature with MultiLineString objects.
  // First we write tracks to have them below points on leaflet maps.
  for (auto i: data.trks) {
    // coordinates
    json_t *j_crd = json_array();
    json_t *j_seg = json_array();
    for (auto tp: i) {
      if (tp.start && json_array_size(j_seg)){
        json_array_append_new(j_crd, j_seg);
        j_seg = json_array();
      }
      json_t *j_pt = json_array();
      json_array_append_new(j_pt, json_real(tp.x));
      json_array_append_new(j_pt, json_real(tp.y));
      json_array_append_new(j_pt, json_integer(tp.t));
      if (tp.have_alt())
        json_array_append_new(j_pt, json_real(tp.z));
      json_array_append_new(j_seg, j_pt);
    }
    if (json_array_size(j_seg)) json_array_append_new(j_crd, j_seg);

    // geometry
    json_t *j_geom = json_object();
    json_object_set_new(j_geom, "type", json_string("MultiLineString"));
    json_object_set_new(j_geom, "coordinates", j_crd);

    // properties
    json_t *j_prop = json_object();
    if (i.name != "") json_object_set_new(j_prop, "name", json_string(i.name.c_str()));
    if (i.comm != "") json_object_set_new(j_prop, "cmt",  json_string(i.comm.c_str()));

    // track
    json_t *j_trk = json_object();
    json_object_set_new(j_trk, "type", json_string("Feature"));
    json_object_set_new(j_trk, "geometry", j_geom);
    if (json_object_size(j_prop)) json_object_set_new(j_trk, "properties", j_prop);
    else json_decref(j_prop);
    json_array_append_new(features, j_trk);
  }

  // waypoints
  for (auto i: data.wpts) {
    json_t *j_wpts = json_array();
    for (auto wp: i ) {

      // coordinate array
      json_t *j_pt = json_array();
      json_array_append_new(j_pt, json_real(wp.x));
      json_array_append_new(j_pt, json_real(wp.y));
      json_array_append_new(j_pt, json_integer(wp.t));
      if (wp.have_alt())
        json_array_append_new(j_pt, json_real(wp.z));

      // geometry
      json_t *j_geom = json_object();
      json_object_set_new(j_geom, "type", json_string("Point"));
      json_object_set_new(j_geom, "coordinates", j_pt);

      // properties
      json_t *j_prop = json_object();
      if (wp.name != "") json_object_set_new(j_prop, "name", json_string(wp.name.c_str()));
      if (wp.comm != "") json_object_set_new(j_prop, "cmt",  json_string(wp.comm.c_str()));

      // waypoint )_
      json_t *j_wpt = json_object();
      json_object_set_new(j_wpt, "type", json_string("Feature"));
      json_object_set_new(j_wpt, "geometry", j_geom);
      if (json_object_size(j_prop)) json_object_set_new(j_wpt, "properties", j_prop);
      else json_decref(j_prop);
      json_array_append_new(j_wpts, j_wpt);
    }

    // properties
    json_t *j_prop = json_object();
    if (i.name != "") json_object_set_new(j_prop, "name", json_string(i.name.c_str()));
    if (i.comm != "") json_object_set_new(j_prop, "cmt",  json_string(i.comm.c_str()));

    json_t *j_wptl = json_object();
    json_object_set_new(j_wptl, "type", json_string("FeatureCollection"));
    if (json_object_size(j_prop)) json_object_set_new(j_wptl, "properties", j_prop);
    else json_decref(j_prop);
    json_object_set_new(j_wptl, "features", j_wpts);
    json_array_append_new(features, j_wptl);
  }

  json_t *J0 = json_object();
  json_object_set_new(J0, "type", json_string("FeatureCollection"));
  json_object_set(J0, "features", features);

  int flags = JSON_REAL_PRECISION(10);
  if (opts.get("json_sort_keys", 1)) flags |= JSON_SORT_KEYS;
  if (opts.get("json_compact", 0)) flags |= JSON_COMPACT;
  if (opts.get("json_indent", 1)) flags |= JSON_INDENT(2);

  char *ret = json_dumps(J0, flags);
  json_decref(J0);
  if (!ret) throw Err() << "Can't write data";
  f<<ret;
  if (!f.good()) throw Err() << "Can't write to file " << filename;
}



void
read_json(const char* filename, GeoData & data, const Opt & opts) {

  throw Err() << "Reading is not supported";
}

