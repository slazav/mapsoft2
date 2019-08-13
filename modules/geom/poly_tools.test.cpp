///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "poly_tools.h"
#include "opt/opt.h"

int
main(){
  try{

    // point_in_polygon (int)
    {
       iLine L("[[0,0],[8,0],[4,8]]");
       assert(point_in_polygon(iPoint("[-1,-1]"),L) == false);
       assert(point_in_polygon(iPoint("[7,3]"),L) == false);
       assert(point_in_polygon(iPoint("[1,1]"),L) == true);

       // borders
       assert(point_in_polygon(iPoint("[0,0]"),L) == false);
       assert(point_in_polygon(iPoint("[8,0]"),L) == true);
       assert(point_in_polygon(iPoint("[4,0]"),L) == true);
       assert(point_in_polygon(iPoint("[4,8]"),L) == false);
       assert(point_in_polygon(iPoint("[1,2]"),L) == false);
       assert(point_in_polygon(iPoint("[7,2]"),L) == true);
    }

    // rect_in_polygon (int)
    {
       iLine L("[[0,0],[8,0],[4,8]]");
       assert(rect_in_polygon(iRect("[-2,-2,1,1]"),L) == false);
       assert(rect_in_polygon(iRect("[7,3,1,1]"),L) == false);

       // borders
       assert(rect_in_polygon(iRect("[-1,-1,0,0]"),L) == false);
       assert(rect_in_polygon(iRect("[8,0,1,1]"),L) == false);
       assert(rect_in_polygon(iRect("[4,-1,1,1]"),L) == true);
       assert(rect_in_polygon(iRect("[4,8,1,1]"),L) == false);
       assert(rect_in_polygon(iRect("[0,2,1,1]"),L) == true);
       assert(rect_in_polygon(iRect("[7,2,1,1]"),L) == false);
    }

    // point_in_polygon (double)
    {
       dLine L("[[0,0],[8,0],[4,8]]");
       assert(point_in_polygon(dPoint("[-1,-1]"),L) == false);
       assert(point_in_polygon(dPoint("[7,3]"),L) == false);
       assert(point_in_polygon(dPoint("[1,1]"),L) == true);

       // borders
       assert(point_in_polygon(dPoint("[0,0]"),L) == false);
       assert(point_in_polygon(dPoint("[8,0]"),L) == true);
       assert(point_in_polygon(dPoint("[4,0]"),L) == true);
       assert(point_in_polygon(dPoint("[4,8]"),L) == false);
       assert(point_in_polygon(dPoint("[1,2]"),L) == false);
       assert(point_in_polygon(dPoint("[7,2]"),L) == true);
    }

    // rect_in_polygon (double)
    {
       dLine L("[[0,0],[8,0],[4,8]]");
       assert(rect_in_polygon(dRect("[-2,-2,1,1]"),L) == false);
       assert(rect_in_polygon(dRect("[7,3,1,1]"),L) == false);

       // borders
       assert(rect_in_polygon(dRect("[-1,-1,0,0]"),L) == false);
       assert(rect_in_polygon(dRect("[8,0,1,1]"),L) == false);
       assert(rect_in_polygon(dRect("[4,-1,1,1]"),L) == true);
       assert(rect_in_polygon(dRect("[4,8,1,1]"),L) == false);
       assert(rect_in_polygon(dRect("[0,2,1,1]"),L) == true);
       assert(rect_in_polygon(dRect("[7,2,1,1]"),L) == false);
    }


    // join_polygons, remove_holes
    {
      dMultiLine ml("["
        "[[0,0],[10,0],[10,10],[0,10]],"
        "[[1,1],[2,1],[1,2]],"
        "[[11,1],[12,1],[11,2]],"
        "[[9,8],[9,9],[8,9]]"
       "]");

      assert(join_polygons(ml) == dLine(
        "[[0,0],[1,1],[2,1],[1,2],[1,1],[0,0],"
        "[10,0],[11,1],[12,1],[11,2],[11,1],[10,0],"
        "[10,10],[9,9],[8,9],[9,8],[9,9],[10,10],[0,10]]"));

      remove_holes(ml);
      assert(ml == dMultiLine(
        "[[[0,0],[1,1],[2,1],[1,2],[1,1],[0,0],"
        "[10,0],[10,10],[9,9],[8,9],[9,8],[9,9],[10,10],[0,10]],"
        "[[11,1],[12,1],[11,2]]]"));
    }

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond