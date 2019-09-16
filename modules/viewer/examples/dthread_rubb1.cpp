#include <iostream>
#include "gobj_test_grid.h"
#include "viewer/dthread_viewer.h"
#include "viewer/rubber.h"


int main(int argc, char **argv){

    Gtk::Main     kit (argc, argv);
    Gtk::Window   win;
    GObjTestGrid  p1(150000);

    DThreadViewer viewer(&p1);
    Rubber rubber(&viewer);

    iPoint p(20,20);
    rubber.add_sq_mark(iPoint(0,0), true);
    rubber.add_sq_mark(p, false);
    rubber.add_rect(p);
    rubber.add_line(p);

    win.add(viewer);
    win.set_default_size(640,480);
    win.show_all();

    kit.run(win);
}
