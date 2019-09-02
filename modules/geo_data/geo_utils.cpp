#include <cmath>
#include "err/err.h"
#include "opt/opt.h"
#include "geo_utils.h"

using namespace std;

int
lon2lon0(const double lon){
  double lon0 =floor( lon/6.0 ) * 6 + 3;
  while (lon0>180)  lon0-=360;
  while (lon0<-180) lon0+=360;
  return lon0;
}

int
lon2pref(const double lon){
  double lon0 = lon2lon0(lon);
  return (lon0<0 ? 60:0) + (lon0-3)/6 + 1;
}

int
crdx2lon0(const double X){
  int pref= floor(X/1e6);
  if (pref==0) throw Err() << "zero coordinate prefix";
  return (pref-(pref>30 ? 60:0))*6-3;
}

double
crdx2nonpref(const double X){
  return X - floor( X/1e6 ) * 1e6;
}

string
GEO_PROJ_SU(double lon){
  int lon0 = lon2lon0(lon);
  int pref = (lon0<0 ? 60:0) + (lon0-3)/6 + 1;
  return "+ellps=krass +towgs84=+28,-130,-95 +proj=tmerc"
         " +lon_0=" + type_to_str(lon0) +
         " +x_0=" + type_to_str(pref) + "500000";
}
