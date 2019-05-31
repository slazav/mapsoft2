#ifndef SHP_H
#define SHP_H

#include <memory>
#include "geom/multiline.h"

#define MAP_POINT   0
#define MAP_LINE    1
#define MAP_POLYGON 2

// Shapelib wrapper for .shp/.shx files
class Shp{
  private:
    class Impl;
    std::unique_ptr<Impl> impl;

  public:

   Shp(const char *fbase, bool create, int type = MAP_LINE);
   ~Shp();

   int get_type() const;
   int get_num() const;

   int put(const dMultiLine &ml);
   dMultiLine get(const int id);
};

#endif
