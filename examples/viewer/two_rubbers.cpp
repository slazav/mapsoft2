#include "viewer/action_manager.h"
#include "viewer/dthread_viewer.h"
#include "viewer/rubber.h"

#include "gobj_test_grid.h"
#include "action_test_line.h"

int main(int argc, char **argv){

    Gtk::Main     kit (argc, argv);
    Gtk::Window   win;
    GObjTestGrid  p1(150000);

    DThreadViewer viewer(&p1);
    Rubber  rubber1(&viewer);
    Rubber  rubber2(&viewer);

    // add box to rubber1
    iPoint p(20,20);
    rubber1.add_src_sq(p, 3);
    rubber1.add_dst_sq(3);
    rubber1.add_rect(p);
    rubber1.add_line(p);

    ActionManager AM(&viewer);
    ActionTestLine A(&rubber2);
    AM.add(&A);
    AM.select(A.get_name());

    win.add(viewer);
    win.set_default_size(640,480);
    win.show_all();

    kit.run(win);
}
