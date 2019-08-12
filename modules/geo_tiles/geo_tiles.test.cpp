#include <cassert>
#include <iostream>
#include "geo_tiles.h"
int
main(){
  try{


    // avoid zero lat or lon because it is some small number,
    // different on i586 and x86_64
    GeoTiles calc;
    assert(type_to_str(calc.tile_to_range(0,0,0)) == "[-180,-85.0511288,360,170.102258]");
    assert(type_to_str(calc.tile_to_range(0,0,1)) == "[-180,-85.0511288,180,85.0511288]");
    assert(type_to_str(calc.tile_to_range(1,0,2)) == "[-90,-85.0511288,90,18.5378683]");

    assert(type_to_str(calc.gtile_to_range(0,1,1)) == "[-180,-85.0511288,180,85.0511288]");
    assert(type_to_str(calc.gtile_to_range(1,0,2)) == "[-90,66.5132604,90,18.5378683]");

    assert(calc.pt_to_tile(dPoint(1,1),0) == iPoint(0,0));
    assert(calc.pt_to_tile(dPoint(1,1),1) == iPoint(1,1));
    assert(calc.pt_to_tile(dPoint(1,1),2) == iPoint(2,2));

    assert(calc.pt_to_gtile(dPoint(1,1),0) == iPoint(0,0));
    assert(calc.pt_to_gtile(dPoint(1,1),1) == iPoint(1,0));
    assert(calc.pt_to_gtile(dPoint(1,1),2) == iPoint(2,1));

    assert(calc.range_to_tiles(dRect(-1,-1,2,2),0) == iRect(0,0,1,1));
    assert(calc.range_to_tiles(dRect(-1,-1,2,2),1) == iRect(0,0,2,2));
    assert(calc.range_to_tiles(dRect(-1,-1,2,2),2) == iRect(1,1,2,2));

    assert(calc.range_to_gtiles(dRect(-1,-1,2,2),0) == iRect(0,0,1,1));
    assert(calc.range_to_gtiles(dRect(-1,-1,2,2),1) == iRect(0,0,2,2));
    assert(calc.range_to_gtiles(dRect(-1,-1,2,2),2) == iRect(1,1,2,2));


    assert(calc.pt_to_gtile(dPoint(77.695619,32.183764),15) == iPoint(23456,13287));
  }
  catch (Err e){
    std::cerr << "Error:" << e.str() << "\n";
  }
  return 0;
}
