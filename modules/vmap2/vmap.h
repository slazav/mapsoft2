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
    DBSimple   objects, mapinfo;
    GeoHashDB  geo_ind;

public:
  VMap(const char *name, bool create):
    mapinfo(name, "mapinfo", create),
    objects(name, "objects", create),
    geo_ind(name, "geohash", create) {};

#define MAPINFO_NAME 1
#define MAPINFO_BRD  2
#define MAPINFO_BBOX 3

  ///////////////

  /// Get map name. If the field is not set return empty string without an error.
  std::string get_name();

  /// Set map name
  void set_name(const std::string & name);

  /// Get map border. If the field is not set return empty dMultiLine without an error
  dMultiLine get_brd();

  /// Set map border
  void set_brd(const dMultiLine & b);

  /// Get map bounding box. If the field is not set return empty dRect
  dRect get_bbox();

  /// Set map bounding box (internal use only!)
  private: void set_bbox(const dRect & b);

  public:


  /// Add object to the map.
  uint32_t add(const VMapObj & o);

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
