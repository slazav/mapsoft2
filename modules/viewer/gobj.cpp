#include "gobj.h"
#include <limits.h> // for INT_MIN, INT_MAX

const int GObj::TILE_SIZE;
const int GObj::FILL_NONE;
const int GObj::FILL_PART;
const int GObj::FILL_ALL;
const iRect GObj::MAX_RANGE(iPoint(INT_MIN/2, INT_MIN/2),
                            iPoint(INT_MAX/2, INT_MAX/2));
