#ifndef MAPDB_H
#define MAPDB_H

#include <string>
#include <list>
#include <map>
#include <set>

#include "db_simple.h"
#include "db_geohash.h"
#include "geom/multiline.h"

//#include "fig/fig.h"

// current MapDB version (integer)
#define MAPDB_VERSION 0

// Class for vector map (without label information!)
// All coordinates are lat,lon in WGS84 datum.

/*********************************************************************/
// enums

typedef enum{
  MAPDB_POINT    = 0,
  MAPDB_LINE     = 1,
  MAPDB_POLYGON  = 2
} MapDBObjClass;

typedef enum{
  MAPDB_DIR_NO    = 0,
  MAPDB_DIR_FRW   = 1,
  MAPDB_DIR_BCK   = 2
} MapDBObjDir;

/*********************************************************************/
// MapDBObj -- a single map object

struct MapDBObj: public dMultiLine {
  MapDBObjClass   cl;      // object class: MAPDB_POINT, MAPDB_LINE, MAPDB_POLYGON
  uint16_t        type;    // = MP type
  MapDBObjDir     dir;     // object direction: MAPDB_DIR_NO, MAPDB_DIR_FRW, MAPDB_DIR_BCK
  float           angle;   // object angle, deg
  std::string     name;    // object name (to be printed on map labels)
  std::string     comm;    // object comment
  std::set<std::string> tags;    // object tags

  // defaults
  MapDBObj() {cl=MAPDB_POINT; type=0; dir=MAPDB_DIR_NO; angle=0;}

  // pack object to a string (for DB storage)
  std::string pack() const;

  // unpack object from a string (for DB storage)
  void unpack(const std::string & s);

  /***********************************************/
  // operators <=>
  /// Less then operator.
  bool operator< (const MapDBObj & o) const {
    if (cl!=o.cl)       return cl<o.cl;
    if (type!=o.type)   return type<o.type;
    if (dir!=o.dir)     return dir<o.dir;
    if (angle!=o.angle) return angle<o.angle;
    if (name!=o.name)   return name<o.name;
    if (comm!=o.comm)   return comm<o.comm;
    if (tags!=o.tags)   return tags<o.tags;
    return dMultiLine::operator<(o);
  }

  /// Equal opertator.
  bool operator== (const MapDBObj & o) const {
    return cl==o.cl && type==o.type && dir==o.dir && angle==o.angle &&
        name==o.name && comm==o.comm && tags==o.tags &&
        dMultiLine::operator==(o);
  }
  // derived operators:
  bool operator!= (const MapDBObj & other) const { return !(*this==other); } ///< operator!=
  bool operator>= (const MapDBObj & other) const { return !(*this<other);  } ///< operator>=
  bool operator<= (const MapDBObj & other) const { return *this<other || *this==other; } ///< operator<=
  bool operator>  (const MapDBObj & other) const { return !(*this<=other); } ///< operator>

};


/*********************************************************************/
// MapDB -- a storage for map objects

class MapDB {
private:

  // class for checking/making database folder
  class FolderMaker{ public: FolderMaker(std::string name, bool create); };

  FolderMaker folder; // Making folder for the databases.
                      // This line should appear before all database members.

  DBSimple   mapinfo; // map information
  DBSimple   objects; // object data
  DBSimple   labels;  // label data
  GeoHashDB  geohash; // geohashes for spatial indexing

  int map_version; // set in the constructor

public:

  // Constructor. Open all databases, check map version.
  MapDB(std::string name, bool create);

  ///////////////
  /* Function for working with map information (mapinfo.db) */

  /// Get map version.
  uint32_t get_map_version() const {return map_version;}

  /// Get map name. If the field is not set return empty string without an error.
  std::string get_map_name();

  /// Set map name
  void set_map_name(const std::string & name);

  /// Get map border. If the field is not set return empty dMultiLine without an error
  dMultiLine get_map_brd();

  /// Set map border
  void set_map_brd(const dMultiLine & b);


  ///////////////
  /* Functions for working with map objects */

  /// Add object to the map, return object ID.
  uint32_t add(const MapDBObj & o);

  /// Rewrite existing object, update geohashes.
  /// If object does not exist it will be added anyway.
  void put(uint32_t id, const MapDBObj & o);

  /// Read an object.
  MapDBObj get(uint32_t id);

  /// Delete an object.
  /// If the object does not exist throw an error.
  void del(uint32_t id);

  /// Find objects with given type and range
  std::set<int> find(MapDBObjClass cl, int type, const dRect & range);


  ///////////////
  /* Import/export */
  public:

  /// Import objects from MP file.
  void import_mp(
    const std::string & mp_file,
    const Opt & opts);

  /// Export objects to MP file.
  void export_mp(
    const std::string & mp_file,
    const Opt & opts);

  /// Import objects from VMAP file.
  void import_vmap(
    const std::string & vmap_file,
    const Opt & opts);

  /// Export objects to VMAP file.
  void export_vmap(
    const std::string & vmap_file,
    const Opt & opts);

};



#endif
