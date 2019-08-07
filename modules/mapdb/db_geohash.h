#ifndef DB_GEOHASH_H
#define DB_GEOHASH_H

#include <memory>
#include <set>
#include "geom/rect.h"

// Berkleydb-based geohash database for spatial indexing
class GeoHashDB {
  private:
    class Impl;
    std::unique_ptr<Impl> impl;

  public:

   GeoHashDB(const char *fname, const char *dbname, bool create);
   ~GeoHashDB();

   // add object with id and range.
   void put(const int id, const dRect & range);

   // Delete object with id and range.
   // If the record does not exist do nothing.
   void del(const int id, const dRect & range);

   // get id of objects which may be found in the range
   std::set<int> get(const dRect & range);
};


#endif
