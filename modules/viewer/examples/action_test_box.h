#include <iostream>
#include "viewer/action.h"
#include "viewer/rubber.h"

class ActionTestBox : public Action{
  public:
  ActionTestBox(Rubber * r) : rubber(r) { }

  std::string get_name() { return "TestBox"; }

  void init() {
    clear=true;
  }

  void reset() {
    rubber->clear();
    clear=true;
  }
  void click(const iPoint & p, const Gdk::ModifierType & state){
    if (clear){
      rubber->add_sq_mark(iPoint(0,0), true, 3);
      rubber->add_sq_mark(p, false, 3);
      rubber->add_rect(p);
      rubber->add_line(p);
    } else {
      rubber->clear();
    }
    clear=!clear;
  }
  Rubber * rubber;
  bool clear;
};
