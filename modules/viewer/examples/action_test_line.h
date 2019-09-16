#include <iostream>
#include "geom/line.h"
#include "viewer/action.h"
#include "viewer/rubber.h"


class ActionTestLine : public Action{
  public:
  ActionTestLine(Rubber * r) : rubber(r) { }

  std::string get_name() { return "TestLine"; }

  void init() {
    std::cout << "Test action for drawing polylines.\n"
              << " * left button: add points\n"
              << " * control + left button: remove last point\n"
              << " * middle button: finish line, print it to stdout\n"
              << " * right button:  abort action\n";
  }

  void reset() {
    rubber->clear();
    data.clear();
  }

  void click(const iPoint & p, const Gdk::ModifierType & state){

    if (state&Gdk::BUTTON1_MASK){
      if (state&Gdk::CONTROL_MASK){

        if (data.size()>0) data.pop_back();

        if (rubber->size()>0){
          rubber->pop();
        }
        if (rubber->size()>0){
          RubberSegment s = rubber->pop();
          s.flags |= RUBBFL_MOUSE_P2;
          s.p2=iPoint(0,0);
          rubber->add(s);
        }
      }
      else{
        if (rubber->size()>0){
          RubberSegment s = rubber->pop();
          s.flags &= ~RUBBFL_MOUSE;
          s.p2 = p;
          rubber->add(s);
        }
        rubber->add_line(p);
        data.push_back(p);
      }
    }
    else if (state&Gdk::BUTTON2_MASK){
      data.push_back(p);
      std::cout << data << "\n";
      data.clear();
      rubber->clear();
    }
    else if (state&Gdk::BUTTON3_MASK){
      data.clear();
      rubber->clear();
    }
  }
  Rubber * rubber;
  iLine data;
  bool clear;
};
