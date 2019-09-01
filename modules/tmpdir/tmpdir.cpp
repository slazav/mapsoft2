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
#include "filename/filename.h"


using namespace std;

TmpDir::TmpDir(const char *mask) {
  dir = mask;
  std::string etmp = getenv("TMPDIR");
  if (etmp != "") dir = etmp + "/" + dir;
  if (mkdtemp((char *)dir.c_str()) == NULL)
    throw Err() << "TMPDIR: Can't create directory: "
                << dir << ": " << strerror(errno);
}

TmpDir::~TmpDir() {
  std::vector<std::string>::reverse_iterator i;

  // remove files and directories (including the base one)
  paths.insert(paths.begin(), dir);
  for (i=paths.rbegin(); i!=paths.rend(); i++) {
    if (remove(i->c_str())!=0)
      std::cerr << "TMPDIR: Can't remove file or directory: "
                << *i << ": " << strerror(errno) << endl;
  }
}

std::string
TmpDir::add(const std::string & fname) {

  // set is_dir flag
  bool is_dir = *fname.rbegin() == '/';

  // find sub-folders, add it recursively
  int i = fname.rfind('/', is_dir?fname.size()-2:-1);
  if (i>0) {
    std::string head(fname.begin(), fname.begin()+i);
    i = head.rfind('/');
    std::string tail(head.begin()+i+1, head.end());
    if (tail!="." && tail!=".." && tail!="")
      add(head + '/');
  }

  // full path
  std::string fpath = dir + "/" + fname;

  // was it created already?
  bool exists=false;
  for (int j=0; j<files.size(); j++)
    if (files[j] == fname) {exists=true; break;}
  if (exists) return fpath;

  // create directory
  if (is_dir) {
    if (mkdir(fpath.c_str(), 0777)!=0){
      throw Err() << "TMPDIR: Can't create directory: "
                  << fpath << ": " << strerror(errno);
    }
  }
  else {
    // create empty file
    ofstream s(fpath);
  }

  // add to lists
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
TmpDir::unzip(const std::string & zipname) {

  int err;
  struct zip *zip_file = zip_open(zipname.c_str(), 0, &err);
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

    // add the file to TmpDir lists, create subfolders
    std::string fpath;
    try {
      fpath = add(fname);
    } catch (Err e) {
      zip_fclose(file_in_zip);
      zip_close(zip_file);
      throw e;
    }

    // write file content
    if (*fname.rbegin()!='/') {
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

