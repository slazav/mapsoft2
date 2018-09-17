#ifndef OPT_H
#define OPT_H

#include <map>
#include <string>
#include <sstream>
#include <list>
#include "err/err.h"

///\addtogroup libmapsoft
///@{

/// Convert std::string to any type (similar to boost::lexical_cast).
/// \relates Opt
template<typename T>
T str_to_type(const std::string & s){
  std::istringstream ss(s);
  T val;
  ss >> std::showbase >> val;
  if (ss.fail() || !ss.eof())
    throw Err() << "can't parse value: " << s;
  return val;
}

// version for std::string, much simplier
template<>
std::string str_to_type<std::string>(const std::string & s);

// version for int, supports HEX values (starting with 0x)
template<>
int str_to_type<int>(const std::string & s);

/// Convert any type to std::string (similar to boost::lexical_cast).
/// \relates Opt
template<typename T>
std::string type_to_str(const T & t){
  std::ostringstream ss;
  ss << t;
  return ss.str();
}

/// version for hex values
/// \relates Opt
template<typename T>
std::string type_to_str_hex(const T & t){
  std::ostringstream ss;
  ss << std::hex << std::showbase << t;
  return ss.str();
}


/// version for std::string, much simplier
template<>
std::string type_to_str<std::string>(const std::string & t);

/***********************************************************/
/** Mapsoft options

Opt class is a `map<string,string>` container with functions for
getting/putting values of arbitrary types. Data types should have `<<`,
`>>` operators and a constructor without arguments.

Example:
```
Opt o;

// put key1=10, type is determined by value
o.put("key1", 10);

// put key2=10, explicitely specify the type
o.put<int>("key2", 10);

// get key2, default value is 1.0
double k2 = o.get<double>("key2", 1.0);

// get key2, type is determined by default value
double k2 = o.get("key2", 1.0);
```

For the Opt class operators `<<` and `>>` are also defined. String representations
is a JSON object with string fields only. It is possible to pack Opt inside Opt,
it will be represented like this:
```
{"key1": "val1", "key2": "val2", "key3": "{\"k1\": \"v1\", \"k2\":\"v2\"}"}
```
*/

class Opt : public std::map<std::string,std::string>{
  public:

  /// Set option value for a given key.
  template<typename T>
  void put (const std::string & key, const T & val) {
    (*this)[key] = type_to_str(val);
  }

  /// Set option value for a given key (hex version).
  template<typename T>
  void put_hex (const std::string & key, const T & val) {
    (*this)[key] = type_to_str_hex(val);
  }

  /// Returns value for a given key.
  /// If option does not exists or cast fails then default value is returned.
  template<typename T>
  T get (const std::string & key, const T & def = T()) const {
    std::map<std::string, std::string>::const_iterator it = find(key);
    if (it == end()) return def;
    return str_to_type<T>(it->second);
  }

  /// Check if option exists.
  bool exists (const std::string & key) const {return find(key) != end();}

  /// Find unknown options.
  void check_unknown (std::list<std::string> known) const;
};


/// Print options as a JSON object with text fields.
/// \relates Opt
std::ostream & operator<< (std::ostream & s, const Opt & o);

/// Read options from a JSON object with text fields.
/// \relates Opt
/// \note This >> operator is different from that in
/// Point or Rect. It always reads the whole stream and
/// returns error if there are extra characters.
/// No possibility to read two Opt objects from one stream.
std::istream & operator>> (std::istream & s, Opt & o);

///@}
#endif
