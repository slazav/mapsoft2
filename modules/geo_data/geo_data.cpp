#include "geo_data.h"
#include "geo_utils.h"

dRect
GeoWptList::bbox() const {
  dRect ret;
  for (auto i:*this) ret = expand(ret, i);
  return ret;
}

void
GeoWptList::clear_alt() {
  for (auto & i:*this){ i.z = nan(""); }
}

dRect
GeoTrk::bbox() const {
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

