#include "line_walker.h"

LineWalker::LineWalker(const dLine & _line) {
  line = _line.flatten();
  current_l=0;
  current_n=0;

  double l=0;
  ls.push_back(0);
  for (int j=1; j<line.size(); j++){
    dPoint p1 (line[j-1]);
    dPoint p2 (line[j]);
    l+=len(p1-p2);
    ls.push_back(l);
  }
}

double
LineWalker::length() const {
  return ls[ls.size()-1];
}


dPoint
LineWalker::pt() const{
  if (is_end()) return line[current_n];
  else return (line[current_n] + (line[current_n+1]-line[current_n]) *
               (current_l-ls[current_n])/(ls[current_n+1]-ls[current_n]) );
}

double
LineWalker::dist() const {
  return current_l;
}

dPoint
LineWalker::tang() const{
  if (is_end()) return dPoint(1,0);
  return ::norm(line[current_n+1]-line[current_n]);
}

dPoint
LineWalker::norm() const{
  dPoint ret = tang();
  return dPoint(-ret.y, ret.x);
}

dLine
LineWalker::get_points(double dl){
  dLine ret;
  if (dl <= 0) return ret;

  double l = current_l + dl;

  // add the first point
  ret.push_back(pt());
  if (is_end()) return ret;

  while (current_l < l){
    if (is_end()) return ret;
    move_frw_to_node();
    // add next node
    ret.push_back(line[current_n]);
  }
  // the last node is too far, move it
  current_n--;
  current_l = l;
  *ret.rbegin() = pt();
  return ret;
}

void
LineWalker::move_begin(){
  current_l = 0;
  current_n = 0;
}

void
LineWalker::move_end(){
  current_n = ls.size()-1;
  current_l = ls[current_n];
}

void
LineWalker:: move_frw(double dl){
  if (dl < 0) {move_bck(-dl); return;}
  if (dl == 0) return;
  double l = current_l + dl;
  while (current_l < l){
    if (is_end()) return;
    move_frw_to_node();
  }
  current_n--;
  current_l=l;
}

void
LineWalker::move_bck(double dl){
  if (dl < 0) {move_frw(-dl); return;}
  if (dl == 0) return;
  double l = current_l - dl;
  while (current_l > l){
    if (current_n == 0) return;
    move_bck_to_node();
  }
  current_l=l;
}

void
LineWalker::move_frw_to_node(){
  if (current_n == ls.size()-1) return;
  current_n++;
  current_l=ls[current_n];
}

void
LineWalker::move_bck_to_node(){
  if ((current_l==ls[current_n]) && (current_n!=0))
    current_n--;
  current_l=ls[current_n];
}

bool
LineWalker::is_end() const{
  return (current_n == ls.size()-1);
}

bool
LineWalker::is_begin() const{
  return (current_n == 0);
}

