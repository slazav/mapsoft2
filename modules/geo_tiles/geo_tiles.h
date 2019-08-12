#ifndef GEO_TILES_H
#define GEO_TILES_H

#include "geom/rect.h"
#include "err/err.h"
#include "opt/opt.h"
#include <string>
#include <vector>

///\addtogroup libmapsoft
///@{
///\defgroup Tiles
///TMS/Google tile calculator
///@{

/// TMS/Google tile calculator.
/// See: http://www.maptiler.org/google-maps-coordinates-tile-bounds-projection/
class GeoTiles{

  double width;  // meters in 360 degrees on equator (z=0 tile size)
  int tsize;    // tile size
  double ires;  // initial resolution, 156543.03392804062 for tsize=256
  double shift; // origin shift, 20037508.342789244

  public:

  /// constructor: initialise width
  GeoTiles(const int tile_size=256){
    // intialize the TMS Global Mercator pyramid"
    tsize = tile_size;
    ires  = 2 * M_PI * 6378137 / tsize; // 156543.03392804062 for tsize=256
    shift = 2 * M_PI * 6378137 / 2.0;   // 20037508.342789244
  }

  /********************************************************/

  /// Converts XY point from Spherical Mercator EPSG:900913 to lon/lat in WGS84 Datum.
  dPoint m_to_ll(const dPoint & p) const {
    dPoint ret(p/shift * 180.0);
    ret.y = 180/M_PI * (2.0*atan(exp(ret.y * M_PI/180.0)) - M_PI/2.0);
    return ret;
  }
  /// Converts given lon/lat in WGS84 Datum to XY in Spherical Mercator EPSG:900913.
  dPoint ll_to_m(const dPoint & p) const {
    dPoint ret(p);
    ret.y = log(tan((90.0 + ret.y) * M_PI/360.0 )) / (M_PI/180.0);
    return ret * shift/180.0;
  }

  /// Converts pixel coordinates in given zoom level of pyramid to EPSG:900913.
  dPoint px_to_m(const dPoint & p, const int z) const {
    return p*ires/(1<<z) - dPoint(shift,shift);
  }
  /// Converts EPSG:900913 to pyramid pixel coordinates in given zoom level.
  dPoint m_to_px(const dPoint & p, const int z) const {
    return (p + dPoint(shift,shift))/ires*(1<<z);
  }

  double px2m(const int z) const { return ires/(1<<z); }

  /// Returns a tile covering region in given pixel coordinates.
  iPoint px_to_tile(const dPoint & p) const {
    return iPoint(
      int(ceil(p.x/tsize) - 1),
      int(ceil(p.y/tsize) - 1));
  }

  /// Converts TMS tile coordinates to Google Tile coordinates (and back).
  iPoint tile_gtile(const iPoint &p, const int z) const {
    iPoint ret(p);
    ret.y = ((1<<z) - 1) - ret.y;
    return ret;
  }

  /// Converts TMS tile coordinates to Microsoft QuadTree.
  std::string tile_to_qtree(const iPoint &p, const int z) const {
    std::string ret;
    iPoint pp(p);
    pp.y = ((1<<z) - 1) - pp.y;
    for (int i=z; i>=0; i--){
      int digit = 0;
      int mask = 1 << (i-1);
      if ((pp.x & mask) != 0) digit += 1;
      if ((pp.y & mask) != 0) digit += 2;
      ret += type_to_str(digit);
    }
    return ret;
  }

  /********************************************************/
  /// Returns WGS84 region for a given TMS tile.
  dRect tile_to_range(iPoint tile, const int z) const {
    return dRect(
      m_to_ll(px_to_m(tile*tsize, z)),
      m_to_ll(px_to_m((tile+iPoint(1,1))*tsize, z)) );
  }

  /// same but with integer arguments.
  dRect tile_to_range(const int x, const int y, const int z) const {
    return tile_to_range(iPoint(x,y), z); }

  /// Find TMS tile with scale z which covers a given point.
  iPoint pt_to_tile(const dPoint & p, const int z) const {
    return px_to_tile(m_to_px(ll_to_m(p), z));
  }

  /// Return TMS tiles with scale z which cover a region.
  iRect range_to_tiles(const dRect & range, const int z) const {
    return iRect(
      pt_to_tile(range.tlc(), z),
      pt_to_tile(range.brc(), z)+iPoint(1,1));
  }

  /********************************************************/
  /// Returns WGS84 region for a given google tile.
  dRect gtile_to_range(iPoint tile, const int z) const {
    return dRect(
      m_to_ll(px_to_m(tile_gtile(tile,z)*tsize, z)),
      m_to_ll(px_to_m((tile_gtile(tile,z)+iPoint(1,1))*tsize, z)) );
  }

  /// same but with integer arguments.
  dRect gtile_to_range(const int x, const int y, const int z) const {
    return tile_to_range(tile_gtile(iPoint(x,y),z),z); }

  /// Find google tile with scale z which covers a given point.
  iPoint pt_to_gtile(const dPoint & p, const int z) const {
    return tile_gtile(px_to_tile(m_to_px(ll_to_m(p), z)),z);
  }

  /// Return google tiles with scale z which cover a region.
  iRect range_to_gtiles(const dRect & range, const int z) const {
    return iRect(
      tile_gtile(pt_to_tile(range.blc(), z),z),
      tile_gtile(pt_to_tile(range.trc(), z),z)+iPoint(1,1));
  }


};


#endif
