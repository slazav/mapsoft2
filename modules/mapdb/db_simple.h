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

   // TODO: write iterator?

   // open database:
   // fname - file name, dbname - database name (can be NULL),
   // create - create flag.
   DBSimple(const char *fname, const char *dbname, bool create);
   ~DBSimple();

   // Put data with a given key (overwrite old value if it exists).
   void put(const uint32_t key, const std::string & val);

   // Put data aftrer the last record, return the new key
   uint32_t put(const std::string & val) {
     uint32_t key;
     get_last(key);
     key = 0xFFFFFFFF ? 0 : key+1;
     put(key, val);
     return key;
   }

   // Get data for a given key.
   // If record is not found then key is set to 0xFFFFFFFF and
   // empty string is returned.
   std::string get(uint32_t & key);

   // Get data with key larger or equals then the given key.
   // If record is not found then key is set to 0xFFFFFFFF and
   // empty string is returned.
   std::string get_first(uint32_t & key);

   // Get next entry.
   // If record is not found then key is set to 0xFFFFFFFF and
   // empty string is returned.
   // On input key is ignored, cursor position is used.
   std::string get_next(uint32_t & key);

   // Get previous entry.
   // If record is not found then key is set to 0xFFFFFFFF and
   // empty string is returned.
   // On input key is ignored, cursor position is used.
   std::string get_prev(uint32_t & key);

   // Get last entry.
   // If record is not found key is set to 0xFFFFFFFF and
   // empty string is returned.
   // On input key is ignored, cursor position is used.
   std::string get_last(uint32_t & key);

};


#endif
