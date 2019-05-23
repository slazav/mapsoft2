#ifndef GEO_DATA_H
#define GEO_DATA_H

#include <vector>
#include <string>
#include <cmath>
#include "geom/point.h"
#include "geom/line.h"
#include "geom/rect.h"
#include "opt/opt.h"

///\addtogroup libmapsoft
///@{

///\defgroup GeoData Mapsoft geodata classes and functions.
///@{

#define UNDEF_ALT -1e7

/********************************************************************/
/// Distance in m between two points (haversine formula, altitude is ignored).
double geo_dist_2d(const dPoint &p1, const dPoint &p2);

/********************************************************************/
/// Single waypoint, a child of dPoint. Can have "undefined" altitude,
/// in this case it is not used in transformations (See ConvGeo class).
/// All additional information lives in opt variable.
/// Note: default z in GeoWpt is UNDEF_ALT, and in dPoint is 0;
struct GeoWpt : dPoint {
  std::string name; ///< name
  std::string comm; ///< comment
  Opt opts;         ///< Waypoint options
  time_t t;         ///< unix time (ms)

  /// constructors
  GeoWpt() {z=UNDEF_ALT; t=0;}
  GeoWpt(const dPoint &p): dPoint(p){}
  GeoWpt(const double x, const double y, const double z=UNDEF_ALT):
    dPoint(x,y,z), t(0){}

  /// check if altitude is defined
  bool have_alt() const {return z>UNDEF_ALT;}

  /// set the altitude to undefined state
  void clear_alt() {z=UNDEF_ALT;}
};

/********************************************************************/
/// Waypoint list. An std::vector of GeoWpt with additional options.
struct GeoWptList : std::vector<GeoWpt>{
  std::string name; ///< name
  std::string comm; ///< comment
  Opt opts; ///< Waypoint list options

  /// constructor
  GeoWptList() {}

  /// get x-y range in lon-lat coords
  dRect bbox2d() const;

  /// set the altitude of all points to undefined state
  void clear_alt();
};

/********************************************************************/
/// single trackpoint
struct GeoTpt : dPoint {
  bool start; ///< start flag
  time_t t;   ///< unix time (ms)

  /// constructor
  GeoTpt(): start(false), t(0) {z=UNDEF_ALT;}
  GeoTpt(const dPoint &p): dPoint(p), start(false) {}
  GeoTpt(const double x, const double y, const double z=UNDEF_ALT,
         const bool start = false, const time_t t=0):
    dPoint(x,y,z), start(start), t(t) {}

  /// check if altitude is defined
  bool have_alt() const {return z>UNDEF_ALT;}

  /// set the altitude to undefined state
  void clear_alt() {z=UNDEF_ALT;}
};

/********************************************************************/
/// track
struct GeoTrk : std::vector<GeoTpt>{
  std::string name; ///< name
  std::string comm; ///< comment
  Opt opts; ///< Track options

  /// Constructor
  GeoTrk() {}

  /// Get x-y range in lon-lat coords.
  dRect bbox2d() const;

  /// Get length in m (using Haversine formula).
  double length() const;

  /// convert to dLine
  operator dLine() const;

  /// set the altitude of all points to undefined state
  void clear_alt();
};


/********************************************************************/
/// GeoMap. Shows how to draw some region on a 2D picture.
struct GeoMap{
  std::string name; ///< name
  std::string comm; ///< comment
  std::map<dPoint,dPoint> ref; ///< reference points, mapping from geo to image coordinates
  dLine border;                ///< map border (in image coordinates), should be always set
  std::string proj;            ///< map projection (option string for libproj)

  std::string image;           ///< image file for the map (folder for tile maps)
  dRect image_bbox;            ///< image boundary box (in image coordinates)
  dRect image_tsize;           ///< image tile size (for tiled maps)
  std::string image_tfmt;      ///< image tile format (for tiled maps)
  bool image_tswap;            ///< are image tiles swapped in y (for tiled maps)

  /// Constructor: create empty map
  GeoMap(): image_tswap(false) {}

  /******************************************************************/
  // operators +,-,/,*

  /// Add p to image coordinates (shift the map)
  /// TODO: maybe it is better not to shift image_bbox.
  /// now there is no reason to add/subtract points.
  GeoMap & operator+= (const dPoint & p) {
    for (auto i:ref) i.second+=p;
    border+=p;
    image_bbox+=p;
    return *this;
  }

  /// Multiply image coordinates by k (scale the map)
  GeoMap & operator*= (const double k) {
    for (auto i:ref) i.second*=k;
    border*=k;
    image_bbox*=k;
    return *this;
  }

  /// Add p to image coordinates (shift the map)
  GeoMap operator+ (const dPoint & p) const { GeoMap ret(*this); return ret+=p; }

  /// Subtract p from image coordinates (shift the map)
  GeoMap & operator-= (const dPoint & p) { *this+=-p; return *this; }

  /// Subtract p from image coordinates (shift the map)
  GeoMap operator- (const dPoint & p) const { GeoMap ret(*this); return ret+=-p; }

  /// Multiply image coordinates by k (scale the map)
  GeoMap operator* (const double k) const { GeoMap ret(*this); return ret*=k; }

  /// Divide image coordinates by k (scale the map)
  GeoMap & operator/= (const double k) { *this*=1.0/k; return *this; }

  /// Divide image coordinates by k (scale the map)
  GeoMap operator/ (const double k) const { GeoMap ret(*this); return ret*=1.0/k; }

  /******************************************************************/

//  /// Get x-y range in lon-lat coords (using border, ref, proj).
//  dRect bbox2d() const;
};

/********************************************************************/
/// map list
struct GeoMapList : public std::vector<GeoMap>{
  std::string name; ///< name
  std::string comm; ///< comment
  Opt opts; ///< Map list options

  /// constructor
  GeoMapList() {}

//  /// get x-y range in lon-lat coords
//  dRect bbox2d() const;
};

/******************************************************************/
// additional operators for 

/// Multiply coordinates by k (k*line = line*k)
/// \relates GeoMap
GeoMap operator* (const double k, const GeoMap & l);

/// Add p to every point (p+line = line+p)
/// \relates GeoMap
GeoMap operator+ (const dPoint & p, const GeoMap & l);

/********************************************************************/

struct GeoData{
  Opt opts;
  std::list<GeoWptList> wpts;
  std::list<GeoTrk>     trks;
  std::list<GeoMapList> maps;

  /// clear all data
  void clear() { wpts.clear(); trks.clear(); maps.clear();}

  bool empty() const { return wpts.empty() && trks.empty() && maps.empty(); }

  void push_back(const GeoWptList & d) {wpts.push_back(d);}
  void push_back(const GeoTrk     & d) {trks.push_back(d);}
  void push_back(const GeoMapList & d) {maps.push_back(d);}

//  /// get range of all maps in lon-lat coords, fast
//  dRect range_map() const;
//  /// get range of all maps in lon-lat coords using file size
//  dRect range_map_correct() const;
//  /// get range of all tracks and waypoints in lon-lat coords
//  dRect range_geodata() const;
//  /// get range of all data in lon-lat coords
//  dRect range() const;

  // add data from another geo_data object
//  void add(const geo_data & w);

};

//#ifdef SWIG
//%template(vector_GeoTpt) std::vector<GeoTpt>;
//%template(vector_GeoTrk)  std::vector<GeoTrk>;
//#endif  // SWIG

///@}
///@}
#endif

