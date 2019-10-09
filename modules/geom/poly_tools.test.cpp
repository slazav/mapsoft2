///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "err/assert_err.h"
#include "poly_tools.h"
#include "opt/opt.h"

int
main(){
  try{

    // point_in_polygon (int)
    for (int b=0; b<2; b++){
      iLine L("[[0,0],[8,0],[4,8]]");
      assert_eq(point_in_polygon(iPoint("[-1,-1]"),L,b), false);
      assert_eq(point_in_polygon(iPoint("[7,3]"),L,b), false);
      assert_eq(point_in_polygon(iPoint("[1,1]"),L,b), true);

      // borders
      assert_eq(point_in_polygon(iPoint("[0,0]"),L,b), b);
      assert_eq(point_in_polygon(iPoint("[8,0]"),L,b), b);
      assert_eq(point_in_polygon(iPoint("[4,0]"),L,b), b);
      assert_eq(point_in_polygon(iPoint("[4,8]"),L,b), b);
      assert_eq(point_in_polygon(iPoint("[1,2]"),L,b), b);
      assert_eq(point_in_polygon(iPoint("[7,2]"),L,b), b);
    }

    {
      iLine L("[[-10,-10], [64,-10], [128,64], [-10,100]]");
      assert_eq(point_in_polygon(iPoint("[130,64]"),L,1), false);
      assert_eq(point_in_polygon(iPoint("[120,64]"),L,1), true);
    }

    {
      iLine L("[[0,0], [10,0], [5,10], [0,0]]");
      assert_eq(point_in_polygon(iPoint("[10,10]"),L,1), false);
      assert_eq(point_in_polygon(iPoint("[0,10]"),L,1), false);
      assert_eq(point_in_polygon(iPoint("[5,10]"),L,1), true);
    }

    // rect_in_polygon (int)
    for (int b=0; b<2; b++){
      iLine L("[[0,0],[8,0],[4,8]]");
      assert_eq(rect_in_polygon(iRect("[-2,-2,1,1]"),L,b), false);
      assert_eq(rect_in_polygon(iRect("[7,3,1,1]"),L,b), false);
      assert_eq(rect_in_polygon(iRect(),L,b), false);
      assert_eq(rect_in_polygon(iRect("[1,1,0,0]"),L,b), true);

      // borders
      assert_eq(rect_in_polygon(iRect("[0,0,0,0]"),L,b), b);
      assert_eq(rect_in_polygon(iRect("[-1,-1,1,1]"),L,b), b);
      assert_eq(rect_in_polygon(iRect("[8,0,1,1]"),L,b), b);
      assert_eq(rect_in_polygon(iRect("[4,-1,1,1]"),L,b), b);
      assert_eq(rect_in_polygon(iRect("[4,8,1,1]"),L,b), b);
      assert_eq(rect_in_polygon(iRect("[0,2,1,1]"),L,b), b);
      assert_eq(rect_in_polygon(iRect("[7,2,1,1]"),L,b), b);
    }

    // point_in_polygon (double)
    for (int b=0; b<2; b++){
      dLine L("[[0,0],[8,0],[4,8]]");
      assert_eq(point_in_polygon(dPoint("[-1,-1]"),L,b), false);
      assert_eq(point_in_polygon(dPoint("[7,3]"),L,b), false);
      assert_eq(point_in_polygon(dPoint("[1,1]"),L,b), true);

      // borders
      assert_eq(point_in_polygon(dPoint("[0,0]"),L,b), b);
      assert_eq(point_in_polygon(dPoint("[8,0]"),L,b), b);
      assert_eq(point_in_polygon(dPoint("[4,0]"),L,b), b);
      assert_eq(point_in_polygon(dPoint("[4,8]"),L,b), b);
      assert_eq(point_in_polygon(dPoint("[1,2]"),L,b), b);
      assert_eq(point_in_polygon(dPoint("[7,2]"),L,b), b);
    }

    // rect_in_polygon (double)
    for (int b=0; b<2; b++){
      dLine L("[[0,0],[8,0],[4,8]]");
      assert_eq(rect_in_polygon(dRect("[-2,-2,1,1]"),L,b), false);
      assert_eq(rect_in_polygon(dRect("[7,3,1,1]"),L,b), false);
      assert_eq(rect_in_polygon(dRect(),L,b), false);
      assert_eq(rect_in_polygon(dRect("[1,1,0,0]"),L,b), true);

      // borders
      assert_eq(rect_in_polygon(dRect("[0,0,0,0]"),L,b), b);
      assert_eq(rect_in_polygon(dRect("[-1,-1,1,1]"),L,b), b);
      assert_eq(rect_in_polygon(dRect("[8,0,1,1]"),L,b), b);
      assert_eq(rect_in_polygon(dRect("[4,-1,1,1]"),L,b), b);
      assert_eq(rect_in_polygon(dRect("[4,8,1,1]"),L,b), b);
      assert_eq(rect_in_polygon(dRect("[0,2,1,1]"),L,b), b);
      assert_eq(rect_in_polygon(dRect("[7,2,1,1]"),L,b), b);
    }



    // join_polygons, remove_holes
    {
      dMultiLine ml("["
        "[[0,0],[10,0],[10,10],[0,10]],"
        "[[1,1],[2,1],[1,2]],"
        "[[11,1],[12,1],[11,2]],"
        "[[9,8],[9,9],[8,9]]"
       "]");

      assert_eq(join_polygons(ml), dLine(
        "[[0,0],[1,1],[2,1],[1,2],[1,1],[0,0],"
        "[10,0],[11,1],[12,1],[11,2],[11,1],[10,0],"
        "[10,10],[9,9],[8,9],[9,8],[9,9],[10,10],[0,10]]"));

      remove_holes(ml);
      assert_eq(ml, dMultiLine(
        "[[[0,0],[1,1],[2,1],[1,2],[1,1],[0,0],"
        "[10,0],[10,10],[9,9],[8,9],[9,8],[9,9],[10,10],[0,10]],"
        "[[11,1],[12,1],[11,2]]]"));

      ml = dMultiLine("[]");
      assert_eq(join_polygons(ml), dLine("[]"));
      remove_holes(ml);
      assert_eq(ml, dMultiLine("[]"));

      ml = dMultiLine("[[]]");
      assert_eq(join_polygons(ml), dLine("[]"));
      remove_holes(ml);
      assert_eq(ml, dMultiLine("[[]]"));
    }

     // figure_line, figure_bbox
    {
      assert_eq(figure_line<int>("[1,2]"), iMultiLine("[[1,2]]")); // Point
      assert_eq(figure_line<int>("[1,2,3]"), iMultiLine("[[1,2,3]]")); // Point
      assert_eq(figure_line<int>("[1,2,3,4]"), iMultiLine("[[1,2],[4,2],[4,6],[1,6],[1,2]]")); // Rect
      assert_eq(figure_line<int>("[1,2,0,0]"), iMultiLine("[[1,2],[1,2],[1,2],[1,2],[1,2]]")); // zero-size Rect
      assert_eq(figure_line<int>("[[1,2],[3,4]]"), iMultiLine("[[1,2],[3,4]]")); // Line
      assert_eq(figure_line<int>("[[[1,2],[3,4]],[]]"), iMultiLine("[[[1,2],[3,4]],[]]")); // MultiLine
      assert_eq(figure_line<int>(""), iMultiLine());
      assert_eq(figure_line<int>("[]"), iMultiLine()); // empty Line/Multiline
      assert_eq(figure_line<int>("[[]]"), iMultiLine("[[]]")); // MultiLine with one empty segment

      assert_err(figure_line<int>("aaa"), "can't read figure: aaa"); // Rect


      assert_eq(figure_bbox<int>("[1,2]"), iRect(1,2,0,0)); // Point
      assert_eq(figure_bbox<int>("[1,2,3]"), iRect(1,2,0,0)); // Point (z ignored)
      assert_eq(figure_bbox<int>("[1,2,3,4]"), iRect(1,2,3,4)); // Rect
      assert_eq(figure_bbox<int>("[1,2,0,0]"), iRect(1,2,0,0)); // Rect
      assert_eq(figure_bbox<int>("[[1,2],[3,4]]"), iRect(1,2,2,2)); // Line
      assert_eq(figure_bbox<int>("[[[1,2],[3,4]],[]]"), iRect(1,2,2,2)); // MultiLine
      assert_eq(figure_bbox<int>(""), iRect());
      assert_eq(figure_bbox<int>("[]"), iRect()); // empty Line
      assert_eq(figure_bbox<int>("[[]]"), iRect()); // empty MultiLine

      assert_err(figure_bbox<int>("aaa"), "can't read figure: aaa"); // Rect

    }


  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond