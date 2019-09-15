#include "viewer/action_manager.h"
#include "viewer/dthread_viewer.h"
#include "viewer/rubber.h"

#include "gobj_test_tile.h"
#include "gobj_test_grid.h"

class TestWin : public Gtk::Window{
  public:

  GObjTestTile  o1, o2;
  GObjTestGrid  o3;

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
      " '0' - Clear lines\n"
      " '1' - Line from mouse to the center\n"
      " '2' - Box from mouse to the center\n"
      " '3' - Ellipse\n"
      " '4' - Centered ellipse\n"
      " '5' - Circle\n"
      " '6' - Centered circle\n"
      " '7' - Square marks\n"
      " '8' - Cross-in-circle marks\n"
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
       case '0':
         rubber.clear();
         return true;
       case '1':
         rubber.clear();
         rubber.add_line(v->get_center());
         return true;
       case '2':
         rubber.clear();
         rubber.add_rect(v->get_center());
         return true;
       case '3':
         rubber.clear();
         rubber.add_ell(v->get_center());
         return true;
       case '4':
         rubber.clear();
         rubber.add_ellc(v->get_center());
         return true;
       case '5':
         rubber.clear();
         rubber.add_circ(v->get_center());
         return true;
       case '6':
         rubber.clear();
         rubber.add_circc(v->get_center());
         return true;
       case '7':
         rubber.clear();
         rubber.add_sq_mark(iPoint(0,0), true);
         rubber.add_sq_mark(v->get_center(), false);
         return true;
       case '8':
         rubber.clear();
         rubber.add_cr_mark(iPoint(0,0), true);
         rubber.add_cr_mark(v->get_center(), false);
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
