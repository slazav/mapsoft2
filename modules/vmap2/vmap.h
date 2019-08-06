#ifndef VMAP_H
#define VMAP_H

#include <string>
#include <list>
#include <map>

#include "mapdb/db_simple.h"
#include "mapdb/db_geohash.h"
#include "geom/multiline.h"
#include "geohash/storage.h"
#include "mp/mp.h"
#include "vmap1/vmap1.h"

//#include "fig/fig.h"

// Class for vector map (without label information!)
// All coordinates are lat,lon in WGS84 datum.

/*********************************************************************/
// enums

typedef enum{
  VMAP_POINT    = 0,
  VMAP_LINE     = 1,
  VMAP_POLYGON  = 2
} VMapObjClass;

typedef enum{
  VMAP_DIR_NO    = 0,
  VMAP_DIR_FRW   = 1,
  VMAP_DIR_BCK   = 2
} VMapObjDir;

/*********************************************************************/
// VMapObj -- a single map object

struct VMapObj: public dMultiLine {
  VMapObjClass    cl;      // object class: VMAP_POINT, VMAP_LINE, VMAP_POLYGON
  int             type;    // = MP type
  VMapObjDir      dir;     // object direction: VMAP_DIR_NO, VMAP_DIR_FRW, VMAP_DIR_BCK
  float           angle;   // object angle, deg
  std::string     name;    // object name (to be printed on map labels)
  std::string     comm;    // object comment
  std::string     src;     // object source

  // defaults
  VMapObj() {cl=VMAP_POINT; type=0; dir=VMAP_DIR_NO; angle=0;}

  // pack object to a string (for DB storage)
  std::string pack() const;

  // unpack object from a string (for DB storage)
  void unpack(const std::string & s);

  /***********************************************/
  // operators <=>
  /// Less then operator.
  bool operator< (const VMapObj & o) const {
    if (cl!=o.cl)       return cl<o.cl;
    if (type!=o.type)   return type<o.type;
    if (dir!=o.dir)     return dir<o.dir;
    if (angle!=o.angle) return angle<o.angle;
    if (name!=o.name)   return name<o.name;
    if (comm!=o.comm)   return comm<o.comm;
    if (src!=o.src)     return src<o.src;
    return dMultiLine::operator<(o);
  }

  /// Equal opertator.
  bool operator== (const VMapObj & o) const {
    return cl==o.cl && type==o.type && dir==o.dir && angle==o.angle &&
        name==o.name && comm==o.comm && src==o.src &&
        dMultiLine::operator==(o);
  }
  // derived operators:
  bool operator!= (const VMapObj & other) const { return !(*this==other); } ///< operator!=
  bool operator>= (const VMapObj & other) const { return !(*this<other);  } ///< operator>=
  bool operator<= (const VMapObj & other) const { return *this<other || *this==other; } ///< operator<=
  bool operator>  (const VMapObj & other) const { return !(*this<=other); } ///< operator>

};


/*********************************************************************/
// VMap -- a storage for map objects

// TODO: use DB storage instead of map!
class VMap {
private:
    DBSimple   storage;
    GeoHashDB  geo_ind;

    dMultiLine  brd; // border (will be kept in the DB)
    dRect  bbox;     // bounding box (will be kept in the DB)

public:
  VMap(const std::string &name, bool create):
    storage((name + ".db").c_str(), "objects", create),
    geo_ind((name + ".db").c_str(), "geohash", create) {};

  /// Get border.
  dMultiLine get_brd() const {return brd;}

  /// Set border.
  void set_brd(const dMultiLine & b) { brd = b;}

  /// Get bbox.
  dRect get_bbox() const {return bbox;}

  /// Add object to the map.
  void add(const VMapObj & o);

  /* Import/export */

  /// Import objects from MP file.
  void import_mp(
    const std::string & mp_file,
    const Opt & opts);

  /// Export objects to MP file.
  void export_mp(
    const std::string & mp_file,
    const Opt & opts);

  /// Import objects from VMAP1 file.
  void import_vmap1(
    const std::string & vmap_file,
    const Opt & opts);

  /// Export objects to VMAP1 file.
  void export_vmap1(
    const std::string & vmap_file,
    const Opt & opts);

};



#endif
