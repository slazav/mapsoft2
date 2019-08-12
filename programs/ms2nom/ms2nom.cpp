#include <cstring>
#include "geo_nom/geo_nom.h"

using namespace std;

void usage(){
  cout << "\n"
       << "ms2nom -- soviet nomenclature map calculations\n"
       << "\n"
       << "usage: ms2nom [-E] -p <point> <scale> -- maps at the point\n"
       << "       ms2nom [-E] -r <range> <scale> -- maps at the range\n"
       << "       ms2nom [-E] -n <name> -- map range\n"
       << "       ms2nom [-E] -c <name> -- map center\n"
       << "       ms2nom [-E] -s <name> x_shift y_shift -- adjacent map\n"
       << "       ms2nom [-E] -S <name> <scale> -- maps of different scale\n"
       << "       ms2nom [-E] -t <name> <range>  -- check if the map touches the range\n"
       << "\n"
       << "Option -E turns on 'extended mode': single sheets (like Q10-001) are allowed\n"
       << "on input and always returned on output; for a single sheet suffix '.<N>x<M>' is\n"
       << "allowed to multiply the range (like n49-001.3x2). At the moment -s option with\n"
       << "such multiplication returns single sheets. This is not very useful\n"
       << "and maybe changed later.\n"
       << "\n"
       << "Supported scales: 1:1000000, 1:500000, 1:200000, 1:100000, 1:50000.\n"
       << "Scale can be written in following forms:\n"
       << "* 1:1000000, 1:1'000'000, 1000000, 1M, 10km/cm\n"
       << "* 1:500000, 1:500'000, 500000, 500k, 5km/cm\n"
       << "* 1:200000, 1:200'000, 200000, 100k, 1km/cm\n"
       << "* 1:100000, 1:100'000, 100000, 100k, 1km/cm\n"
       << "* 1:50000, 1:50'000, 50000, 50k, 500m/cm\n"
       << "\n"
  ;
}

int
main(int argc, char **argv){
  try {

    bool ex = false;

    // extended mode
    if ((argc>2) && (strcmp(argv[1], "-E")==0)){
      argc--; argv++; ex=true;
    }

    if ((argc == 4) && (strcmp(argv[1], "-p") == 0)){
      string name = pt_to_nom(
          dPoint(argv[2]), str_to_type<nom_scale_t>(argv[3]), ex);
      cout << name << "\n";
      return 0;
    }

    if ((argc == 4) && (strcmp(argv[1], "-r") == 0)){
      set<string> names = range_to_nomlist(
          dRect(argv[2]), str_to_type<nom_scale_t>(argv[3]), ex);
      for (auto const &n:names) cout << n << "\n";
      return 0;
    }

    if ((argc == 3) && (strcmp(argv[1], "-n") == 0)){
      nom_scale_t sc;
      cout << setprecision(9) << nom_to_range(argv[2], sc, ex) << "\n";
      return 0;
    }

    if ((argc == 3) && (strcmp(argv[1], "-c") == 0)){
      nom_scale_t sc;
      cout << setprecision(9) << nom_to_range(argv[2], sc, ex).cnt() << "\n";
      return 0;
    }

    if ((argc == 5) && (strcmp(argv[1], "-s") == 0)){
      iPoint sh(str_to_type<int>(argv[3]),
                str_to_type<int>(argv[4]));
      cout << nom_shift(argv[2], sh, ex) << "\n";
      return 0;
    }

    if ((argc == 4) && (strcmp(argv[1], "-S") == 0)){
      nom_scale_t sc;
      dRect r = expand(nom_to_range(argv[2], sc, ex), -0.001, -0.001);
      set<string> names = range_to_nomlist(r, str_to_type<nom_scale_t>(argv[3]), ex);
      for (auto const & n:names) cout << n << "\n";
      return 0;
    }

    if ((argc == 4) && (strcmp(argv[1], "-t") == 0)){
      nom_scale_t sc;
      dRect r1 = nom_to_range(argv[2], sc, ex);
      dRect r2 = str_to_type<dRect>(argv[3]);
      return intersect(r1,r2).empty();
    }

    usage();
    return 1;

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

