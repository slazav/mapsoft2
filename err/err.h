#ifndef ERR_H
#define ERR_H

///\addtogroup libmapsoft
///@{

#include <iostream>
#include <sstream>
#include <string>

/***********************************************************/
/** A simple class for exceptions.
All mapsoft libraries throw human-readable text exceptions using a
simple Err class. There is also a possibility to transfer an integer error
code (default -1).

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
  cerr << "Error: " << E.str() << "\n";
}
```
*/

class Err {
  std::ostringstream s;    // stream for error messages
  int c;

  public:
    /// Constructor with optional error code.
    Err(int c_ = -1): c(c_) {}

    /// Copy constructor.
    Err(const Err & o) { c=o.c; s << o.s.str(); }

    /// Operator << for error messages.
    template <typename T>
      Err & operator<<(const T & o){ s << o; return *this; }

    /// Get error code.
    int code() const {return c;}

    /// Get error message.
    std::string str() const { return s.str(); }

};

///@}
#endif
