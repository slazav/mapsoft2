#include <iostream>
#include "gobj_test_tile.h"
#include "viewer/simple_viewer.h"
#include "viewer/rubber.h"


int main(int argc, char **argv){

    Gtk::Main     kit (argc, argv);
    Gtk::Window   win;
    GObjTestTile  p1;

    SimpleViewer viewer(&p1);
    Rubber rubber(&viewer);

    iPoint p(20,20);
    rubber.add_sq_mark(iPoint(0,0), true, 3);
    rubber.add_sq_mark(p, false, 3);
    rubber.add_rect(p);
    rubber.add_line(p);

//    rubber.add_ell(p);
    rubber.add_ellc(p);
//    rubber.add_circ(p);
//    rubber.add_circc(p);

    win.add(viewer);
    win.set_default_size(640,480);
    win.show_all();

    kit.run(win);
}
