#ifndef DB_SIMPLE_H
#define DB_SIMPLE_H

#include <memory>
#include <string>
#include "geom/rect.h"

// Simple Berkleydb-based database with integer key and
// std::string value. I plan to use it for vector map attributes
// instead of shapelib/dbf.

class DBSimple{
  private:
    class Impl;
    std::unique_ptr<Impl> impl;

  public:

   DBSimple(const char *fbase, bool create);
   ~DBSimple();

   void put(const int key, const std::string & val);

   std::string get(const int key);
};

#endif
