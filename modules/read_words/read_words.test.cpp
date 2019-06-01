///\cond HIDDEN (do not show this in Doxyden)

#include <iostream>
#include "read_words.h"
#include "err/err.h"

int
main(int argc, const char* argv[]){
  try {

    bool lc = (argc==2 && std::string(argv[1])=="-l");
    int N[2] = {0,0};
    while (1){
      std::vector<std::string> vs = read_words(std::cin, N, lc);
      if (vs.size()<1) break;

      std::cout << N[0] << ":";
      for (auto const & i:vs) std::cout << " [" << i << "]";
      std::cout << "\n";

    }
    return 0;
  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
