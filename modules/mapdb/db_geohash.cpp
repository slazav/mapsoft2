#include <cstring>
#include <set>
#include <map>
#include <string>
#include <db.h>

#include "err/err.h"
#include "geohash/geohash.h"
#include "db_geohash.h"
#include "db_tools.h"

// Max hash length. 12 gives 0.1m accuracy
#define HASHLEN 12

/**********************************************************/
/**********************************************************/
// GeoHashM

/**********************************************************/
// Implementation class
class GeoHashM::Impl {
  std::multimap<std::string, int> db;

  public:
   void put(const int id, const dRect & range);

   std::set<int> get(const dRect & range);
   std::set<int> get(const std::string & hash0, bool exact);

};

/**********************************************************/
// Main class methods
GeoHashM::GeoHashM():
   impl(std::unique_ptr<Impl>(new Impl())) {}
GeoHashM::~GeoHashM(){}

void
GeoHashM::put(const int id, const dRect & range){ impl->put(id, range);}

std::set<int>
GeoHashM::get(const dRect & range){ return impl->get(range);}


/**********************************************************/
// Implementation class methods

void
GeoHashM::Impl::put(const int id, const dRect & range){
  std::set<std::string> hashes = GEOHASH_encode4(range, HASHLEN);
  for (auto const & h:hashes) db.insert(std::make_pair(h, id));
}

std::set<int>
GeoHashM::Impl::get(const dRect & range){
  std::set<std::string> hashes = GEOHASH_encode4(range, HASHLEN);
  std::set<int> ret;
  std::set<std::string> done;
  for (auto const & h:hashes) {
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
GeoHashM::Impl::get(const std::string & hash0, bool exact){

  std::set<int> ret;
  std::multimap<std::string, int>::const_iterator i;
  for (i = db.lower_bound(hash0); i != db.end(); i++) {
    std::string hash = i->first;
    if (exact && hash!=hash0) break;

    if (hash.size()>=hash0.size() &&
        hash.compare(0,hash0.size(), hash0)==0){
      ret.insert(i->second);
      continue;
    }
    break;
  }
  return ret;
}

/**********************************************************/
/**********************************************************/
// GeoHashDB

/**********************************************************/
// Implementation class
class GeoHashDB::Impl {
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
GeoHashDB::GeoHashDB(const char *fbase, bool create):
  impl(std::unique_ptr<Impl>(new Impl(fbase, create))) { }

void
GeoHashDB::put(const int id, const dRect & range){ impl->put(id, range);}

std::set<int>
GeoHashDB::get(const dRect & range){ return impl->get(range);}

GeoHashDB::~GeoHashDB(){}

/**********************************************************/
// Implementation class methods

GeoHashDB::Impl::Impl(const char *fname, bool create){
  // set flags
  int open_flags = create? DB_CREATE:0;

  /* Initialize the DB handle */
  DB *dbp;
  int ret = db_create(&dbp, NULL, 0);
  if (ret != 0) throw Err() << "db_geohash: " << db_strerror(ret);
  db = std::shared_ptr<void>(dbp, bdb_close);

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

void
GeoHashDB::Impl::put(const int id, const dRect & range){
  std::set<std::string> hashes = GEOHASH_encode4(range, HASHLEN);
  for (auto const & h:hashes) {
    DBT k = mk_dbt(h);
    DBT v = mk_dbt(&id);
    // std::cerr << "PUT [" << h << "] " << id << "\n";
    int ret = ((DB*)db.get())->put((DB*)db.get(), NULL, &k, &v, 0);
    if (ret != 0) throw Err() << "db_geohash: " << db_strerror(ret);
  }
}

std::set<int>
GeoHashDB::Impl::get(const dRect & range){
  std::set<std::string> hashes = GEOHASH_encode4(range, HASHLEN);
  std::set<int> ret;
  std::set<std::string> done;
  for (auto const & h:hashes) {
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
GeoHashDB::Impl::get(const std::string & hash0, bool exact){
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

/**********************************************************/
/**********************************************************/



