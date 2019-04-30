///\cond HIDDEN (do not show this in Doxyden)

#include <fstream>

#include <sys/stat.h>
#include <sys/types.h>

#include "tmpdir.h"
#include "err/err.h"


int
main(int argc, char **argv){

  // create files, zip them
  try{
    if (argc<2) throw Err() << "usage: " << argv[0] << " <file1> <file2> ... <zip>";

    TmpDir tmpdir("mapsift_test1_XXXXXX");
    for (int i=1; i<argc-1; i++) {
       std::string fname = argv[i];
       std::string fpath = tmpdir.add(fname);
       if (*fname.rbegin()!='/'){
         std::ofstream f(fpath);
         f << "test: " << i << "\n";
       }
    }
    tmpdir.zip(argv[argc-1]);
  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }

  // unzip files, print content
  try{
    TmpDir tmpdir("mapsift_test2_XXXXXX");
    tmpdir.unzip(argv[argc-1]);
    std::vector<std::string> files = tmpdir.get_files();
    std::vector<std::string> paths = tmpdir.get_paths();
    //std::cout << "dir: " << tmpdir.get_dir() << "\n";

    //for (int i=0; i<files.size(); i++)
    //  std::cout << files[i] << " " << paths[i] << "\n";

    tmpdir.zip(argv[argc-1]); //zip back
  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }

  return 0;

}

///\endcond
