#include "viewer/action_manager.h"
#include "viewer/dthread_viewer.h"
#include "viewer/rubber.h"

#include "gobj_test_grid.h"
#include "action_test_circ.h"

int main(int argc, char **argv){

    Gtk::Main     kit (argc, argv);
    Gtk::Window   win;
    ConvBase      cnv;
    GObjTestGrid  p1(cnv, 150000);

    DThreadViewer viewer(&p1);
    Rubber  rubber(&viewer);
    ActionManager AM(&viewer);
    ActionTestCirc A1(&rubber);
    AM.add(&A1);
    AM.select("TestCirc");

    win.add(viewer);
    win.set_default_size(640,480);
    win.show_all();

    kit.run(win);
}
