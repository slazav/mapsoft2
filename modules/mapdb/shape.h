#ifndef SHAPE_H
#define SHAPE_H

#include <memory>
#include <shapefil.h>

#include "geom/multiline.h"

#define MAP_POINT   0
#define MAP_LINE    1
#define MAP_POLYGON 2

// Shapelib wrapper
class Shape{
  private:
    class Impl;
    std::unique_ptr<Impl> impl;

  public:

   Shape(const char *fbase, bool create, int type = MAP_LINE);
   ~Shape();

   int get_type() const;
   int get_num() const;

   int put(const dMultiLine &ml);
   dMultiLine get(const int id);
};

#endif
