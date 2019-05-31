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

  public:
   // add object with id and range.
   virtual void put(const int id, const dRect & range);

   // get id of objects which may be found in the range
   std::set<int> get(const dRect & range);
};


#endif
