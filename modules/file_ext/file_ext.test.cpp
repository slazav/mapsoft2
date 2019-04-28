///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "file_ext.h"

using namespace std;

int main() {
    assert(file_ext("file.jpg",   ".jpg") == 1);
    assert(file_ext("file.a.jpg", ".jpg") == 1);
    assert(file_ext("file.jpg.a", ".jpg") == 0);
    assert(file_ext("FILE.JPG",   ".jpg") == 1);
    assert(file_ext("FILE.JPG",   ".gif") == 0);
}

///\endcond
