#include <string>
#include <cstring>

bool
file_ext_check(const std::string &fname, const char *ext){
  int lf=fname.size();
  int le=strlen(ext);
  return (lf>=le) && (strncasecmp(fname.c_str() + (lf-le), ext, le)==0);
}

std::string
file_ext_repl(const std::string &fname, const char *ext){
  int i = fname.rfind('.');
  std::string base = fname;
  if (i>=0) base = std::string(fname.begin(), fname.begin()+i);
  return base + ext;
}
