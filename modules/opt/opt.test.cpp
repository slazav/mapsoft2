///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <sstream>
#include "opt.h"
#include "err/assert_err.h"

int
main(){
try{

  Opt O1;
  O1.put("int", 123);
  assert ( O1.get<int>("int") == 123 );
  assert ( O1.get<std::string>("int") == "123" );

  O1.put("int", "123a");
  assert_err(
    O1.get<int>("int"), "can't parse value: 123a");

  O1.put("d", "123.1 ");
  assert_err(
    O1.get("d", 1.0), "can't parse value: 123.1 ");

  assert(O1.exists("d") == true);
  assert(O1.exists("e") == false);
  assert(O1.find("d")->second == "123.1 ");

  O1.put<std::string>("d", "1 2 3 4 5");
  assert(O1.get<std::string>("d", "1 2 3") == "1 2 3 4 5");
  assert(O1.get<std::string>("x", "1 2 3") == "1 2 3");

  assert(O1.get("d", "1 2 3") == "1 2 3 4 5");
  assert(O1.get("x", "1 2 3") == "1 2 3");


  O1.put("d", "123.1 ");

  /////////////////////////////////////////////
  // check_unknown()
  std::list<std::string> k;

  k = {"int","d","a"};
  O1.check_unknown(k);

  k = {"a", "b"};
  assert_err(O1.check_unknown(k), "unknown options: d, int")

  k = {"a", "b", "d"};
  assert_err(O1.check_unknown(k), "unknown option: int");

  /////////////////////////////////////////////
  // check_conflict()
  k = {"int","b"};
  O1.check_conflict(k);
  k = {"b","c"};
  O1.check_conflict(k);

  k = {"a", "int", "d"};
  assert_err(O1.check_conflict(k), "options can not be used together: int, d");

  /////////////////////////////////////////////
  // dump and parse simple options:

  // put Opt inside Opt!
  O1.put("opts", O1);

  std::ostringstream os;
  os << O1;
  assert(os.str() == "{\"d\": \"123.1 \", \"int\": \"123a\","
                     " \"opts\": \"{\\\"d\\\": \\\"123.1 \\\", \\\"int\\\": \\\"123a\\\"}\"}");

  std::istringstream is(os.str());
  Opt O2;
  O2.put("a", 1); // these fields should disappear
  O2.put("d", 2);
  is >> O2; // read O2 from is
  os.str(std::string()); // clear the stream
  os << O2;
  assert(os.str() == "{\"d\": \"123.1 \", \"int\": \"123a\","
                     " \"opts\": \"{\\\"d\\\": \\\"123.1 \\\", \\\"int\\\": \\\"123a\\\"}\"}");

  os.str(std::string()); // clear the stream
  os << O2.get<Opt>("opts");
  assert(os.str() == "{\"d\": \"123.1 \", \"int\": \"123a\"}");

  {
    std::istringstream is("{} ");
    is >> O1;
    assert(O1.size() == 0);
  }

  // some error cases
  {
    std::istringstream is("[1,2,3]");
    assert_err(is >> O1, "Reading Opt: a JSON object with string fields expected");
  }

  {
    std::istringstream is("{");
    assert_err(is >> O1, "JSON error: string or '}' expected near end of file");
  }

  {
    std::istringstream is("{a: 1}");
    assert_err(is >> O1, "JSON error: string or '}' expected near 'a'");
  }

  {
    std::istringstream is("{b: \"2\"}");
    assert_err(is >> O1, "JSON error: string or '}' expected near 'b'");
  }

  O1.put("h1", "0xFF");
  O1.put_hex("h2", 254);
  assert(O1.get("h1", std::string()) == "0xFF");
  assert(O1.get("h2", std::string()) == "0xfe");

  assert(O1.get("h1", 0) == 255);
  assert(O1.get("h2", 0) == 254);

  Opt O3;
  O3.put("h1", 123);
  O3.put("h3", 124);
  O1.put(O3);
  assert(O1.get<int>("h1") == 123);
  assert(O1.get<int>("h2") == 254);
  assert(O1.get<int>("h3") == 124);
  assert(O1.get<int>("h4") == 0);

  Opt O4("{\"k1\":\"v1\", \"k2\":\"v2\", \"k3\":\"100\"}");
  assert(O4.get("k1", std::string()) == "v1");
  assert(O4.get("k2", std::string()) == "v2");
  assert(O4.get("k3", 0) == 100);

  O1.put("hex", "0xFFFFFFFF"); // unsigned int -> int
  assert(O1.get("hex",0) == 0xFFFFFFFF);

  O1.put("hex", "0xFFFFFFFFF"); // to long
  assert_err(O1.get("hex",0), "can't parse value: 0xFFFFFFFFF");

//  assert(O1.get("h1", 0.0) == 255);
//  assert(O1.get("h2", 0.0) == 254);

}
catch (Err e) {
  std::cerr << "Error: " << e.str() << "\n";
  return 1;
}
return 0;
}

///\endcond
