#include "geo_data.h"

// see https://en.wikipedia.org/wiki/Haversine_formula
double geo_dist_2d(const dPoint &p1, const dPoint &p2){
  double R = 6380e3; // metres
  double cy1 = cos(p1.y * M_PI/180.0);
  double cy2 = cos(p2.y * M_PI/180.0);
  double hdx = (1 - cos((p2.x - p1.x) * M_PI/180.0))/2;
  double hdy = (1 - cos((p2.y - p1.y) * M_PI/180.0))/2;
  return 2*R * asin(sqrt(hdy + cy1*cy2*hdx));
}

dRect
GeoWptList::bbox2d() const {
  dRect ret;
  for (auto i:*this) ret = expand(ret, i);
  return ret;
}

void
GeoWptList::clear_alt() {
  for (auto & i:*this){ i.z = nan(""); }
}

dRect
GeoTrk::bbox2d() const {
  dRect ret;
  for (auto i:*this) ret.expand(i);
  return ret;
}

void
GeoTrk::clear_alt() {
  for (auto & i:*this) i.z = nan("");
}

double
GeoTrk::length() const {
  double ret=0;
  for (int i=0; i<(int)size()-1; i++)
    ret += geo_dist_2d((*this)[i], (*this)[i+1]);
  return ret;
}

GeoTrk::operator dLine() const {
  dLine ret;
  for (auto i:*this) ret.push_back(dPoint(i));
  return ret;
}

GeoMap operator* (const double k, const GeoMap & l) { return l*k; }

GeoMap operator+ (const dPoint & p, const GeoMap & l) { return l+p; }
