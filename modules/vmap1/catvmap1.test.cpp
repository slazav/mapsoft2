#include "vmap1.h"
#include "err/err.h"
#include <fstream>

int
main(int argc, char* argv[]){
  try {

    if (argc<2) {
      std::cerr << "Usage: catvmap1 <in_file1> ... > <out_file>\n";
      return 1;
    }

    VMap1 W;

    for (int i=1; i<argc; i++){
      std::ifstream s(argv[i]);
      W.add(read_vmap1(s));
    }

    write_vmap1(std::cout, W);
  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
  }
  return 0;
}