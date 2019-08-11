#ifndef DB_SIMPLE_H
#define DB_SIMPLE_H

#include <memory>
#include <string>
#include <stdint.h>
#include "geom/rect.h"

// Simple Berkleydb-based database with integer key and
// std::string value.

class DBSimple{
  private:
    class Impl;
    std::unique_ptr<Impl> impl;

  public:

   // TODO: write iterator?

   // open database:
   // fname - file name
   // dbname - database name (can be NULL),
   // create - create flag.
   // dup    - alow duplicates flag (default false).
   // Note: if you use non-null dbname and put a few databases in a single file,
   // you will need to create environment to open both databases.
   DBSimple(std::string fname, const char *dbname, bool create, bool dup = false);
   ~DBSimple();

   // Put data with a given key (overwrite old value if it exists).
   // If key duplication is not allowed and key already exists,
   // then the record will be overwritten.
   void put(const uint32_t key, const std::string & val);

   // Put data after the last record, return the new key
   uint32_t put(const std::string & val) {
     uint32_t key;
     get_last(key);
     key = 0xFFFFFFFF ? 0 : key+1;
     put(key, val);
     return key;
   }

   // Check if the key exists in the database.
   bool exists(const uint32_t key);

   // Get data for a given key.
   // If record is not found then key is set to 0xFFFFFFFF and
   // empty string is returned.
   // Equivalent to DB_SET flag in BerkleyDB
   std::string get(uint32_t & key);

   // Get data with key larger or equals then the given key.
   // Value of key is set according to the returned record.
   // If record is not found then key is set to 0xFFFFFFFF and
   // empty string is returned.
   // Equivalent to DB_SET_RANGE flag in BerkleyDB
   std::string get_range(uint32_t & key);

   // Get next entry.
   // If record is not found then key is set to 0xFFFFFFFF and
   // empty string is returned.
   // On input key is ignored, cursor position is used.
   // Equivalent to DB_NEXT flag in BerkleyDB
   std::string get_next(uint32_t & key);

   // Get previous entry.
   // If record is not found then key is set to 0xFFFFFFFF and
   // empty string is returned.
   // On input key is ignored, cursor position is used.
   // Equivalent to DB_PREV flag in BerkleyDB
   std::string get_prev(uint32_t & key);

   // Get last entry.
   // If record is not found key is set to 0xFFFFFFFF and
   // empty string is returned. On input key is ignored.
   // Equivalent to DB_LAST flag in BerkleyDB
   std::string get_last(uint32_t & key);

   // Get first entry.
   // If record is not found key is set to 0xFFFFFFFF and
   // empty string is returned. On input key is ignored.
   // Equivalent to DB_FIRST flag in BerkleyDB
   std::string get_first(uint32_t & key);

   // Delete entry with a given key.
   // If key duplication is allowed then multiple records can be deleted.
   // Returns number of deleted entries (maybe zero)
   uint32_t del(const uint32_t key);

};


#endif
