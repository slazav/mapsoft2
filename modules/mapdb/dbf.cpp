#include <vector>
#include <shapefil.h>

#include "err/err.h"
#include "dbf.h"

/**********************************************************/
// Implementation class
class Dbf::Impl {
  std::shared_ptr<void> dbf;

public:
  Impl(const char *fbase, bool create);
  ~Impl() {}

  int nfields() const;
  int nrecords() const;
  char field_type(int id) const;
  int field_find(const char * name) const;
  int add_str_field(const char *name, int size);
  bool put_str(int id, int fid, const char * val);
  std::string get_str(int id, int fid);

};

/**********************************************************/
// Main class methods
Dbf::Dbf(const char *fbase, bool create):
  impl(std::unique_ptr<Impl>(new Impl(fbase, create))) { }

int
Dbf::nfields() const {return impl->nfields();}

int
Dbf::nrecords() const {return impl->nrecords();}

char
Dbf::field_type(int id) const {return impl->field_type(id);}

int
Dbf::field_find(const char * name) const {
  return impl->field_find(name);}

int
Dbf::add_str_field(const char *name, int size) {
   return impl->add_str_field(name, size);}

bool
Dbf::put_str(int id, int fid, const char * val) {
   return impl->put_str(id, fid, val);}

std::string
Dbf::get_str(int id, int fid){ return impl->get_str(id, fid);}


Dbf::~Dbf(){}

/**********************************************************/
// Implementation class methods

Dbf::Impl::Impl(const char *fbase, bool create){
  if (create) {
    dbf = std::shared_ptr<void>(DBFCreate(fbase), DBFClose);
    if (dbf == NULL) throw Err() <<
        "Dbf: can't create dbf file: " << fbase << ".dbf" ;
  }
  else {
    dbf = std::shared_ptr<void>(DBFOpen(fbase, "rb+"), DBFClose);
    if (dbf == NULL) throw Err() <<
      "Dbf: can't open dbf file: " << fbase << ".dbf" ;
  }
}

int
Dbf::Impl::nfields() const {
  return DBFGetFieldCount((DBFHandle)dbf.get());
}

int
Dbf::Impl::nrecords() const {
  return DBFGetRecordCount((DBFHandle)dbf.get());
}

char
Dbf::Impl::field_type(int fid) const {
  return DBFGetNativeFieldType((DBFHandle)dbf.get(), fid);
}

int
Dbf::Impl::field_find(const char * name) const {
  return DBFGetFieldIndex((DBFHandle)dbf.get(), name);
}

int
Dbf::Impl::add_str_field(const char *name, int size) {
   return DBFAddField((DBFHandle)dbf.get(), name, FTString, size, 0);
}

bool
Dbf::Impl::put_str(int id, int fid, const char * val) {
   return DBFWriteStringAttribute((DBFHandle)dbf.get(), id, fid, val);
}

std::string
Dbf::Impl::get_str(int id, int fid){
  const char * str = DBFReadStringAttribute((DBFHandle)dbf.get(), id, fid);
  return std::string(str? str:"");
}
