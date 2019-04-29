#ifndef FILETYPE_H
#define FILETYPE_H

#include <string>

/* Check if the file has specified extension (case insensitive).
   Example: file_ext_check("file.jpg", ".jpg") => true */
bool file_ext_check(const std::string &fname, const char *ext);


/* Replace last file extension (if any) with a new one.
   Example: file_ext_repl("file.jpg", ".gif") => "file.gif" */
std::string file_ext_repl(const std::string &fname, const char *ext);

#endif
