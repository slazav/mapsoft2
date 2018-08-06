///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "time_fmt.h"
#include "err/err.h"

main(){
  try{
     assert(write_utc_iso_time(0)              == "1970-01-01T00:00:00Z");
     assert(write_utc_iso_time(1533473911000)  == "2018-08-05T12:58:31Z");
     assert(write_utc_iso_time(1533473911001)  == "2018-08-05T12:58:31.001Z");
     assert(write_utc_iso_time(1533473911123)  == "2018-08-05T12:58:31.123Z");
     assert(write_utc_iso_time(15334739110000) == "2455-12-09T09:45:10Z");

     assert(parse_utc_time("2018/08/05	12:58:31.1") == 1533473911100);


     try { parse_utc_time(""); }
     catch (Err e) {assert (e.str() == "Unsupported time format: \"\"");}

     try { parse_utc_time("1970-01-01"); }
     catch (Err e) {assert (e.str() == "Unsupported time format: \"1970-01-01\"");}

     try { parse_utc_time("1970-01-01T00:00:00Z a"); }
     catch (Err e) {assert (e.str() == "Unsupported time format: \"1970-01-01T00:00:00Z a\"");}


     assert(parse_utc_time("1970-01-01T00:00:00Z") == 0);
     assert(parse_utc_time("2018-08-05T12:58:31Z") == 1533473911000);
     assert(parse_utc_time("2018-08-05T12:58:31.001Z") == 1533473911001);
     assert(parse_utc_time("2018-08-05T12:58:31.123Z") == 1533473911123);
     assert(parse_utc_time("2455-12-09T09:45:10Z") == 15334739110000);

     assert(parse_utc_time("2018/08/05T12:58:31") == 1533473911000);
     assert(parse_utc_time("2018-08-05 12:58:31Z") == 1533473911000);
     assert(parse_utc_time("2018-08-05 12:58:31") == 1533473911000);
     assert(parse_utc_time("2018-08-05 12:58:31 ") == 1533473911000);
     assert(parse_utc_time("2018/08/05 12:58:31 	") == 1533473911000);
     assert(parse_utc_time("2018/08/05	12:58:31 ") == 1533473911000);
     assert(parse_utc_time("2018/08/05	12:58:31.001 ") == 1533473911001);
     assert(parse_utc_time("2018/08/05	12:58:31.1") == 1533473911100);

     assert(parse_utc_time(write_utc_iso_time(0)) == 0);
     assert(parse_utc_time(write_utc_iso_time(123456789)) == 123456789);
     assert(parse_utc_time(write_utc_iso_time(123456789000)) == 123456789000);

     assert( write_ozi_time(-2209161600000) == "0.0000000");
     assert( write_ozi_time(0) == "25569.0000000");
     assert( write_ozi_time(1533473911001) == "43317.5406366");
     assert( write_ozi_time(15334739115959) == "203054.4064347");

     assert( parse_ozi_time(write_ozi_time(-2209161600000)) == -2209161600000);
     assert( parse_ozi_time(write_ozi_time(0)) == 0);

     // Accuracy of ozi format: 1e-7 * 3600*24 = 8.6ms
     assert( abs(parse_ozi_time(write_ozi_time(1533473911001)) - 1533473911001) < 10);
     assert( abs(parse_ozi_time(write_ozi_time(15334739115959)) - 15334739115959) < 10);


  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond
