#include <proj_api.h>

#include "err/err.h"
#include "conv_geo.h"

ConvGeo::ConvGeo(const std::string & src, const std::string & dst){
  sc_src = sc_dst = 1;

  if (src==dst) {
    pj_src = pj_dst = NULL;
    return;
  }

  pj_src = std::shared_ptr<void>(pj_init_plus(src.c_str()), pj_free);
  if (!pj_src)
    throw Err() << "Can't create projection \""
                << src << "\": " << pj_strerrno(pj_errno);

  pj_dst = std::shared_ptr<void>(pj_init_plus(dst.c_str()), pj_free);
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
    double *z = isnan(p.z)? NULL:&p.z;
    if (pj_transform(pj_src.get(), pj_dst.get(), 1, 1, &p.x, &p.y, z)!=0)
      throw Err() << "Can't convert coordinates: " << pj_strerrno(pj_errno);
  }
  if (sc_dst!=1.0) {p.x*=sc_dst; p.y*=sc_dst;};
}

void
ConvGeo::bck_pt(dPoint & p) const{
  if (sc_dst!=1.0) {p.x/=sc_dst; p.y/=sc_dst;};
  if (pj_src!=pj_dst){
    double *z = isnan(p.z)? NULL:&p.z;
    if (pj_transform(pj_dst.get(), pj_src.get(), 1, 1, &p.x, &p.y, z)!=0)
      throw Err() << "Can't convert coordinates: " << pj_strerrno(pj_errno);
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

ConvMap::ConvMap(const GeoMap & m, const std::string & dst):
      map2dst(m.proj, dst) {

  // convert refpoints to map projection
  ConvGeo map2wgs(m.proj);
  std::map<dPoint,dPoint> refpts = m.ref;
  for (auto & pp:refpts) map2wgs.bck(pp.second);
  img2map.reset(refpts);

  push_back(&img2map); // image -> map proj
  push_back(&map2dst); // map proj -> dst
}


