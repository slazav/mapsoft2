#ifndef TIME_FMT_H
#define TIME_FMT_H

#include <string>
#include <ctime>

///\addtogroup libmapsoft
///@{

/// In mapsoft unix time in ms is used.

///\defgroup TimeFmt different time formats
///@{

/// Format mapsoft time (1000*unix_time) for gpx file.
/// UTC, ISO 8601, fractional seconds allowed
/// Example: 2011-09-27T18:50:03.123Z

/// Convert mapsoft time to UTC time represented as ISO 8601 string
/// Examples:
///  0 -> "1970-01-01T00:00:00Z"
///  1533473911000 -> "2018-08-05T12:58:31Z"
///  1533473911001 -> "2018-08-05T12:58:31.001Z"
std::string time_utc_iso8601(const time_t t);

/// Convert UTC time represented as ISO 8601 string (used in GPX) to a mapsoft time.
time_t parse_utc_iso8601(const std::string & str);

/// Convert mapsoft time to Ozi format (fractional number of days since 12/30/1899 12:00AM GMT).
/// Note that in Ozi time accuracy is 1e-7 * 3600*24 = 8.6 ms.
/// Conversion to Ozi and then back will be accurate within this limit.
std::string time_ozi(const time_t t);

/// Convert time to Ozi format to mapsoft time.
time_t parse_ozi(const std::string & str);

///@}
///@}
#endif

