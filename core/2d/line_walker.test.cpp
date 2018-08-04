///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "line_walker.h"

main(){
  try{

    dLine l1("[[0,0],[2,0],[2,2],[3,2],[4,3],[5,3]]");
    LineWalker lw(l1);

    // we are at the beginning
    assert(lw.length() == 6.0+sqrt(2.0));
    assert(lw.dist() == 0);
    assert(lw.norm() == dPoint(0,1));
    assert(lw.tang() == dPoint(1,0));
    assert(lw.pt() == dPoint(0,0));

    // start moving alone the line, check dist, norm, tang, pt
    lw.move_frw(0.5);
    assert(lw.dist() == 0.5);
    assert(lw.norm() == dPoint(0,1));
    assert(lw.tang() == dPoint(1,0));
    assert(lw.pt() == dPoint(0.5,0));

    lw.move_frw(1.5);
    assert(lw.dist() == 2);
    assert(lw.norm() == dPoint(0,1));
    assert(lw.tang() == dPoint(1,0));
    assert(lw.pt() == dPoint(2,0));

    lw.move_frw(1);
    assert(lw.dist() == 3);
    assert(lw.norm() == dPoint(-1,0));
    assert(lw.tang() == dPoint(0,1));
    assert(lw.pt() == dPoint(2,1));

    lw.move_bck(0.5);
    assert(lw.is_begin() == false);
    assert(lw.is_end() == false);
    assert(lw.dist() == 2.5);
    assert(lw.norm() == dPoint(-1,0));
    assert(lw.tang() == dPoint(0,1));
    assert(lw.pt() == dPoint(2,0.5));

    // move to the beginning
    lw.move_begin();
    assert(lw.is_begin() == true);
    assert(lw.is_end() == false);
    assert(lw.dist() == 0);
    assert(lw.pt() == dPoint(0,0));

    // moving back from the beginning does not change anything
    lw.move_bck_to_node();
    assert(lw.dist() == 0);
    assert(lw.pt() == dPoint(0,0));

    lw.move_bck(1);
    assert(lw.dist() == 0);
    assert(lw.pt() == dPoint(0,0));

    // move to the end
    lw.move_end();
    assert(lw.is_begin() == false);
    assert(lw.is_end() == true);
    assert(lw.dist() == lw.length());
    assert(lw.pt() == dPoint(5,3));
    assert(lw.norm() == dPoint(0,1));
    assert(lw.tang() == dPoint(1,0));

    // moving forward does not change anything
    lw.move_frw_to_node();
    assert(lw.dist() == lw.length());
    assert(lw.pt() == dPoint(5,3));

    lw.move_frw(1);
    assert(lw.dist() == lw.length());
    assert(lw.pt() == dPoint(5,3));

    // check move_bck_to_node, move_frw_to_node
    lw.move_bck_to_node();
    assert(lw.dist() == lw.length()-1);
    assert(lw.pt() == dPoint(4,3));

    lw.move_frw_to_node();
    assert(lw.dist() == lw.length());
    assert(lw.pt() == dPoint(5,3));

    // check get_points()
    assert(lw.get_points(1) == dLine("[[5,3]]"));
    assert(lw.pt() == dPoint(5,3));

    lw.move_begin();
    assert(lw.get_points(1) == dLine("[[0,0],[1,0]]"));
    assert(lw.pt() == dPoint(1,0));

    assert(lw.get_points(2) == dLine("[[1,0],[2,0],[2,1]]"));
    assert(lw.pt() == dPoint(2,1));

    assert(lw.get_points(6) == dLine("[[2,1],[2,2],[3,2],[4,3],[5,3]]"));
    assert(lw.is_end() == true);

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond
