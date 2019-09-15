## filename -- functions for working with filenames

* Check if the file has specified extension (case insensitive).
```
bool file_ext_check(const std::string &fname, const char *ext);
```

Example:
``` c++
file_ext_check("file.jpg", ".jpg") => true
```

* Replace last file extension (if any) with a new one.
Example: file_ext_repl("file.jpg", ".gif") => "file.gif"
``` c++
std::string file_ext_repl(const std::string &fname, const char *ext);
```

* Extract directory names from a filename, a list which can be used to
 make/delete all dirs:
```c++
std::vector<std::string> file_get_dirs(const std::string &fname);
```

Example: d1/d2/d3/../d4/f -> d1/d2/d3/../d4, d1/d2/d3, d1/d2, d1

* Extract directory prefix from a filename:
```c++
std::string file_get_prefix(const std::string &fname);
```
Example: d1/d2/d3/../d4/f.ext -> d1/d2/d3/../d4/

