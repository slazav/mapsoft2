#include <iostream>
#include "viewer/dthread_viewer.h"
#include "gobj_test_tile.h"

// dthread viewer + slow tile object

int main(int argc, char **argv){

    Gtk::Main     kit (argc, argv);
    Gtk::Window   win;
    ConvBase      cnv;
    GObjTestTile  pl2(cnv, true);

    DThreadViewer viewer(&pl2);

    win.add(viewer);
    win.set_default_size(640,480);
    win.show_all();

    kit.run(win);
}
