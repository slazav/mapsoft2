#include <proj_api.h>

#include "err/err.h"
#include "conv_geo.h"
#include "geo_utils.h"

std::string expand_proj_aliases(const std::string & pars){

  // a few predefined projections
  if (pars == "WGS") // default projection: lon-lat in WGS84 datum
    return "+datum=WGS84 +proj=lonlat";

  if (pars == "WEB") // web mercator
    return "+proj=webmerc +datum=WGS84";

  if (pars == "FI" || pars == "KKJ") // Finnish maps, KKJ (EPSG:2393?)
    return "+proj=tmerc +lon_0=27 +x_0=3500000 +ellps=intl"
      " +towgs84=-90.7,-106.1,-119.2,4.09,0.218,-1.05,1.37";

  if (pars == "ETRS-TM35FIN" || pars == "ETRS89") // Finnish maps (EPSG:3067?)
    return "+proj=utm +zone=35 +ellps=GRS80 +units=m +no_defs";

  if (pars == "CH") // Swiss maps
    return "+proj=somerc +lat_0=46.95240555555556"\
      " +lon_0=7.439583333333333 +x_0=600000 +y_0=200000"\
      " +ellps=bessel +towgs84=674.374,15.056,405.346,0,0,0,0"\
      " +units=m +no_defs";

  if (pars == "SU_LL") // Soviet datum (lon-lat)
    return "+ellps=krass +towgs84=+28,-130,-95 +proj=lonlat";

  // SU<N> -- Soviet coordinate system with central meridian N
  if (pars.length()>2 &&
      pars.substr(0,2) == "SU"){
    int lon = str_to_type<int>(pars.substr(2,pars.length()-2));
    int lon0 = lon2lon0(lon);
    int pref = (lon0<0 ? 60:0) + (lon0-3)/6 + 1;
    return "+ellps=krass +towgs84=+28,-130,-95 +proj=tmerc"
           " +lon_0=" + type_to_str(lon0) +
           " +x_0=" + type_to_str(pref) + "500000";
  }


  return pars;
}

ConvGeo::ConvGeo(const std::string & src,
       const std::string & dst, const bool use2d){
  cnv2d = use2d;
  sc_src = sc_dst = 1;

  if (src==dst) {
    pj_src = pj_dst = NULL;
    return;
  }

  // build PROJ handlers
  pj_src = std::shared_ptr<void>(
             pj_init_plus(expand_proj_aliases(src).c_str()), pj_free);
  if (!pj_src)
    throw Err() << "Can't create projection \""
                << src << "\": " << pj_strerrno(pj_errno);

  pj_dst = std::shared_ptr<void>(
             pj_init_plus(expand_proj_aliases(dst).c_str()), pj_free);
  if (!pj_dst)
    throw Err() << "Can't create projection \""
                << dst << "\": " << pj_strerrno(pj_errno);

  if (pj_is_latlong(pj_src.get())) sc_src=M_PI/180.0;
  if (pj_is_latlong(pj_dst.get())) sc_dst=180.0/M_PI;
}


void
ConvGeo::frw_pt(dPoint & p) const{
  if (sc_src!=1.0) {p.x*=sc_src; p.y*=sc_src;}  // this if increases speed...
  if (pj_src!=pj_dst) {
    double *z = (cnv2d || isnan(p.z))? NULL:&p.z;
    if (pj_transform(pj_src.get(), pj_dst.get(), 1, 1, &p.x, &p.y, z)!=0)
      throw Err() << "Can't convert coordinates: " << pj_strerrno(pj_errno);
    if (!isnormal(p.x) || !isnormal(p.y))
      throw Err() << "Can't convert coordinates: non-numeric result";
  }
  if (sc_dst!=1.0) {p.x*=sc_dst; p.y*=sc_dst;};
}

void
ConvGeo::bck_pt(dPoint & p) const{
  if (sc_dst!=1.0) {p.x/=sc_dst; p.y/=sc_dst;};
  if (pj_src!=pj_dst){
    double *z = (cnv2d || isnan(p.z))? NULL:&p.z;
    if (pj_transform(pj_dst.get(), pj_src.get(), 1, 1, &p.x, &p.y, z)!=0)
      throw Err() << "Can't convert coordinates: " << pj_strerrno(pj_errno);
    if (!isnormal(p.x) || !isnormal(p.y))
      throw Err() << "Can't convert coordinates: non-numeric result";
  }
  if (sc_src!=1.0) {p.x/=sc_src; p.y/=sc_src;}
}

bool
ConvGeo::is_src_deg() const {
  return pj_is_latlong(pj_src.get());}

bool
ConvGeo::is_dst_deg() const {
  return pj_is_latlong(pj_dst.get());}

/**********************************************************/

ConvMap::ConvMap(const GeoMap & m, const std::string & dst) {

  std::shared_ptr<ConvAff2D> img2map(new ConvAff2D());
  std::shared_ptr<ConvBase>  map2dst(new ConvGeo(m.proj, dst));

  // convert refpoints to map projection
  ConvGeo map2wgs(m.proj);
  std::map<dPoint,dPoint> refpts = m.ref;
  for (auto & pp:refpts) map2wgs.bck(pp.second);
  img2map->reset(refpts);

  push_back(img2map); // image -> map proj
  push_back(map2dst); // map proj -> dst
}


