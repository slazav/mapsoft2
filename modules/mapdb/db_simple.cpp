#include <cstring>
#include <string>
#include <db.h>

#include "err/err.h"
#include "db_simple.h"
#include "db_tools.h"

/**********************************************************/
// Implementation class
class DBSimple::Impl {
  public:
    std::shared_ptr<void> db;

    Impl(const std::string & fname, bool create);
    ~Impl() {}

   void put(const int key, const std::string & val);
   std::string get(const int key);

};

/**********************************************************/
// Main class methods
DBSimple::DBSimple(const std::string & fname, bool create):
  impl(std::unique_ptr<Impl>(new Impl(fname, create))) { }

void
DBSimple::put(const int key, const std::string & val){ impl->put(key, val);}


std::string
DBSimple::get(const int key){ return impl->get(key);}

DBSimple::~DBSimple(){}

/**********************************************************/
// Implementation class methods

DBSimple::Impl::Impl(const std::string &fname, bool create){
  // set flags
  int open_flags = create? DB_CREATE:0;

  /* Initialize the DB handle */
  DB *dbp;
  int ret = db_create(&dbp, NULL, 0);
  if (ret != 0) throw Err() << "db_simple: " << db_strerror(ret);
  db = std::shared_ptr<void>(dbp, bdb_close);

  /* Open the database */
  ret = dbp->open(dbp,    /* Pointer to the database */
                  NULL,          /* Txn pointer */
                  fname.c_str(), /* file */
                  NULL,          /* database */
                  DB_BTREE,      /* Database type (using btree) */
                  open_flags,    /* Open flags */
                  0644);         /* File mode*/
  if (ret != 0)
    throw Err() << "db_simple: " << db_strerror(ret);

}

void
DBSimple::Impl::put(const int key, const std::string & val){
  DBT k = mk_dbt(&key);
  DBT v = mk_dbt(val);
  int ret = ((DB*)db.get())->put((DB*)db.get(), NULL, &k, &v, 0);
    if (ret != 0) throw Err() << "db_simple: " << db_strerror(ret);
}

std::string
DBSimple::Impl::get(const int key){
  DBT k = mk_dbt(&key);
  DBT v = mk_dbt();
  int ret = ((DB*)db.get())->get((DB*)db.get(), NULL, &k, &v, 0);
  if (ret != 0) throw Err() << "db_simple: " << db_strerror(ret);
  return std::string((char*)v.data, (char*)v.data+v.size);
}

