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
  SimpleViewer *v1;
  DThreadViewer *v2;

  TestWin(): o3(150000), o2(true){
    v1 = new SimpleViewer(&o1);
    v2 = new DThreadViewer(&o2);
    signal_key_press_event().connect (sigc::mem_fun (this, &TestWin::on_key_press));
    v=(SimpleViewer *)v2;

    add(*v);
    set_default_size(640,480);
    show_all();
    std::cerr << "Viewer Test\n"
              << " '1' - SimpleViewer\n"
              << " '2' - DThreadViewer\n"
              << " '9' - Gradient object\n"
              << " '0' - Grid object\n"
              << " 'R' - Refresh\n"
              << " 'Q' - Quit\n";
 }

  bool on_key_press(GdkEventKey * event) {
     std::cerr << "key_press: " << event->keyval << "\n";
     switch (event->keyval) {
       case 'r':
       case 'R':
         v->redraw();
         return true;
       case 'q':
       case 'Q':
         hide();
       case '1':
         change_viewer(v1);
         return true;
       case '2':
         change_viewer(v2);
         return true;
       case '9':
         v->set_obj(&o2);
         return true;
       case '0':
         v->set_obj(&o3);
         return true;
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
