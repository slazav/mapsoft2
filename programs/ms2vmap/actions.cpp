#include <iostream>
#include "mapdb/mapdb.h"

using namespace std;

void
action_import_mp(const char *mapname,const vector<string> & args, const Opt & opts){
  string name("import_mp");
  if (args.size()<1) throw Err() << name << ": file name expected";

  // TODO: options, help
  MapDB map(mapname, 1);
  map.import_mp(args[0], opts);

}

void
action_export_mp(const char *mapname,const vector<string> & args, const Opt & opts){
  string name("export_mp");
  if (args.size()<1) throw Err() << name << ": file name expected";

  // TODO: options, help
  MapDB map(mapname, 0);
  map.export_mp(args[0], opts);

}


void
action_import_vmap(const char *mapname,const vector<string> & args, const Opt & opts){
  string name("import_vmap");
  if (args.size()<1) throw Err() << name << ": file name expected";

  // TODO: options, help
  MapDB map(mapname, 1);
  map.import_vmap(args[0], opts);

}

void
action_export_vmap(const char *mapname,const vector<string> & args, const Opt & opts){
  string name("export_vmap");
  if (args.size()<1) throw Err() << name << ": file name expected";

  // TODO: options, help
  MapDB map(mapname, 0);
  map.export_vmap(args[0], opts);

}
