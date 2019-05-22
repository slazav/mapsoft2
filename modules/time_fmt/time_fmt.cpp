#include <sstream>
#include <iomanip>
#include <cmath>
#include "time_fmt.h"
#include "err/err.h"

using namespace std;

string
write_utc_iso_time(const time_t t){
  return write_fmt_time("%FT%T%fZ",t);
}

string
write_utc_time_s(const time_t t){
  return write_fmt_time("%F %T", t);
}

string
write_fmt_time(const char *fmt, const time_t t){
  time_t s  = t/1000;
  time_t ms = t%1000;
  struct tm ts;
  gmtime_r(&s, &ts);
  ostringstream str;
  str << setfill('0');

  bool f = false;
  for (const char *c = fmt; *c!=0; c++){
    if (f) {
      switch (*c){
        case 'Y': str << setw(4) << ts.tm_year+1900; break;
        case 'y': str << setw(2) << ts.tm_year%100; break;
        case 'm': str << setw(2) << ts.tm_mon+1; break;
        case 'd': str << setw(2) << ts.tm_mday; break;
        case 'H': str << setw(2) << ts.tm_hour; break;
        case 'M': str << setw(2) << ts.tm_min; break;
        case 'S': str << setw(2) << ts.tm_sec; break;

        case 'F': str << setw(4) << ts.tm_year+1900 << "-"
                      << setw(2) << ts.tm_mon+1 << "-"
                      << setw(2) << ts.tm_mday; break;
        case 'T': str << setw(2) << ts.tm_hour << ":"
                      << setw(2) << ts.tm_min << ":"
                      << setw(2) << ts.tm_sec; break;

        case '%': str << '%'; break;
        case 'n': str << '\n'; break;
        case 't': str << '\t'; break;
        case 's': str << setw(2) << s; break;
        case 'f': if (ms) str << "." << setw(3) << ms; break; // non-standard
      }
      f=false;
    }
    else {
      if (*c == '%') f = true;
      else str << *c;
    }
  }
  return str.str();
}


time_t
parse_utc_time(const string & str){
  istringstream ss(str);
  char sep;
  int ms = 0;
  struct tm ts;
  try {
    ss >> noskipws >> ws;
    ss >> ts.tm_year >> sep;  if (sep!='-' && sep!='/') throw 1;
    ss >> ts.tm_mon  >> sep;  if (sep!='-' && sep!='/') throw 2;
    ss >> ts.tm_mday >> sep;  if (sep!='T' && sep!=' ' && sep!='\t') throw 3;
    ss >> ts.tm_hour >> sep;  if (sep!=':') throw 4;
    ss >> ts.tm_min  >> sep;  if (sep!=':') throw 5;
    ss >> ts.tm_sec;
    // Allow end of string here. If not, read milliseconds,
    // Z or spaces:
    if (!ss.eof()){
      ss >> sep;
      // read milliseconds
      if (sep == '.'){
        int n=2;
        while (!ss.eof()){
          ss >> sep;
          if (sep<'0' || sep>'9' || ss.eof()) break;
          if (n>=0) ms += (sep-'0') * pow(10,n);
          n--;
        }
        if (ss.eof()) sep='Z';
      }
      if (sep!='Z' && sep!=' ') throw 6;
      // Again, allow end of string here.
      if (!ss.eof()){
        ss >> ws;
        // here should be the end:
        if (!ss.eof()) throw 7;
      }
    }
    if (!ss.eof()) throw 8;

    ts.tm_year-=1900;
    ts.tm_mon-=1;
    if (ms<0  || ms>999)  throw 8;
    // mktime() converts local time to unix seconds!
    ts.tm_isdst = 1;
    time_t t0 = timegm(&ts);
    if (t0 == -1) throw 9;
    return t0*1000 + ms;
  }
  catch (int i){
    throw Err() << "Unsupported time format: \"" << str << "\"";
  }
}


string
write_ozi_time(const time_t t){
  ostringstream str;
  str << fixed << setprecision(7)
      << (t/1000.0+2209161600.0)/3600.0/24.0;
  return str.str();
}

time_t
parse_ozi_time(const string & str){
  istringstream ss(str);
  double t;
  ss >> ws >> t >> ws;
  if (ss.fail() || !ss.eof())
    throw Err() << "Unsupported time format: \"" << str << "\"";

  return time_t((t*3600.0*24.0 - 2209161600.0)*1000);
}
