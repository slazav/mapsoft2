#include "opt.h"
#include <algorithm>
#include <jansson.h>

template<>
std::string str_to_type<std::string>(const std::string & s){ return s; }

template<>
int str_to_type<int>(const std::string & s){
  std::istringstream ss(s);
  int val; ss >> val;
  if (!ss.eof()){
    char c; ss>>c;
    if (val!=0 || c!='x')
      throw Err() << "can't parse value: " << s;
    ss >> std::hex >> val;
  }
  if (ss.fail() || !ss.eof())
    throw Err() << "can't parse value: " << s;
  return val;
}


template<>
std::string type_to_str<std::string>(const std::string & t){ return t; }

void
Opt::check_unknown (std::list<std::string> known) const {
  std::string unknown;
  int n=0;
  for (auto i : *this){
    if (std::find(known.begin(), known.end(), i.first) == known.end())
      unknown += (n++ ? ", ": " ") + i.first;
  }
  if (n){
    throw Err() << "unknown "
                << (n==1? "option:":"options:")
                << unknown;
  }
}

// input/output operators for options
std::ostream & operator<< (std::ostream & s, const Opt & o){
  json_error_t e;
  json_t *J = json_object();
  for (auto i: o){
    json_object_set(J, i.first.c_str(), json_string(i.second.c_str()));
  }
  char *ret = json_dumps(J, JSON_SORT_KEYS);
  json_decref(J);
  if (!ret) throw Err() << "can't write Opt object";
  s<<ret;
  free(ret);
  return s;
}

std::istream & operator>> (std::istream & s, Opt & o){

  // read the whole stream into a string
  std::ostringstream os;
  s>>os.rdbuf();
  std::string str=os.str();

  json_error_t e;
  json_t *J = json_loadb(str.data(), str.size(), 0, &e);

  o.clear(); // clear old contents
  try {
    if (!J)
      throw Err() << e.text;
    if (!json_is_object(J))
      throw Err() << "Reading Opt: a JSON object with string fields expected";

    const char *k;
    json_t *v;
    json_object_foreach(J, k, v){
      if (!json_is_string(v))
        throw Err() << "Reading Opt: a JSON object with string fields expected";
      o[k] = json_string_value(v);
    }
  }
  catch (Err e){
    json_decref(J);
    throw e;
  }
  json_decref(J);
  return s;
}
