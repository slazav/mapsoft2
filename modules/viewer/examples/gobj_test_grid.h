#ifndef GOBJ_TEST_GRID_H
#define GOBJ_TEST_GRID_H

#include "viewer/gobj.h"

// Grid for test purposes
class GObjTestGrid: public GObj{
  int delay;
public:
  GObjTestGrid(
    const int delay = 0,
    std::shared_ptr<ConvBase> c = std::shared_ptr<ConvBase>(new ConvBase)
  );
  virtual int draw(const CairoWrapper & cr, const dRect &box) override;
};


#endif
