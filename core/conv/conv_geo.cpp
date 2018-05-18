#include "conv_geo.h"
#include "err/err.h"

ConvGeo::ConvGeo(const std::string & src, const std::string & dst){
  refcounter   = new int;
  *refcounter  = 1;

  if (src==dst) {
    pj_src = pj_dst = NULL;
    sc_src = sc_dst = 1;
    return;
  }

  pj_src = pj_init_plus(src.c_str());
  if (!pj_src)
    throw Err() << "Can't create projection \""
                << src << "\": " << pj_strerrno(pj_errno);

  pj_dst = pj_init_plus(dst.c_str());
  if (!pj_dst){
    pj_free(pj_src);
    throw Err() << "Can't create projection \""
                << dst << "\": " << pj_strerrno(pj_errno);
  }
  if (pj_is_latlong(pj_src)) sc_src=M_PI/180.0;
  if (pj_is_latlong(pj_dst)) sc_dst=180.0/M_PI;
}

ConvGeo &
ConvGeo::operator=(const ConvGeo & other){
  if (this != &other){
    destroy();
    copy(other);
  }
  return *this;
}

void
ConvGeo::copy(const ConvGeo & other){
   this->ConvBase::operator=(other);
   refcounter = other.refcounter;
   pj_src = other.pj_src;
   pj_dst = other.pj_dst;
   (*refcounter)++;
   if (*refcounter<=0)
     throw Err() << "ConvGeo refcounter error: " << *refcounter;
}

void
ConvGeo::destroy(void){
  (*refcounter)--;
  if (*refcounter<=0){
    delete refcounter;
    if (pj_dst) pj_free(pj_dst);
    if (pj_src) pj_free(pj_src);
  }
}


