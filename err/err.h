#ifndef ERR_H
#define ERR_H

///\addtogroup libmapsoft
///@{
///\defgroup Err
///Simple class for exceptions
///@{

#include <iostream>
#include <sstream>
#include <string>

/***********************************************************/
/** Err -- a simple class for exceptions.
Example:
```
try {

  // throw an error with any text:
  throw Err() << "pipe " << n << "is blocked!";

  // some error code can be added (instead of default -1):
  throw Err(-2) << "some other error";
}

// catch an error:
catch (Err E){
  cerr << E.get_error();
}
```
*/

class Err {
  std::ostringstream s;    // stream for error messages
  int c;

  public:
    Err(int c_ = -1): c(c_) {}
    Err(const Err & o) { c=o.c; s << o.s.str(); }

    /// Operator << for error messages.
    template <typename T>
      Err & operator<<(const T & o){ s << o; return *this; }

    /// Get error code.
    int code() const {return c;}

    /// Return error message.
    std::string str() const { return s.str(); }

};

#endif
