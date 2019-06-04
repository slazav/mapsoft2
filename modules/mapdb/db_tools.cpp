#include <cstring>
#include <string>
#include <db.h>

void
bdb_close(DB *dbp){
  if (dbp) dbp->close(dbp,0);
}

void
bdb_cur_close(DBC *curp){
  if (curp) curp->close(curp);
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

