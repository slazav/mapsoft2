#include "mp.h"
#include "err/err.h"
#include <fstream>

int
main(int argc, char* argv[]){
  try {

    if (argc<2) {
      std::cerr << "Usage: catmp <in_file1> ... > <out_file>\n";
      return 1;
    }

    MP W;

    for (int i=1; i<argc; i++){
      std::ifstream s(argv[i]);
      read_mp(s, W);
    }

    write_mp(std::cout, W);
  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
  }
  return 0;
}