#include <cassert>
#include <iostream>
#include "geo_tiles.h"
#include "err/assert_err.h"
int
main(){
  try{


    // avoid zero lat or lon because it is some small number,
    // different on i586 and x86_64
    GeoTiles calc;

    assert_eq(calc.get_tsize(), 256);
    assert_eq(type_to_str(calc.ll_to_m(dPoint(30,67))), "[3339584.72,10156058.7]");
    assert_eq(type_to_str(calc.m_to_ll(dPoint(3339584.72,10156058.7))), "[30,66.9999999]");

//  calc.px_to_m(const dPoint & p, const int z) const {
//  calc.px2m(const int z) const { return ires/(1<<z); }
//  calc.px_to_tile(const dPoint & p) const {
//  calc.tile_gtile(const iPoint &p, const int z) const {
//
//  calc.tile_to_qtree(const iPoint &p, const int z) const {



    assert_eq(type_to_str(calc.tile_to_range(0,0,0)), "[-180,-85.0511288,360,170.102258]");
    assert_eq(type_to_str(calc.tile_to_range(0,0,1)), "[-180,-85.0511288,180,85.0511288]");
    assert_eq(type_to_str(calc.tile_to_range(1,0,2)), "[-90,-85.0511288,90,18.5378683]");

    assert_eq(type_to_str(calc.gtile_to_range(0,1,1)), "[-180,-85.0511288,180,85.0511288]");
    assert_eq(type_to_str(calc.gtile_to_range(1,0,2)), "[-90,66.5132604,90,18.5378683]");

    assert_eq(calc.pt_to_tile(dPoint(1,1),0), iPoint(0,0));
    assert_eq(calc.pt_to_tile(dPoint(1,1),1), iPoint(1,1));
    assert_eq(calc.pt_to_tile(dPoint(1,1),2), iPoint(2,2));

    assert_eq(calc.pt_to_gtile(dPoint(1,1),0), iPoint(0,0));
    assert_eq(calc.pt_to_gtile(dPoint(1,1),1), iPoint(1,0));
    assert_eq(calc.pt_to_gtile(dPoint(1,1),2), iPoint(2,1));


    assert_eq(calc.range_to_tiles(dRect(-1,-1,2,2),0), iRect(0,0,1,1));
    assert_eq(calc.range_to_tiles(dRect(-1,-1,2,2),1), iRect(0,0,2,2));
    assert_eq(calc.range_to_tiles(dRect(-1,-1,2,2),2), iRect(1,1,2,2));

    assert_eq(calc.range_to_gtiles(dRect(-1,-1,2,2),0), iRect(0,0,1,1));
    assert_eq(calc.range_to_gtiles(dRect(-1,-1,2,2),1), iRect(0,0,2,2));
    assert_eq(calc.range_to_gtiles(dRect(-1,-1,2,2),2), iRect(1,1,2,2));

    assert_eq(calc.pt_to_gtile(dPoint(77.695619,32.183764),15), iPoint(23456,13287));
    assert_eq(calc.pt_to_gtile(dPoint(26.77188,61.33552),14), iPoint(9410,4633));

    assert_eq(type_to_str(calc.gtile_to_range(iPoint(9410,4633), 14)),
      "[26.7626953,61.3335397,0.0219726562,0.0105387287]");
  }
  catch (Err e){
    std::cerr << "Error:" << e.str() << "\n";
  }
  return 0;
}
