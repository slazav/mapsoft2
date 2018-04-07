#include <iconv.h>
#include "iconv.h"
#include <errno.h>
#include <iostream>
#include <cstring>
#include "err/err.h"

#define ERR (iconv_t)(-1)

void
IConv::copy(const IConv & other){
  cd = other.cd;
  (*refcounter)++;
  assert(*refcounter >0);
}

void
IConv::destroy(void){
  (*refcounter)--;
  if (*refcounter<=0){
    // do not throw exceptions in destructor...
    if (cd) iconv_close(cd);
    delete refcounter;
  }
}

IConv::IConv(const char *from, const char *to) {
  refcounter   = new int;
  *refcounter  = 1;
  cd = iconv_open(to, from);
  if (cd == ERR) throw Err() <<
    "can't do iconv conversion from " << from << " to " << to;
}

std::string
IConv::cnv(const std::string & s){

  if (cd == ERR) return s;
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
    size_t res = iconv(cd, &ibuf_ptr, &icnt, &obuf_ptr, &ocnt);

    if (( res == -1) && (errno != E2BIG ) && (icnt>0) && (ocnt>0)){
      // skip unknown char
      icnt--; ocnt--;  *obuf_ptr=*ibuf_ptr; ibuf_ptr++; obuf_ptr++;
    }
    // note: in UTF-16 string can contain zeros!
    ret += std::string(obuf, obuf+OSIZE-ocnt);
  }
  return ret;
}
