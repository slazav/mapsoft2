#ifndef ICONV_H
#define ICONV_H

#include <string>
#include <cassert>

///\addtogroup libmapsoft
///@{

/// Declare iconv_t to avoid including <iconv.h> here
typedef void * iconv_t;

/// wrapper for libiconv
class IConv{

  /************************************/
  /* data and memory management */
  iconv_t cd; ///< conversion descriptor
  int *refcounter;
  void copy(const IConv & other);
  void destroy(void);

  /************************************/
  // Copy constructor, destructor, assignment
  public:

    IConv(const IConv & other){ copy(other); }
    IConv & operator=(const IConv & other){
      if (this != &other){ destroy(); copy(other); }
      return *this;
    }
    ~IConv(){ destroy(); }

  /************************************/
  /// Constructor
  IConv(const char *from, const char *to);

  /// convert
  std::string cnv(const std::string & s);
};

///@}
#endif
