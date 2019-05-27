#include <iostream>
#include "gobj_test_grid.h"
#include "viewer/simple_viewer.h"
#include "viewer/rubber.h"



int main(int argc, char **argv){

    Gtk::Main     kit (argc, argv);
    Gtk::Window   win;
    GObjTestGrid  p1;

    SimpleViewer viewer(&p1);
    Rubber rubber(&viewer);

    iPoint p(20,20);
    rubber.add_src_sq(p, 3);
    rubber.add_dst_sq(3);
    rubber.add_rect(p);
    rubber.add_line(p);

    win.add(viewer);
    win.set_default_size(640,480);
    win.show_all();

    kit.run(win);
}
