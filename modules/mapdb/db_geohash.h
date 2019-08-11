#ifndef DB_GEOHASH_H
#define DB_GEOHASH_H

#include <memory>
#include <set>
#include <stdint.h>
#include "geom/rect.h"

// Berkleydb-based geohash database for spatial indexing
class GeoHashDB {
  private:
    class Impl;
    std::unique_ptr<Impl> impl;

  public:

   GeoHashDB(std::string fname, const char *dbname, bool create);
   ~GeoHashDB();

   // Get id of objects of certain type which may be found in the range.
   std::set<int> get(const uint32_t type, const dRect & range);

   // Add an object with id, type and range.
   void put(const uint32_t id, const uint32_t type, const dRect & range);

   // Delete an object with id, type and range.
   // If the record does not exist do nothing.
   void del(const uint32_t id, const uint32_t type, const dRect & range);

};


#endif
