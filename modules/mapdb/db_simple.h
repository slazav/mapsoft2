#ifndef DB_SIMPLE_H
#define DB_SIMPLE_H

#include <memory>
#include <string>
#include <stdint.h>
#include "geom/rect.h"

// Simple Berkleydb-based database with integer key and
// std::string value. I plan to use it for vector map attributes
// instead of shapelib/dbf.

class DBSimple{
  private:
    class Impl;
    std::unique_ptr<Impl> impl;

  public:

   DBSimple(const std::string & fname, bool create);
   ~DBSimple();

   // Put data with a given key (overwrite old value if it exists).
   void put(const uint32_t key, const std::string & val);

   // Get data for a given key.
   std::string get(const uint32_t key);

   // Get last key + 1, or 0 if database is empty.
   uint32_t get_end() const;

};

#endif
