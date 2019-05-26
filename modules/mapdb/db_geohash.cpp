#include <cstring>
#include <set>
#include <db.h>

#include "err/err.h"
#include "geohash/geohash.h"
#include "db_geohash.h"

// Max hash length. 12 gives 0.1m accuracy
#define HASHLEN 12

/**********************************************************/
// Implementation class
class DBGeoHash::Impl {
  public:
    std::shared_ptr<void> db;

    Impl(const char *fbase, bool create);
    ~Impl() {}

   void put(const int id, const dRect & range);

   std::set<int> get(const dRect & range);
   std::set<int> get(const std::string & hash0, bool exact);

};

/**********************************************************/
// Main class methods
DBGeoHash::DBGeoHash(const char *fbase, bool create):
  impl(std::unique_ptr<Impl>(new Impl(fbase, create))) { }

void
DBGeoHash::put(const int id, const dRect & range){ impl->put(id, range);}

std::set<int>
DBGeoHash::get(const dRect & range){ return impl->get(range);}

DBGeoHash::~DBGeoHash(){}

/**********************************************************/
// Implementation class methods

void
db_close(DB *dbp){
  if (dbp) dbp->close(dbp,0);
}

DBGeoHash::Impl::Impl(const char *fname, bool create){
  // set flags
  int open_flags = create? DB_CREATE:0;

  /* Initialize the DB handle */
  DB *dbp;
  int ret = db_create(&dbp, NULL, 0);
  if (ret != 0) throw Err() << "db_geohash: " << db_strerror(ret);
  db = std::shared_ptr<void>(dbp, db_close);

  // allow duplicates
  ret = dbp->set_flags(dbp, DB_DUP);
  if (ret != 0) throw Err() << "db_geohash: " << db_strerror(ret);

  /* Open the database */
  ret = dbp->open(dbp,    /* Pointer to the database */
                  NULL,          /* Txn pointer */
                  fname,         /* file */
                  NULL,          /* database */
                  DB_BTREE,      /* Database type (using btree) */
                  open_flags,    /* Open flags */
                  0644);         /* File mode*/
  if (ret != 0)
    throw Err() << "db_geohash: " << db_strerror(ret);

}

// empty DBT
DBT mk_dbt(){
  DBT ret;
  memset(&ret, 0, sizeof(DBT));
  return ret;
}
// make DBT from string
DBT mk_dbt(const std::string & str){
  DBT ret = mk_dbt();
  ret.data = (void *) str.data();
  ret.size = str.length();
  return ret;
}
// make DBT from other types
template <typename T>
static DBT mk_dbt(const T * v){
  DBT ret = mk_dbt();
  ret.data = (void *)v;
  ret.size = sizeof(T);
  return ret;
}

void
DBGeoHash::Impl::put(const int id, const dRect & range){
  std::vector<std::string> hashes = GEOHASH_encode4(range, HASHLEN);
  for (auto h:hashes) {
    DBT k = mk_dbt(h);
    DBT v = mk_dbt(&id);
    // std::cerr << "PUT [" << h << "] " << id << "\n";
    int ret = ((DB*)db.get())->put((DB*)db.get(), NULL, &k, &v, 0);
    if (ret != 0) throw Err() << "db_geohash: " << db_strerror(ret);
  }
}

std::set<int>
DBGeoHash::Impl::get(const dRect & range){
  std::vector<std::string> hashes = GEOHASH_encode4(range, HASHLEN);
  std::set<int> ret;
  std::set<std::string> done;
  for (auto h:hashes) {
    for (int i=0; i<=h.size(); i++) {
      std::string hh = h.substr(0,i);
      if (done.count(hh)) continue; // do not repeat queries with same hash
      bool exact = i < h.size();  // for full hashes look also for smaller regions.
      done.insert(hh);
      // std::cerr << "GET [" << hh << "] " << (i<h.size()) << "\n";
      std::set<int> r = get(hh, i<h.size());
      ret.insert(r.begin(), r.end());
    }
  }
  return ret;
}

std::set<int>
DBGeoHash::Impl::get(const std::string & hash0, bool exact){
  DBT k = mk_dbt(hash0);
  DBT v = mk_dbt();

  std::set<int> ret;

  int fl = DB_SET_RANGE; // first get t >= t1
  DBC *curs = NULL;
  try {
    // get cursor
    ((DB*)db.get())->cursor((DB*)db.get(), NULL, &curs, 0);
    if (curs==NULL) throw Err() << "db_geohash: can't get a cursor";

    while (1){
      // get new record (hash>=hash0)
      int res = curs->c_get(curs, &k, &v, fl);
      if (res == DB_NOTFOUND) break;
      if (res!=0) throw Err() << "db_geohash: " << db_strerror(res);

      // check what we found
      if (v.size != sizeof(int))
        throw Err() << "db_geohash: bad value";
      std::string hash((char*)k.data, (char*)k.data+k.size);
      if (exact && hash!=hash0) break;

      if (hash.size()>=hash0.size() &&
          hash.compare(0,hash0.size(), hash0)==0){
        ret.insert(*(int*)v.data);
        fl=DB_NEXT; // switch to DB_NEXT and repeat
        continue;
      }
      break;
    }
  }
  catch (Err e){
    if (curs) curs->close(curs);
    throw e;
  }
  return ret;
}

