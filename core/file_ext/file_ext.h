#ifndef FILETYPE_H
#define FILETYPE_H

#include <string>

/* check if the file has specified extension (case insensitive) */
bool file_ext(const std::string &fname, const char *ext);

#endif
