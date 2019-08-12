#include "geo_tiles.h"
#include "opt/opt.h"
#include <iostream>
#include <cstring>

using namespace std;

void usage(){
  cerr << "\n"
       << "Google tile calculator.\n"
       << "usage: convs_tile -p <point> <z> -- tile which covers a WGS84 point\n"
       << "       convs_tile -r <range> <z> -- tiles which cover a range (return a rectangle)\n"
       << "       convs_tile -R <range> <z> -- tiles which cover a range (return a list)\n"
       << "       convs_tile -n x y z -- tile range\n"
       << "       convs_tile -c x y z -- tile center\n"
       << "       convs_tile -t x y z <range>  -- check if a tile touches a range\n"
       << "\n"
  ;
}

int main(int argc, char** argv){

  GeoTiles calc;

  if ((argc == 4) && (strcmp(argv[1], "-p") == 0)){
    iPoint ret = calc.pt_to_gtile(
        str_to_type<dPoint>(argv[2]),
        str_to_type<int>(argv[3]));
    cout << ret << "\n";
    return 0;
  }

  if ((argc == 4) && (strcmp(argv[1], "-r") == 0)){
    iRect ret = calc.range_to_gtiles(
        str_to_type<dRect>(argv[2]),
        str_to_type<int>(argv[3]));
    cout << ret << "\n";
    return 0;
  }

  if ((argc == 4) && (strcmp(argv[1], "-R") == 0)){
    int z = str_to_type<int>(argv[3]);
    iRect ret = calc.range_to_gtiles(
        str_to_type<dRect>(argv[2]),z);
    for (int y = ret.y; y<ret.y+ret.h; y++)
      for (int x = ret.x; x<ret.x+ret.w; x++)
        cout << x << "," << y << "," << z << "\n";
    return 0;
  }

  if ((argc == 5) && (strcmp(argv[1], "-n") == 0)){
    dRect ret = calc.gtile_to_range(
      str_to_type<int>(argv[2]),
      str_to_type<int>(argv[3]),
      str_to_type<int>(argv[4]));
    cout << setprecision(9) << ret << "\n";
    return 0;
  }

  if ((argc == 5) && (strcmp(argv[1], "-c") == 0)){
    dRect ret = calc.gtile_to_range(
      str_to_type<int>(argv[2]),
      str_to_type<int>(argv[3]),
      str_to_type<int>(argv[4]));
    cout << setprecision(9) << ret.cnt() << "\n";
    return 0;
  }

  if ((argc == 6) && (strcmp(argv[1], "-t") == 0)){
    dRect r1 = calc.gtile_to_range(
      str_to_type<int>(argv[2]),
      str_to_type<int>(argv[3]),
      str_to_type<int>(argv[4]));
    dRect r2 = str_to_type<dRect>(argv[5]);
    return intersect(r1,r2).empty();
  }

  usage();
  return 1;
}
