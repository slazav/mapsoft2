///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <sstream>
#include "opt.h"

int
main(){
try{
  Opt O1;
  O1.put("int", 123);
  assert ( O1.get<int>("int") == 123 );
  assert ( O1.get<std::string>("int") == "123" );

  O1.put("int", "123a");
  try {
    O1.get<int>("int");
  }
  catch (Err E){
    assert (E.str() == "can't parse value: 123a");
  }

  O1.put("d", "123.1 ");
  try {
    O1.get("d", 1.0);
  }
  catch (Err E){
    assert (E.str() == "can't parse value: 123.1 ");
  }

  assert(O1.exists("d") == true);
  assert(O1.exists("e") == false);
  assert(O1.find("d")->second == "123.1 ");

  O1.put<std::string>("d", "1 2 3 4 5");
  assert(O1.get<std::string>("d", "1 2 3") == "1 2 3 4 5");
  assert(O1.get<std::string>("x", "1 2 3") == "1 2 3");

  O1.put("d", "123.1 ");

  /////////////////////////////////////////////
  // check_unknown()
  try {
    std::list<std::string> k = {"int","d","a"};
    O1.check_unknown(k);
  }
  catch (Err e) {
    std::cerr << "Error:" << e.str() << "\n";
    assert(false);
  }

  try {
    std::list<std::string> k = {"a", "b"};
    O1.check_unknown(k);
    assert(false);
  }
  catch (Err e) {
    assert(e.str() == "unknown options: d, int");
  }

  try {
    std::list<std::string> k = {"a", "b", "d"};
    O1.check_unknown(k);
    assert(false);
  }
  catch (Err e) {
    assert(e.str() == "unknown option: int");
  }
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
  try {
    std::istringstream is("[1,2,3]");
    is >> O1;
  }
  catch (Err e) {
    assert(e.str() == "Reading Opt: a JSON object with string fields expected");
  }

  try {
    std::istringstream is("{");
    is >> O1;
  }
  catch (Err e) {
    assert(e.str() == "string or '}' expected near end of file");
  }

  try {
    std::istringstream is("{a: 1}");
    is >> O1;
  }
  catch (Err e) {
    assert(e.str() == "string or '}' expected near 'a'");
  }

  try {
    std::istringstream is("{b: \"2\"}");
    is >> O1;
  }
  catch (Err e) {
    assert(e.str() == "string or '}' expected near 'b'");
  }

  O1.put("h1", "0xFF");
  O1.put_hex("h2", 254);
  assert(O1.get("h1", std::string()) == "0xFF");
  assert(O1.get("h2", std::string()) == "0xfe");

  assert(O1.get("h1", 0) == 255);
  assert(O1.get("h2", 0) == 254);

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
