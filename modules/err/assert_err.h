#ifndef ASSERT_ERR_H
#define ASSERT_ERR_H

///\addtogroup libmapsoft
///@{

#include <cassert>
#include "err.h"

// assert_err macro

#define assert_err(cmd,ret)\
  try{ cmd; assert(false); } catch (Err e) {\
    if (e.str()!=(ret)) std::cerr << "[" << e.str()<< "] != [" << (ret) << "]\n";\
    assert(e.str()==(ret));}

///@}
#endif
