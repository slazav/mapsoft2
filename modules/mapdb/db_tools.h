#ifndef DB_TOOLS_H
#define DB_TOOLS_H

// common code for BerkleyDB-based databases.

#include <cstring>
#include <set>
#include <db.h>

#include "err/err.h"
#include "geohash/geohash.h"
#include "db_geohash.h"

// close DB handle
void bdb_close(DB *dbp);

// close DBC handle
void bdb_cur_close(DBC *dbc);

// make empty DBT
DBT mk_dbt();

// make DBT from a string
DBT mk_dbt(const std::string & str);

// make DBT from other types
template <typename T>
static DBT mk_dbt(const T * v){
  DBT ret = mk_dbt();
  ret.data = (void *)v;
  ret.size = sizeof(T);
  return ret;
}

#endif