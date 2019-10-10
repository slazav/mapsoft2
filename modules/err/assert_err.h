#ifndef ASSERT_ERR_H
#define ASSERT_ERR_H

///\addtogroup libmapsoft
///@{

#include <cmath> // fabs
#include <cassert>
#include "err.h"

// assert_err macro

#define assert_err(cmd,ret)\
  {try{\
    cmd;\
    throw Err(-9999)\
      << "assert_err: " << __FILE__ << ":" << __LINE__ << ": error is not thrown:\n"\
      << "command: " << #cmd << "\n"\
      << "expected error: " << (ret)<< "\n";\
  } catch (Err e) {\
    if (e.code()==-9999) throw e;\
    if (e.str()!=(ret)){\
      throw Err()\
        << "assert_err: " << __FILE__ << ":" << __LINE__ << ": wrong error message:\n"\
        << "command: " << #cmd << "\n"\
        << "expected error: " << (ret) << "\n"\
        << "actual error:   " << e.str()<< "\n";\
    }\
  }}

//  catch an error and check it
#define assert_eq(v1,v2)\
  {if ((v1) != (v2)){\
    throw Err()\
      << "assert_eq: " << __FILE__ << ":" << __LINE__ << ": arguments are not equal:\n"\
      << "v1: " << #v1  << "\n"\
      << "    " << (v1) << "\n"\
      << "v2: " << #v2  << "\n"\
      << "    " << (v2) << "\n";\
  }}

// compare two double values and check that difference is less then e
#define assert_feq(v1,v2,e)\
  {if (fabs((v1) - (v2)) > e){\
    throw Err()\
      << "assert_feq: " << __FILE__ << ":" << __LINE__ << ": arguments are not equal:\n"\
      << "v1: " << #v1  << "\n"\
      << "    " << (v1) << "\n"\
      << "v2: " << #v2  << "\n"\
      << "    " << (v2) << "\n";\
  }}

// compare two objects with a dist(a,b) function, check thet the result is less then e
#define assert_deq(v1,v2,e)\
  {if (dist((v1),(v2)) > e){\
    throw Err()\
      << "assert_feq: " << __FILE__ << ":" << __LINE__ << ": arguments are not equal:\n"\
      << "v1: " << #v1  << "\n"\
      << "    " << (v1) << "\n"\
      << "v2: " << #v2  << "\n"\
      << "    " << (v2) << "\n";\
  }}

///@}
#endif
