#include "viewer/action_manager.h"
#include "viewer/dthread_viewer.h"
#include "viewer/rubber.h"

#include "gobj_test_tile.h"
#include "gobj_test_grid.h"

class TestWin : public Gtk::Window{
  public:

  GObjTestTile      o1, o2;
  GObjTestGrid      o3;

  SimpleViewer *v;
  SimpleViewer v1;
  DThreadViewer v2;
  Rubber rubber;


  TestWin(): o3(150000), o2(true), v1(&o1), v2(&o2), v(&v2), rubber(v){
    signal_key_press_event().connect (sigc::mem_fun (this, &TestWin::on_key_press));

    add(*v);
    set_default_size(640,480);
    show_all();
    std::cerr <<
      "Viewer Test\n"
      " 's' - SimpleViewer\n"
      " 'd' - DThreadViewer\n"
      " 'c' - Color gradient object\n"
      " 'g' - Grid object\n"
      " 's' - Refresh\n"
      " 'q' - Quit\n";
 }

  bool on_key_press(GdkEventKey * event) {
     std::cerr << "key_press: " << event->keyval << "\n";
     switch (event->keyval) {
       case 'r': case 'R': v->redraw(); return true;
       case 'q': case 'Q': hide();  return true;
       case 's': case 'S': change_viewer(&v1); return true;
       case 'd': case 'D': change_viewer(&v2); return true;
       case 'c': case 'C': v->set_obj(&o2);   return true;
       case 'g': case 'G': v->set_obj(&o3);   return true;
     }
     return false;
  }

  void change_viewer(SimpleViewer *v1){
    if (!v1) return;
    v->redraw();
    v=v1;
    remove();
    add(*v);
    show_all();
    v->redraw();
  }
};



int main(int argc, char **argv){

    Gtk::Main     kit (argc, argv);
    TestWin       win;

    kit.run(win);
}
