///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <iostream>
#include "filename.h"

using namespace std;

int main() {
    assert(file_ext_check("file.jpg",   ".jpg") == 1);
    assert(file_ext_check("file.a.jpg", ".jpg") == 1);
    assert(file_ext_check("file.jpg.a", ".jpg") == 0);
    assert(file_ext_check("FILE.JPG",   ".jpg") == 1);
    assert(file_ext_check("FILE.JPG",   ".gif") == 0);

    assert(file_ext_repl("file.jpg", ".gif") == "file.gif");
    assert(file_ext_repl("file.j", ".gif") == "file.gif");
    assert(file_ext_repl("file", ".gif") == "file.gif");
    assert(file_ext_repl("file.jpg.jpg", ".gif") == "file.jpg.gif");
    assert(file_ext_repl(".jpg", ".gif") == ".gif");

    std::vector<std::string> ff =
      file_get_dirs("dir1/dir2/dir3//dir4/../../dir5/./dir6/file");
//    for (int i=0; i<ff.size(); i++) std::cerr << i << " -- " << ff[i] << "\n";

    assert(ff.size() == 6);
    assert(ff[0] == "dir1/dir2/dir3//dir4/../../dir5/./dir6");
    assert(ff[1] == "dir1/dir2/dir3//dir4/../../dir5");
    assert(ff[2] == "dir1/dir2/dir3//dir4");
    assert(ff[3] == "dir1/dir2/dir3");
    assert(ff[4] == "dir1/dir2");
    assert(ff[5] == "dir1");

    ff = file_get_dirs("/dir1/dir2/");
//    for (int i=0; i<ff.size(); i++) std::cerr << i << " -- " << ff[i] << "\n";

    assert(ff.size() == 2);
    assert(ff[0] == "/dir1/dir2");
    assert(ff[1] == "/dir1");
}

///\endcond
