#include <list>
#include <vector>
#include <string>
#include <map>
#include <iostream>

//#include "2d/line_utils.h"
//#include "2d/line_rectcrop.h"
//#include "geo_io/geofig.h"
//#include "geo/geo_convs.h"
//#include "geo/geo_nom.h"
//#include "vmap/zn.h"
#include "vmap.h"

using namespace std;

/***************************************/

bool
VMapLab::operator< (const VMapLab & o) const{
  if (pos != o.pos) return (pos < o.pos);
  if (dir != o.dir) return (dir < o.dir);
  if (hor != o.hor) return (hor < o.hor);
  if (ang != o.ang) return (ang < o.ang);
  return false;
}

VMapLab::VMapLab(){
  dir=0; ang=0.0; fsize=0; hor=true;
}

bool
VMapLfull::operator< (const VMapLfull & o) const{
  if (text != o.text) return (text < o.text);
  if (ref != o.ref) return (ref < o.ref);
  return VMapLab::operator<(o);
}

object_class
VMapObj::get_class() const{
  if (type & 0x200000) return VMAP1_POLYGON;
  if (type & 0x100000) return VMAP1_POLYLINE;
  return VMAP1_POI;
}

bool
VMapObj::operator< (const VMapObj & o) const{
  if (type != o.type) return (type < o.type);
  if (text != o.text) return (text < o.text);
  if (dir  != o.dir)  return (dir < o.dir);
  return dMultiLine::operator<(o);
}

VMapObj::VMapObj(){
  type=0; dir=0;
}

dRect
VMap::range() const{
  dRect ret;
  for (auto const & o:*this) ret.expand(o.bbox());
  return ret;
}

void
VMap::add(const VMap & W){
  mp_id  = W.mp_id;
  name   = W.name;
  style  = W.style;
  rscale = W.rscale;
  brd    = W.brd;
  insert(end(), W.begin(), W.end());
  lbuf.insert(lbuf.end(), W.lbuf.begin(), W.lbuf.end());
}

VMap::VMap(){
  mp_id=0;
  rscale=0;
  style="";
}
