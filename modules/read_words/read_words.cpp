#include "err/err.h"
#include "read_words.h"

std::vector<std::string> read_words(std::istream & ss) {
  std::string str;
  std::vector<std::string> ret;
  bool quote1=false, quote2=false, comment=false;

  // read stream char by char
  for (char c; ss.get(c); !ss.eof()){

    // end of line
    if (c == '\n') {
      comment=false;
      if (str!="") ret.push_back(str);
      str="";
      if (ret.size()) return ret;
      else continue;
    }

    // comments -- until end of line
    if (c == '#' && !quote1 && !quote2) comment=true;
    if (comment) continue;

    // escape character (all chars including newline can be escaped!)
    if (c == '\\') {
      ss.get(c);
      str += c;
      continue;
    }

    // quote
    if (c == '"'  && !quote2) {quote1=!quote1; continue; }
    if (c == '\'' && !quote1) {quote2=!quote2; continue; }


    // space -- word separaters unless quoted
    if (!quote1 && !quote2 && (c == ' ' || c == '\t')){
       if (str!="") ret.push_back(str);
       str="";
       continue;
    }

    // append normal characters
    str+=c;
  }

  if (quote1 || quote2) throw Err() << "Unclosed quote";
  if (str!="") ret.push_back(str);
  return ret;
}
