#include <iostream>
#include "viewer/dthread_viewer.h"
#include "gobj_test_grid.h"

// dthread viewer + slow grid object

int main(int argc, char **argv){

    Gtk::Main     kit (argc, argv);
    Gtk::Window   win;
    ConvBase      cnv;
    GObjTestGrid  pl(cnv, 150000);

    DThreadViewer viewer(&pl);

    win.add(viewer);
    win.set_default_size(640,480);
    win.show_all();

    kit.run(win);
}
