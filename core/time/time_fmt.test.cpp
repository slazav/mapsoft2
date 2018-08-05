///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "time_fmt.h"
#include "err/err.h"

main(){
  try{
     assert(time_utc_iso8601(0)              == "1970-01-01T00:00:00Z");
     assert(time_utc_iso8601(1533473911000)  == "2018-08-05T12:58:31Z");
     assert(time_utc_iso8601(1533473911001)  == "2018-08-05T12:58:31.001Z");
     assert(time_utc_iso8601(1533473911123)  == "2018-08-05T12:58:31.123Z");
     assert(time_utc_iso8601(15334739110000) == "2455-12-09T09:45:10Z");

     try { parse_utc_iso8601(""); }
     catch (Err e) {assert (e.str() == "Unsupported time format: \"\"");}

     try { parse_utc_iso8601("1970-01-01"); }
     catch (Err e) {assert (e.str() == "Unsupported time format: \"1970-01-01\"");}

     try { parse_utc_iso8601("1970-01-01T00:00:00Z a"); }
     catch (Err e) {assert (e.str() == "Unsupported time format: \"1970-01-01T00:00:00Z a\"");}


     assert(parse_utc_iso8601("1970-01-01T00:00:00Z") == 0);
     assert(parse_utc_iso8601("2018-08-05T12:58:31Z") == 1533473911000);
     assert(parse_utc_iso8601("2018-08-05T12:58:31.001Z") == 1533473911001);
     assert(parse_utc_iso8601("2018-08-05T12:58:31.123Z") == 1533473911123);
     assert(parse_utc_iso8601("2455-12-09T09:45:10Z") == 15334739110000);

     assert(parse_utc_iso8601(time_utc_iso8601(0)) == 0);
     assert(parse_utc_iso8601(time_utc_iso8601(123456789)) == 123456789);
     assert(parse_utc_iso8601(time_utc_iso8601(123456789000)) == 123456789000);

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond
