///\cond HIDDEN (do not show this in Doxyden)

#include <iostream>
#include "read_words.h"
#include "err/err.h"

int
main(){
try{
    while (1){
      std::vector<std::string> vs = read_words(std::cin);
      if (std::cin.eof() && vs.size()<1) break;
      std::vector<std::string>::const_iterator i;
      for (i=vs.begin(); i!=vs.end(); i++)
        std::cout << " [" << *i << "]";
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
