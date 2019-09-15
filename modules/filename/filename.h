#ifndef FILENAME_H
#define FILENAME_H

#include <string>
#include <vector>

/* Check if the file has specified extension (case insensitive).
   Example: file_ext_check("file.jpg", ".jpg") => true
*/
bool file_ext_check(const std::string &fname, const char *ext);


/* Replace last file extension (if any) with a new one.
   Example: file_ext_repl("file.jpg", ".gif") => "file.gif"
*/
std::string file_ext_repl(const std::string &fname, const char *ext);

/* Extract directory names from a filename, a list which can be used to
   make/delete all dirs:
   Example: d1/d2/d3/../d4/f -> d1/d2/d3/../d4, d1/d2/d3, d1/d2, d1
*/
std::vector<std::string> file_get_dirs(const std::string &fname);

/* Extract directory prefix from a filename */
std::string file_get_prefix(const std::string &fname);


#endif
