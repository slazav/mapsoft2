/* Test program! */
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cassert>

#include "err/assert_err.h"
#include "iofilter.h"



int
main(){

  {
    std::ifstream ff("iofilter.test.cpp");
    IFilter flt(ff, "cat");

    std::string l;
    std::getline(flt.stream(), l);
    assert_eq(l, "/* Test program! */");
  }

  {
    IFilter flt("echo abc");
    std::string l;
    std::getline(flt.stream(), l);
    assert_eq(l, "abc");
  }


  {
    {
      std::ofstream ff("test1.tmp");
      OFilter flt(ff, "tac");
      flt.stream() << "test1\ntest2\n";
    }

sleep(1);
    std::ifstream fi("test1.tmp");
    std::string l;
    std::getline(fi, l);
    assert_eq(l, "test2");
    std::getline(fi, l);
    assert_eq(l, "test1");
    unlink("test1.tmp");
  }

  {
    {
      OFilter flt("tac > test2.tmp");
      flt.stream() << "test1\ntest2\n";
    }

sleep(1);
    std::ifstream fi("test2.tmp");
    std::string l;
    std::getline(fi, l);
    assert_eq(l, "test2");
    std::getline(fi, l);
    assert_eq(l, "test1");
    unlink("test2.tmp");
  }


  return 0;
}
