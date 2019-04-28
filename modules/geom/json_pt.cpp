#include <string>
#include "point.h"
#include "line.h"
#include "multiline.h"
#include "err/err.h"
#include <jansson.h>
/// functions for reading json lines, multilines
/// 

/**********************************************************/

dPoint
json_to_point(json_t *P) {
  if (!json_is_array(P) || (json_array_size(P)!=2 && json_array_size(P)!=3))
    throw Err() << "Reading Point: a JSON two-element array is expected";
  json_t *X = json_array_get(P, 0);
  json_t *Y = json_array_get(P, 1);
  if (!X || !Y || !json_is_number(X) || !json_is_number(Y))
    throw Err() << "Reading Point: a numerical values expected";
  double z=0;

  if (json_array_size(P)==3){
    json_t *Z = json_array_get(P, 2);
    if (!Z || !json_is_number(Z))
      throw Err() << "Reading Point: a numerical values expected";
    z = json_number_value(Z);
  }
  return dPoint(json_number_value(X), json_number_value(Y),z);
}

dLine
json_to_line(json_t *J) {
  if (!json_is_array(J))
    throw Err() << "Reading Line: a JSON array is expected";
  dLine ret;
  json_t *P;
  size_t index;
  json_array_foreach(J, index, P){
    ret.push_back(json_to_point(P));
  }
  return ret;
}

dMultiLine
json_to_mline(json_t *J) {
  if (!json_is_array(J))
    throw Err() << "Reading MultiLine: a JSON array is expected";

  dMultiLine ret;
  json_t *L;
  size_t index;
  json_array_foreach(J, index, L){
    ret.push_back(json_to_line(L));
  }
  return ret;
}


/**********************************************************/

dPoint string_to_point(const std::string & str){
  dPoint ret;
  json_error_t e;
  json_t *J = json_loadb(str.data(), str.size(), 0, &e);

  try {
    if (!J) throw Err() << e.text;
    ret=json_to_point(J);
  }
  catch (Err e){
    json_decref(J);
    throw e;
  }
  json_decref(J);
  return ret;
}

dLine string_to_line(const std::string & str){
  dLine ret;
  json_error_t e;
  json_t *J = json_loadb(str.data(), str.size(), 0, &e);

  try {
    if (!J) throw Err() << e.text;
    ret=json_to_line(J);
  }
  catch (Err e){
    json_decref(J);
    throw e;
  }
  json_decref(J);
  return ret;
}

dMultiLine string_to_mline(const std::string & str){
  dMultiLine ret;
  json_error_t e;
  json_t *J = json_loadb(str.data(), str.size(), 0, &e);

  try {
    if (!J) throw Err() << e.text;
    ret=json_to_mline(J);
  }
  catch (Err e){
    json_decref(J);
    throw e;
  }
  json_decref(J);
  return ret;
}
