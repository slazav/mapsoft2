#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>

#include <jansson.h>

#include "opt/opt.h"
#include "err/err.h"

#include "geo_data.h"


using namespace std;

/* see:
 * http://geojson.org/
 * https://tools.ietf.org/html/rfc7946
 * https://leafletjs.com/examples/geojson/
 */

void
write_json (const string &fname, const GeoData & data, const Opt & opts){

  bool v = opts.get("verbose", false);
  if (v) cerr << "Writing GeoJSON file: " << fname << endl;

  json_t *features = json_array();
  bool skip_zt = opts.get("geo_skip_zt", false);

  // tracks
  // Each track is a feature with MultiLineString objects.
  // First we write tracks to have them below points on leaflet maps.
  for (auto const & trk: data.trks) {
    if (v) cerr << "  Writing track: " << trk.name
           << " (" << trk.size() << " points)" << endl;

    // coordinates
    json_t *j_crd = json_array();
    json_t *j_seg = json_array();
    for (auto const & tp: trk) {
      if (tp.start && json_array_size(j_seg)){
        json_array_append_new(j_crd, j_seg);
        j_seg = json_array();
      }
      json_t *j_pt = json_array();
      json_array_append_new(j_pt, json_real(tp.x));
      json_array_append_new(j_pt, json_real(tp.y));
      if (!skip_zt && (tp.t!=0 || tp.have_alt())) {
        json_array_append_new(j_pt, tp.have_alt()? json_real(tp.z) : json_null());
        if (tp.t!=0) json_array_append_new(j_pt, json_integer(tp.t));
      }
      json_array_append_new(j_seg, j_pt);
    }
    if (json_array_size(j_seg)) json_array_append_new(j_crd, j_seg);

    // geometry
    json_t *j_geom = json_object();
    json_object_set_new(j_geom, "type", json_string("MultiLineString"));
    json_object_set_new(j_geom, "coordinates", j_crd);

    // properties
    json_t *j_prop = json_object();
    if (trk.name != "") json_object_set_new(
                          j_prop, "name", json_string(trk.name.c_str()));
    if (trk.comm != "") json_object_set_new(
                          j_prop, "cmt",  json_string(trk.comm.c_str()));
    for (auto const & o:trk.opts)
      json_object_set_new(j_prop, o.first.c_str(), json_string(o.second.c_str()));

    // track
    json_t *j_trk = json_object();
    json_object_set_new(j_trk, "type", json_string("Feature"));
    json_object_set_new(j_trk, "geometry", j_geom);
    if (json_object_size(j_prop)) json_object_set_new(j_trk, "properties", j_prop);
    else json_decref(j_prop);
    json_array_append_new(features, j_trk);
  }

  // waypoints
  for (auto const & wpl: data.wpts) {
    if (v) cerr << "  Writing waypoints: " << wpl.name
           << " (" << wpl.size() << " points)" << endl;

    json_t *j_wpts = json_array();
    for (auto const & wp: wpl ) {

      // coordinate array
      json_t *j_pt = json_array();
      json_array_append_new(j_pt, json_real(wp.x));
      json_array_append_new(j_pt, json_real(wp.y));
      if (!skip_zt && (wp.t!=0 || wp.have_alt())) {
        json_array_append_new(j_pt, wp.have_alt()? json_real(wp.z) : json_null());
        if (wp.t!=0) json_array_append_new(j_pt, json_integer(wp.t));
      }

      // geometry
      json_t *j_geom = json_object();
      json_object_set_new(j_geom, "type", json_string("Point"));
      json_object_set_new(j_geom, "coordinates", j_pt);

      // properties
      json_t *j_prop = json_object();
      if (wp.name != "") json_object_set_new(j_prop, "name", json_string(wp.name.c_str()));
      if (wp.comm != "") json_object_set_new(j_prop, "cmt",  json_string(wp.comm.c_str()));
      for (auto o:wp.opts)
        json_object_set_new(j_prop, o.first.c_str(), json_string(o.second.c_str()));

      // waypoint
      json_t *j_wpt = json_object();
      json_object_set_new(j_wpt, "type", json_string("Feature"));
      json_object_set_new(j_wpt, "geometry", j_geom);
      if (json_object_size(j_prop)) json_object_set_new(j_wpt, "properties", j_prop);
      else json_decref(j_prop);
      json_array_append_new(j_wpts, j_wpt);
    }

    // properties
    json_t *j_prop = json_object();
    if (wpl.name != "") json_object_set_new(
                          j_prop, "name", json_string(wpl.name.c_str()));
    if (wpl.comm != "") json_object_set_new(
                          j_prop, "cmt", json_string(wpl.comm.c_str()));
    for (auto const & o:wpl.opts)
      json_object_set_new(j_prop, o.first.c_str(), json_string(o.second.c_str()));

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
  if (opts.get("json_compact", 1)) flags |= JSON_COMPACT;
  if (opts.get("json_indent", 0)) flags |= JSON_INDENT(2);

  char *ret = json_dumps(J0, flags);
  json_decref(J0);
  if (!ret) throw Err() << "Can't write data";

  ofstream f(fname);
  if (!f.good()) throw Err() << "Can't open file " << fname << " for writing";
  f<<ret;
  if (!f.good()) throw Err() << "Can't write to file " << fname;
}


/**************************************************************************/
// read a single point (x,y,t,z) from a JSON array
template <typename T>
void read_geojson_pt(json_t *coord, T & pt){
  json_t *val;
  if (json_array_size(coord)>0 &&
     (val = json_array_get(coord, 0))){
    if (json_is_number(val)) pt.x = json_real_value(val);
    else if (!json_is_null(val))
      throw Err() << "number expected in GeoJSON coordinates";
  }
  if (json_array_size(coord)>1 &&
     (val = json_array_get(coord, 1))){
    if (json_is_number(val)) pt.y = json_real_value(val);
    else if (!json_is_null(val))
      throw Err() << "number expected in GeoJSON coordinates";
  }
  if (json_array_size(coord)>2 &&
     (val = json_array_get(coord, 2))){
    if (json_is_number(val)) pt.z = json_real_value(val);
    else if (!json_is_null(val))
      throw Err() << "number expected in GeoJSON coordinates";
  }
  if (json_array_size(coord)>3 &&
     (val = json_array_get(coord, 3))){
    if (json_is_number(val)) pt.t = json_integer_value(val);
    else if (!json_is_null(val))
      throw Err() << "number expected in GeoJSON coordinates";
  }
}

// construct a waypoint from GeoJSON coordinates and properties
GeoWpt read_geojson_wpt(json_t *coord, json_t *prop){
  GeoWpt ret;
  // set properties
  const char *key;
  json_t *val;
  json_object_foreach(prop, key, val) {
    if (!json_is_string(val)) continue;
    else if (strcasecmp(key, "name")==0) ret.name = json_string_value(val);
    else if (strcasecmp(key, "cmt")==0) ret.comm = json_string_value(val);
    else ret.opts.put(key, string(json_string_value(val)));
  }
  // set coordinates
  read_geojson_pt(coord, ret);
  return ret;
}

// construct a track from GeoJSON coordinates and properties
GeoTrk read_geojson_trk(json_t *coord, json_t *prop, const bool multi){
  GeoTrk ret;
  // set properties
  const char *key;
  json_t *val;
  json_object_foreach(prop, key, val) {
    if (!json_is_string(val)) continue;
    else if (strcasecmp(key, "name")==0) ret.name = json_string_value(val);
    else if (strcasecmp(key, "cmt")==0) ret.comm = json_string_value(val);
    else ret.opts.put(key, string(json_string_value(val)));
  }
  // set coordinates
  size_t i;
  json_t *c1;
  json_array_foreach(coord, i, c1) {
    if (multi){
      size_t j;
      json_t *c2;
      json_array_foreach(c1, j, c2) {
        GeoTpt pt;
        if (j==0) pt.start=1;
        read_geojson_pt(c2, pt);
        ret.push_back(pt);
      }
    }
    else {
      GeoTpt pt;
      if (i==0) pt.start=1;
      read_geojson_pt(c1, pt);
      ret.push_back(pt);
    }
  }
  return ret;
}

// construct a waypoint list from GeoJSON properties
GeoWptList read_geojson_wptl(json_t *prop){
  GeoWptList ret;
  // set properties
  const char *key;
  json_t *val;
  json_object_foreach(prop, key, val) {
    if (!json_is_string(val)) continue;
    else if (strcasecmp(key, "name")==0) ret.name = json_string_value(val);
    else if (strcasecmp(key, "cmt")==0) ret.comm = json_string_value(val);
    else ret.opts.put(key, string(json_string_value(val)));
  }
  return ret;
}


// read GeoJSON Feature of FeatureCollection (recursively)
void
read_geojson_feature(json_t *feature, GeoData & data,
                     GeoWptList & wptl, const bool v){
    if (!json_is_object(feature))
      throw Err() << "JSON object expected";
    json_t *j_type = json_object_get(feature, "type");
    if (!json_is_string(j_type))
      throw Err() << "Wrong/missing type in a GeoJSON object";
    string type = json_string_value(j_type);

    if (type == "FeatureCollection"){
      json_t *sub_features = json_object_get(feature, "features");
      if (!json_is_array(sub_features))
        throw Err() << "features array expected in a FeatureCollection";

      // always construct a new waypoint list for a FeatureCollection
      json_t * j_prop = json_object_get(feature, "properties"); // maybe NULL
      GeoWptList wptl1 = read_geojson_wptl(j_prop);

      // read sub-features
      size_t i;
      json_t *sub_feature;
      json_array_foreach(sub_features, i, sub_feature) {
        read_geojson_feature(sub_feature, data, wptl1, v);
      }

      // add waypoint list if it is not empty
      if (wptl1.size()){
        if (v) cerr << "  Reading waypoints: " << wptl1.name
                    << " (" << wptl1.size() << " points)" << endl;
        data.wpts.push_back(wptl1);
      }
      return;
    }

    else if (type == "Feature"){
      // geometry
      json_t * j_geom = json_object_get(feature, "geometry");
      if (!json_is_object(j_geom))
        throw Err() << "Wrong/missing geometry object in a GeoJSON Feature";

      json_t * j_geom_type = json_object_get(j_geom, "type");
      if (!json_is_string(j_geom_type))
        throw Err() << "Wrong/missing type in a GeoJSON geometry";
      string geom_type = json_string_value(j_geom_type);

      json_t * j_geom_coord = json_object_get(j_geom, "coordinates");
      if (!json_is_array(j_geom_coord))
        throw Err() << "Wrong/missing coordinades in a GeoJSON geometry";

      json_t * j_prop = json_object_get(feature, "properties"); // maybe NULL

      // Waypoint
      if (geom_type == "Point") {
        wptl.push_back(read_geojson_wpt(j_geom_coord, j_prop));
      }
      // Track
      else if (geom_type == "MultiLineString" || geom_type == "LineString") {
        GeoTrk trk = read_geojson_trk(
            j_geom_coord, j_prop, geom_type == "MultiLineString");
        if (v) cerr << "  Reading track: " << trk.name
                    << " (" << trk.size() << " points)" << endl;
        data.trks.push_back(trk);
      }
      else
        throw Err() << "Unknown type in a GeoJSON feature: " << type;
    }
    else throw Err() << "Unknown type in a GeoJSON feature: " << type;
}

void
read_json(const string &fname, GeoData & data, const Opt & opts) {
  bool v = opts.get("verbose", false);
  if (v) cerr << "Reading GeoJSON file: " << fname << endl;

  ifstream f(fname);
  istreambuf_iterator<char> begin(f), end;
  string buf(begin, end);
  if (!f.good()) throw Err() << "Can't read file " << fname << " for writing";

  size_t flags = 0;
  json_error_t e;
  json_t *J = json_loads(buf.c_str(), flags, &e);
  if (!J) throw Err() << "Can't parse JSON: " << e.text;

  GeoWptList tmp;
  try { read_geojson_feature(J, data, tmp, v); }
  catch(Err e){
    json_decref(J);
    throw e;
  }
  json_decref(J);
}

