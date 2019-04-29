#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <zip.h>

#include "tmpdir.h"
#include "err/err.h"
#include "file_ext/file_ext.h"


using namespace std;

TmpDir::TmpDir(const char *mask) {
  dir = mask;
  std::string etmp = getenv("TMPDIR");
  if (etmp != "") dir = etmp + "/" + dir;
  if (mkdtemp((char *)dir.c_str()) == NULL)
    throw Err() << "Can't create temporary directory: "
                << dir << ": " << strerror(errno);
}

TmpDir::~TmpDir() {
  std::vector<std::string>::reverse_iterator i;
  paths.insert(paths.begin(), dir);
  for (i=paths.rbegin(); i!=paths.rend(); i++) {
    if (remove(i->c_str())!=0 && errno != ENOENT)
      std::cerr << "Can't remove temporary file or directory: "
                << *i << ": " << strerror(errno) << endl;
  }
}

std::string
TmpDir::add(const std::string & fname) {
  std::string fpath = dir + "/" + fname;
  files.push_back(fname);
  paths.push_back(fpath);
  return fpath;
}

void
TmpDir::zip(const std::string & zipname) const{
  struct zip *Z;
  int err;

  remove(zipname.c_str());
  Z = zip_open(zipname.c_str(), ZIP_CREATE, &err);
  if (!Z) throw Err() << "Can't open ZIP file " << zipname << " for writing";

  std::vector<std::string>::const_iterator f;
  for (int i=0; i<files.size(); i++){

    // directory
    if (*files[i].rbegin() == '/'){
      if (zip_dir_add(Z, files[i].c_str(), 0) < 0) {
        zip_close(Z);
        throw Err() << "Can't create directory in ZIP file: " << zip_strerror(Z);
      }
    }
    // file
    else {
      struct zip_source *s = zip_source_file(Z, paths[i].c_str(),0,0);
      if ((s == NULL) || (zip_add(Z, files[i].c_str(), s) < 0)) {
        zip_close(Z);
        throw Err() << "Can't write data to ZIP file: " << zip_strerror(Z);
      }
    }
  }
  if (zip_close(Z)!=0)
    throw Err() << "Can't write data to ZIP file: " << zip_strerror(Z);
}


void
TmpDir::unzip(const char* zipname) {

  int err;
  struct zip *zip_file = zip_open(zipname, 0, &err);
  if (!zip_file)
    throw Err() << "Can't open ZIP file " << zipname << ": " << zip_strerror(zip_file);

  int files_total = zip_get_num_files(zip_file);
  if (!files_total) {
    zip_close(zip_file);
    throw Err() << "Can't read ZIP file " << zipname << ": " << zip_strerror(zip_file);
  }

  for (int i = 0; i < files_total; i++) {
    struct zip_file *file_in_zip = zip_fopen_index(zip_file, i, 0);
    if (!file_in_zip){
      zip_close(zip_file);
      throw Err() << "Can't read file from ZIP file: "
        << zipname << ": " << zip_strerror(zip_file);
    }

    std::string fname = zip_get_name(zip_file,i,0);

    // create subfolders
    std::vector<std::string> dirlist = file_get_dirs(fname);
    for (int i=0; i<dirlist.size(); i++){

      // was it created already?
      bool exists=false;
      for (int j=0; j<files.size(); j++)
        if (files[j] == dirlist[i] + '/') {exists=true; break;}
      if (exists) continue;

      // add to file list, build full path
      std::string fpath = add(dirlist[i] + '/');
      // create
      if (mkdir(fpath.c_str(), 0777)!=0){
        zip_fclose(file_in_zip);
        zip_close(zip_file);
        throw Err() << "Reading ZIP file: can't create directory: "
                    << dirlist[i] << ": " << strerror(errno);
      }
    }

    // write file content
    if (*fname.rbegin()!='/') {
      std::string fpath = add(fname);
      ofstream out(fpath.c_str());
      char buffer[1];
      while ( zip_fread(file_in_zip, buffer, sizeof(buffer)) > 0)
        out << buffer[0];

      out.close();
    }
    zip_fclose(file_in_zip);
  }
  if (zip_close(zip_file)!=0)
    throw Err() << "Can't decode ZIP file: "
                << zipname << ": " << zip_strerror(zip_file);
}

