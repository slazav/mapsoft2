///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "file_ext.h"

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
}

///\endcond
