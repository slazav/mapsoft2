#include <iostream>
#include "viewer/action.h"
#include "viewer/rubber.h"

class ActionTestCirc : public Action{
  public:
  ActionTestCirc(Rubber * r) : rubber(r) { }

  std::string get_name() { return "TestCirc"; }

  void init() {
    clear=true;
  }

  void reset() {
    rubber->clear();
    clear=true;
  }
  void click(const iPoint & p, const Gdk::ModifierType & state){
    if (clear){
      rubber->add_circc(p);
    } else {
      rubber->clear();
    }
    clear=!clear;
  }
  Rubber * rubber;
  bool clear;
};
