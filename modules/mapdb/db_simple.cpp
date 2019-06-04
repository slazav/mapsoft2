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
    std::shared_ptr<void> db;   // database
    std::shared_ptr<void> cur;  // cursor

    Impl(const std::string & fname, bool create);
    ~Impl() {}

   void put(const uint32_t key, const std::string & val);

   std::string get(uint32_t & key, int flags);

};

/**********************************************************/
// Main class methods
DBSimple::DBSimple(const std::string & fname, bool create):
  impl(std::unique_ptr<Impl>(new Impl(fname, create))) { }

void
DBSimple::put(const uint32_t key, const std::string & val){
  impl->put(key, val);}


std::string
DBSimple::get(uint32_t & key){ return impl->get(key, DB_SET);}

std::string
DBSimple::get_first(uint32_t & key){ return impl->get(key, DB_SET_RANGE);}

std::string
DBSimple::get_next(uint32_t & key){ return impl->get(key, DB_NEXT);}

std::string
DBSimple::get_prev(uint32_t & key){ return impl->get(key, DB_PREV);}

std::string
DBSimple::get_last(uint32_t & key){ return impl->get(key, DB_LAST);}


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

  /* open cursor */
  DBC *curp=NULL;
  dbp->cursor(dbp, NULL, &curp, 0);
  if (curp==NULL)
    throw Err() << "db_simple: can't get a cursor";
  cur = std::shared_ptr<void>(curp, bdb_cur_close);
}

void
DBSimple::Impl::put(const uint32_t key, const std::string & val){
  DB  *dbp = (DB*)db.get();
  DBT k = mk_dbt(&key);
  DBT v = mk_dbt(val);
  int ret = dbp->put(dbp, NULL, &k, &v, 0);
    if (ret != 0) throw Err() << "db_simple: " << db_strerror(ret);
}

// Main get function. Uses cursor, supports all flags.
// Set key to 0xFFFFFFFF if nothing is found.
std::string
DBSimple::Impl::get(uint32_t & key, int flags){
  DB  *dbp = (DB*)db.get();
  DBC *dbc = (DBC*)cur.get();
  DBT k = mk_dbt(&key);
  DBT v = mk_dbt();
  int ret = dbc->c_get(dbc, &k, &v, flags);

  if (ret == DB_NOTFOUND) {key = 0xFFFFFFFF; return std::string();}
  if (ret != 0) throw Err() << "db_simple: " << db_strerror(ret);
  if (k.size != sizeof(uint32_t))
    throw Err() << "db_simple: broken database, wrong key size: " << k.size;
  key = *(uint32_t*)k.data;
  if (key==0xFFFFFFFF)
    throw Err() << "db_simple: database overfull (max key reached)";
  return std::string((char*)v.data, (char*)v.data+v.size);
}

