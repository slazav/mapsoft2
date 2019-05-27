#include <sstream>
#include <iomanip>
#include <cmath>
#include "time_fmt.h"
#include "err/err.h"

using namespace std;

string
write_fmt_time(const char *fmt, const time_t t){
  time_t s  = t/1000;
  time_t ms = t%1000;
  struct tm ts;
  gmtime_r(&s, &ts);
  ostringstream str;
  str << setfill('0');

  const char* months_short[12] = {
     "Jan", "Feb", "Mar", "Apr", "May", "Jun",
     "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

  const char* wdays_short[7] = {
     "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

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

        case 'a': str << wdays_short[ts.tm_wday % 7]; break;
        case 'b': str << months_short[ts.tm_mon % 12]; break;

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
  ts.tm_year = 1970;
  ts.tm_mon  = 1;
  ts.tm_mday = 1;
  ts.tm_hour = 0;
  ts.tm_min  = 0;
  ts.tm_sec  = 0;
  int sh_h = 0; // time shift, h
  int sh_m = 0; // time shift, min
  int sh_s = 1; // shift sign, +1/-1
  try {
    ss >> noskipws >> ws;
    ss >> ts.tm_year;
    if (ss.eof()) goto end;

    ss >> sep >> ts.tm_mon;
    if (sep!='-' && sep!='/') throw 1;
    if (ss.eof()) goto end;

    ss >> sep >> ts.tm_mday;
    if (sep!='-' && sep!='/') throw 2;
    if (ss.eof()) goto end;

    ss >> sep >> ts.tm_hour;
    if (sep!='T' && sep!=' ' && sep!='\t') throw 3;
    if (ss.eof()) goto end;

    ss >> sep >> ts.tm_min;
    if (sep!=':') throw 4;
    if (ss.eof()) goto end;

    ss >> sep >> ts.tm_sec;
    if (sep!=':') throw 5;
    if (ss.eof()) goto end;

    // read milliseconds, Z or spaces:
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
      if (ss.eof()) goto end;
    }
    if (sep=='+' || sep=='-') {
      sh_s = (sep=='+'? +1:-1);
      ss >> sh_h;
      if (ss.eof()) goto end;
      ss >> sep >> sh_m;
      if (sep!=':' || sh_h<0 || sh_h>24 || sh_m<0 || sh_m>59)
          throw 6;
      if (ss.eof()) goto end;
    }
    else if (sep == 'Z' || sep==' ') {
    }
    else throw 7;

    ss >> ws;
    // here should be the end:
    if (!ss.eof()) throw 8;

    end:

    ts.tm_year-=1900;
    ts.tm_mon-=1;
    if (ms<0  || ms>999)  throw 9;
    // mktime() converts _local_ time to unix seconds!
    ts.tm_isdst = 1;
    time_t t0 = timegm(&ts);
    if (t0 == -1) throw 10;
    return t0*1000 - sh_s*(sh_h*3600 + sh_m*60)*1000 + ms;
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
