#include <cstring>
#include <string>
#include <vector>

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

#include <iostream>

std::vector<std::string>
file_get_dirs(const std::string &fname){
  std::vector<std::string> ret;
  std::string s = fname;
  std::string tail;

  while (1) {
    int i = s.rfind('/');
    if (i<=0) return ret;
    s = std::string(s.begin(), s.begin()+i);

    i = s.rfind('/');
    tail = std::string(s.begin()+i+1, s.end());
    if (tail!="." && tail!=".." && tail!="")
      ret.push_back(s);
  };
  return ret;
}

std::string
file_get_prefix(const std::string &fname){
  int i = fname.rfind('/');
  return i<0? "" : std::string(fname.begin(), fname.begin()+i+1);
}
