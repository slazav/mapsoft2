#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "err/err.h"
#include "iofilter.h"



int
main(){
  std::ifstream ff("iofilter.test.cpp");

  IFilter flt(ff, "tac");

  std::cerr << "[" << flt.stream().rdbuf() << "]\n";
  return 0;
}
