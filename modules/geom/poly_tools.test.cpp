///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "poly_tools.h"
#include "opt/opt.h"

int
main(){
  try{

    // point_in_polygon (int)
    for (int b=0; b<2; b++){
      iLine L("[[0,0],[8,0],[4,8]]");
      assert(point_in_polygon(iPoint("[-1,-1]"),L,b) == false);
      assert(point_in_polygon(iPoint("[7,3]"),L,b) == false);
      assert(point_in_polygon(iPoint("[1,1]"),L,b) == true);

      // borders
      assert(point_in_polygon(iPoint("[0,0]"),L,b) == b);
      assert(point_in_polygon(iPoint("[8,0]"),L,b) == b);
      assert(point_in_polygon(iPoint("[4,0]"),L,b) == b);
      assert(point_in_polygon(iPoint("[4,8]"),L,b) == b);
      assert(point_in_polygon(iPoint("[1,2]"),L,b) == b);
      assert(point_in_polygon(iPoint("[7,2]"),L,b) == b);
    }

    {
      iLine L("[[-10,-10], [64,-10], [128,64], [-10,100]]");
      assert(point_in_polygon(iPoint("[130,64]"),L,1) == false);
      assert(point_in_polygon(iPoint("[120,64]"),L,1) == true);
    }

    {
      iLine L("[[0,0], [10,0], [5,10], [0,0]]");
      assert(point_in_polygon(iPoint("[10,10]"),L,1) == false);
      assert(point_in_polygon(iPoint("[0,10]"),L,1) == false);
      assert(point_in_polygon(iPoint("[5,10]"),L,1) == true);
    }

    // rect_in_polygon (int)
    for (int b=0; b<2; b++){
      iLine L("[[0,0],[8,0],[4,8]]");
      assert(rect_in_polygon(iRect("[-2,-2,1,1]"),L,b) == false);
      assert(rect_in_polygon(iRect("[7,3,1,1]"),L,b) == false);
      assert(rect_in_polygon(iRect(),L,b) == false);
      assert(rect_in_polygon(iRect("[1,1,0,0]"),L,b) == true);

      // borders
      assert(rect_in_polygon(iRect("[0,0,0,0]"),L,b) == b);
      assert(rect_in_polygon(iRect("[-1,-1,1,1]"),L,b) == b);
      assert(rect_in_polygon(iRect("[8,0,1,1]"),L,b)  == b);
      assert(rect_in_polygon(iRect("[4,-1,1,1]"),L,b) == b);
      assert(rect_in_polygon(iRect("[4,8,1,1]"),L,b) == b);
      assert(rect_in_polygon(iRect("[0,2,1,1]"),L,b) == b);
      assert(rect_in_polygon(iRect("[7,2,1,1]"),L,b) == b);
    }

    // point_in_polygon (double)
    for (int b=0; b<2; b++){
      dLine L("[[0,0],[8,0],[4,8]]");
      assert(point_in_polygon(dPoint("[-1,-1]"),L,b) == false);
      assert(point_in_polygon(dPoint("[7,3]"),L,b) == false);
      assert(point_in_polygon(dPoint("[1,1]"),L,b) == true);

      // borders
      assert(point_in_polygon(dPoint("[0,0]"),L,b) == b);
      assert(point_in_polygon(dPoint("[8,0]"),L,b) == b);
      assert(point_in_polygon(dPoint("[4,0]"),L,b) == b);
      assert(point_in_polygon(dPoint("[4,8]"),L,b) == b);
      assert(point_in_polygon(dPoint("[1,2]"),L,b) == b);
      assert(point_in_polygon(dPoint("[7,2]"),L,b) == b);
    }

    // rect_in_polygon (double)
    for (int b=0; b<2; b++){
      dLine L("[[0,0],[8,0],[4,8]]");
      assert(rect_in_polygon(dRect("[-2,-2,1,1]"),L,b) == false);
      assert(rect_in_polygon(dRect("[7,3,1,1]"),L,b) == false);
      assert(rect_in_polygon(dRect(),L,b) == false);
      assert(rect_in_polygon(dRect("[1,1,0,0]"),L,b) == true);

      // borders
      assert(rect_in_polygon(dRect("[0,0,0,0]"),L,b) == b);
      assert(rect_in_polygon(dRect("[-1,-1,1,1]"),L,b) == b);
      assert(rect_in_polygon(dRect("[8,0,1,1]"),L,b)  == b);
      assert(rect_in_polygon(dRect("[4,-1,1,1]"),L,b) == b);
      assert(rect_in_polygon(dRect("[4,8,1,1]"),L,b) == b);
      assert(rect_in_polygon(dRect("[0,2,1,1]"),L,b) == b);
      assert(rect_in_polygon(dRect("[7,2,1,1]"),L,b) == b);
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

      ml = dMultiLine("[]");
      assert(join_polygons(ml) == dLine("[]"));
      remove_holes(ml);
      assert(ml == dMultiLine("[]"));

      ml = dMultiLine("[[]]");
      assert(join_polygons(ml) == dLine("[]"));
      remove_holes(ml);
      assert(ml == dMultiLine("[[]]"));
    }

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond