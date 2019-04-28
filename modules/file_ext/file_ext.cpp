#include <string>
#include <cstring>

bool
file_ext(const std::string &fname, const char *ext){
  int lf=fname.size();
  int le=strlen(ext);
  return (lf>=le) && (strncasecmp(fname.c_str() + (lf-le), ext, le)==0);
}
