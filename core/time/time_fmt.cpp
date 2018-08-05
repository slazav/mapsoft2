#include <sstream>
#include <iomanip>
#include "time_fmt.h"
#include "err/err.h"

using namespace std;

string
time_utc_iso8601(const time_t t){
  time_t s  = t/1000;
  time_t ms = t%1000;
  struct tm ts;
  gmtime_r(&s, &ts);
  ostringstream str;
  str << setfill('0')
      << setw(4) << ts.tm_year+1900 << '-'
      << setw(2) << ts.tm_mon+1 << '-'
      << setw(2) << ts.tm_mday  << 'T'
      << setw(2) << ts.tm_hour  << ':'
      << setw(2) << ts.tm_min   << ':'
      << setw(2) << ts.tm_sec;
  if (ms) str << "." << setfill('0') << setw(3) << ms;
  str << 'Z';
  return str.str();
}

time_t
parse_utc_iso8601(const string & str){
  istringstream ss(str);
  char sep;
  int ms = 0;
  struct tm ts;
  try {
    ss >> ws;
    ss >> ts.tm_year >> sep;  if (sep!='-') throw 1;
    ss >> ts.tm_mon  >> sep;  if (sep!='-') throw 2;
    ss >> ts.tm_mday >> sep;  if (sep!='T') throw 3;
    ss >> ts.tm_hour >> sep;  if (sep!=':') throw 4;
    ss >> ts.tm_min  >> sep;  if (sep!=':') throw 5;
    ss >> ts.tm_sec >> sep;
    if (sep == '.') ss >> ms >> sep;
    if (sep!='Z') throw 6;
    ss >> ws;
    if (ss.fail() || !ss.eof()) throw 7;

    ts.tm_year-=1900;
    ts.tm_mon-=1;
    if (ms<0  || ms>999)  throw 8;
    // mktime() converts local time to unix seconds!
    ts.tm_isdst = 1;
    time_t t0 = timegm(&ts);
    if (t0 == -1) throw 9;
//    t0 += ts.tm_gmtoff;
    return t0*1000 + ms;
  }
  catch (int i){
    throw Err() << "Unsupported time format: \"" << str << "\"";
  }
}
