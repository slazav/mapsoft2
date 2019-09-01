#include <iconv.h>
#include "iconv.h"
#include <errno.h>
#include <iostream>
#include <cstring>
#include "err/err.h"

#define ERR (iconv_t)(-1)

class IConv::Impl {
  std::shared_ptr<void> cdp;

  public:

  Impl(const std::string & from, const std::string & to){
    cdp = std::shared_ptr<void>(iconv_open(to.c_str(), from.c_str()), iconv_close);
    if ((iconv_t)(cdp.get()) == ERR) throw Err() <<
      "can't do iconv conversion from " << from << " to " << to;
  }

  ~Impl() {}

  std::string cnv(const std::string & s) const;

};


IConv::IConv(const std::string & from, const std::string & to):
  impl(std::unique_ptr<Impl>(new Impl(from,to))) { }

IConv::IConv() {}

std::string IConv::operator()(const std::string & s) const {
  return impl? impl->cnv(s) : s;
}


IConv::~IConv(){}


std::string
IConv::Impl::cnv(const std::string & s) const{

  if ((iconv_t)(cdp.get()) == ERR) return s;
  std::string ret;

  const size_t ISIZE = s.length();
  const size_t OSIZE = 512;
  char obuf[OSIZE + 1];

  const char *ibuf = s.data();
  char *ibuf_ptr = (char *)ibuf;
  size_t icnt = ISIZE;

  while(icnt){
    char *obuf_ptr = obuf;
    size_t ocnt = OSIZE;
    size_t res = iconv((iconv_t)(cdp.get()), &ibuf_ptr, &icnt, &obuf_ptr, &ocnt);

    if (( res == -1) && (errno != E2BIG ) && (icnt>0) && (ocnt>0)){
      // skip unknown char
      icnt--; ocnt--;  *obuf_ptr=*ibuf_ptr; ibuf_ptr++; obuf_ptr++;
    }
    // note: in UTF-16 string can contain zeros!
    ret += std::string(obuf, obuf+OSIZE-ocnt);
  }
  return ret;
}
