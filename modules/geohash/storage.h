#ifndef GEOHASH_STORAGE_H
#define GEOHASH_STORAGE_H

#include <memory>
#include <map>
#include <set>
#include "geom/rect.h"

// In-memory (std::map-based) geohash database for spatial indexing
class GeoHashStorage {
  private:
    std::multimap<std::string, int> storage;
    virtual std::set<int> get_hash(const std::string & hash0, bool exact);
    dRect bbox;

  public:
   // add object with id and range.
   virtual void put(const int id, const dRect & range);

   // get id of objects which may be found in the range
   std::set<int> get(const dRect & range);

   // Delete an object with id and range.
   // If the record does not exist do nothing.
   void del(const int id, const dRect & range);

   // set bbox for coordinate transformation
   void set_bbox(const dRect & bbox_){ bbox = bbox_; }
};


#endif
