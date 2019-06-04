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

   void put(const uint32_t key, const std::string & val);

   std::string get(const uint32_t key);

   uint32_t get_end() const;

};

/**********************************************************/
// Main class methods
DBSimple::DBSimple(const std::string & fname, bool create):
  impl(std::unique_ptr<Impl>(new Impl(fname, create))) { }

void
DBSimple::put(const uint32_t key, const std::string & val){
  impl->put(key, val);}


std::string
DBSimple::get(const uint32_t key){ return impl->get(key);}

uint32_t
DBSimple::get_end() const {return impl->get_end();}


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
DBSimple::Impl::put(const uint32_t key, const std::string & val){
  DB  *dbp = (DB*)db.get();
  DBT k = mk_dbt(&key);
  DBT v = mk_dbt(val);
  int ret = dbp->put(dbp, NULL, &k, &v, 0);
    if (ret != 0) throw Err() << "db_simple: " << db_strerror(ret);
}

std::string
DBSimple::Impl::get(const uint32_t key){
  DB  *dbp = (DB*)db.get();
  DBT k = mk_dbt(&key);
  DBT v = mk_dbt();
  int ret = dbp->get(dbp, NULL, &k, &v, 0);
  if (ret != 0) throw Err() << "db_simple: " << db_strerror(ret);
  return std::string((char*)v.data, (char*)v.data+v.size);
}

uint32_t
DBSimple::Impl::get_end() const{
  uint32_t key = 0xFFFFFFFF;
  DBT k = mk_dbt(&key);
  DBT v = mk_dbt();

  DB  *dbp = (DB*)db.get();
  DBC *curs=NULL;
  dbp->cursor(dbp, NULL, &curs, 0);
  if (curs==NULL)
    throw Err() << "db_simple: can't get a cursor";

  // get last key
  int ret = curs->c_get(curs, &k, &v, DB_LAST);
  curs->close(curs);

  if (ret == DB_NOTFOUND) return 0;

  if (ret != 0)
    throw Err() << "db_simple: " << db_strerror(ret);

  if (k.size != sizeof(uint32_t))
    throw Err() << "db_simple: broken database, wrong key size: " << k.size;

  key = *(uint32_t*)k.data;

  if (key==0xFFFFFFFF)
    throw Err() << "db_simple: database overfull (max key reached)";

  return key+1;
}
