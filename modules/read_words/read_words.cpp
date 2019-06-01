#include "err/err.h"
#include "read_words.h"

std::vector<std::string> read_words(
      std::istream & ss, int line_num[2], const bool lc) {

  bool first = true;
  std::string str;
  std::vector<std::string> ret;
  bool quote1=false, quote2=false, comment=false;

  // read stream char by char
  for (char c; ss.get(c); !ss.eof()){

    if (c == '\n' && line_num) line_num[1]++;

    // end of line
    if (c == '\n' && !quote1 && !quote2) {
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
      if (first && line_num) {line_num[0] = line_num[1]+1; first=false;}
      if (c == '\n' && line_num) line_num[1]++;
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
    if (first && line_num) {line_num[0] = line_num[1]+1; first=false;}
    str += lc ? tolower(c):c;
  }

  if (quote1 || quote2){
    Err e;
    e << "Unclosed quote";
    if (line_num) e << " in line " << line_num[0];
    throw e;
  }
  if (str!="") ret.push_back(str);
  return ret;
}
