#ifndef TMPDIR_H
#define TMPDIR_H

#include <vector>
#include <string>

/*
Class for working with temporary directories and zip files.
Create a temporary directory, fill it with files, zip everything,
unzip files to directory etc. Directory and all files
will be deleted in the destructor.
*/
class TmpDir {
  std::string dir;
  std::vector<std::string> files, paths;

  public:
  /// Crete a tmp directory using a mask.
  /// Mask should contain a XXXXXX template.
  /// Directory is created in $TMPDIR.
  TmpDir(const char *mask);

  ~TmpDir();

  /// Create (empty) file or directory (if `fname` ends with `/`) in
  /// the tmp directory, return full path.
  /// All sub-directories are created automatically.
  /// If directory or file have been already added the method
  ///  has no effect.
  std::string add(const std::string & fname);

  /// Get directory name.
  std::string & get_dir() {return dir;}

  // Get file list (paths relative to dir, as added by add()).
  std::vector<std::string> & get_files() {return files;}

  // Get file list with full paths (dir + '/' + relative paths).
  std::vector<std::string> & get_paths() {return paths;}


  // Zip all files in the directory.
  void zip(const std::string & zipname) const;

  // Unzip file to the directory.
  void unzip(const char* zipname);

};

#endif
