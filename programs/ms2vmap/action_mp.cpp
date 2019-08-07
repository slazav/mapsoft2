#include <iostream>
#include "mapdb/mapdb.h"

using namespace std;

void
action_import_mp(MapDB & map, const vector<string> & args, const Opt & opts){
  string name("import_mp");
  if (args.size()<1) throw Err() << name << ": file name expected";

  // TODO: options, help
  map.import_mp(args[0], opts);

}

void
action_export_mp(MapDB & map, const vector<string> & args, const Opt & opts){
  string name("export_mp");
  if (args.size()<1) throw Err() << name << ": file name expected";

  // TODO: options, help
  map.export_mp(args[0], opts);

}


void
action_import_vmap(MapDB & map, const vector<string> & args, const Opt & opts){
  string name("import_vmap");
  if (args.size()<1) throw Err() << name << ": file name expected";

  // TODO: options, help
  map.import_vmap(args[0], opts);

}

void
action_export_vmap(MapDB & map, const vector<string> & args, const Opt & opts){
  string name("export_vmap");
  if (args.size()<1) throw Err() << name << ": file name expected";

  // TODO: options, help
  map.export_vmap(args[0], opts);

}
