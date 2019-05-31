#ifndef DB_GEOHASH_H
#define DB_GEOHASH_H

#include <memory>
#include <set>
#include "geom/rect.h"

// Base class of geohash storage for spatial indexing
class GeoHashBase{
  public:

   // add object with id and range.
   virtual void put(const int id, const dRect & range) = 0;

   // get id of objects which may be found in the range
   virtual std::set<int> get(const dRect & range) = 0;
};

// In-memory (std::map-based) geohash database for spatial indexing
class GeoHashM : GeoHashBase {
  private:
    class Impl;
    std::unique_ptr<Impl> impl;

  public:

   GeoHashM();
   ~GeoHashM();

   // add object with id and range.
   void put(const int id, const dRect & range);

   // get id of objects which may be found in the range
   std::set<int> get(const dRect & range);
};

// Berkleydb-based geohash database for spatial indexing
class GeoHashDB : GeoHashBase {
  private:
    class Impl;
    std::unique_ptr<Impl> impl;

  public:

   GeoHashDB(const char *fbase, bool create);
   ~GeoHashDB();

   // add object with id and range.
   void put(const int id, const dRect & range);

   // get id of objects which may be found in the range
   std::set<int> get(const dRect & range);
};


#endif
