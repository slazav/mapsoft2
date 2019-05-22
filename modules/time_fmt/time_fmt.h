#ifndef TIME_FMT_H
#define TIME_FMT_H

#include <string>
#include <ctime>

///\addtogroup libmapsoft
///@{

/// In mapsoft unix time in ms is used.

///\defgroup TimeFmt different time formats
///@{

/** Format time
Supported format sequences:
 %%  a literal %
 %n  a newline
 %t  a tab

 %Y  year
 %y  last two digits of year (00..99)
 %m  month (01..12)
 %d  day of month (e.g., 01)
 %H  hour (00..23)
 %M  minute (00..59)
 %S  second (00..60)
 %F  same as %Y-%m-%d
 %T  same as %H:%M:%S

 %s  seconds since 1970-01-01 00:00:00 UTC
 %f  fractional part of a second it it is non-zero (non-standard)
*/

std::string write_fmt_time(const char *fmt, const time_t t);

/// Convert UTC time to unix milliseconds (used in mapsoft).
/// Time can be represented in different forms including ISO 8601 (used in GPX)
/// and "yyyy-mm-dd HH:MM:SS" format
/// Examples:
/// "2018-08-05T12:58:31Z"
/// "2018-08-05 12:58:31"
/// "2018/08/05 12:58:31"
/// "2018-08-05 12:58:31.1"
/// "2018-08-05 12:58:31.002"
/// "2018-08-05 12:58:31.002Z"
time_t parse_utc_time(const std::string & str);

/// Convert mapsoft time to Ozi format (fractional number of days since 12/30/1899 12:00AM GMT).
/// Note that in Ozi time accuracy is 1e-7 * 3600*24 = 8.6 ms.
/// Conversion to Ozi and then back will be accurate within this limit.
std::string write_ozi_time(const time_t t);

/// Convert Ozi time to mapsoft time.
time_t parse_ozi_time(const std::string & str);

///@}
///@}
#endif

