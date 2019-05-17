///\cond HIDDEN (do not show this in Doxyden)
#include <fstream>
#include "err/err.h"
#include "fig_data.h"

int
main(int argc, char* argv[]){
  try {

    if (argc<2) {
      std::cerr << "Usage: catfig <in_file1> ... > <out_file>\n";
      return 1;
    }

    Fig W;

    for (int i=1; i<argc; i++){
      std::cerr << "Reading " << argv[i] << "\n";
      std::ifstream s(argv[i]);
      W.read(s);
    }

    W.write(std::cout);
  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
  }
  return 0;
}

///\endcond
