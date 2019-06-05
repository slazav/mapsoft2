#include <iostream>
#include "vmap2/vmap.h"

using namespace std;

void
action_import_mp(VMap & map, const vector<string> & args, const Opt & opts){
  string name("import_mp");
  if (args.size()<1) throw Err() << name << ": file name expected";

  // TODO: options, help
  map.import_mp(args[0], opts);

}

void
action_export_mp(VMap & map, const vector<string> & args, const Opt & opts){
  string name("export_mp");
  if (args.size()<1) throw Err() << name << ": file name expected";

  // TODO: options, help
  map.export_mp(args[0], opts);

}


void
action_import_vmap1(VMap & map, const vector<string> & args, const Opt & opts){
  string name("import_vmap1");
  if (args.size()<1) throw Err() << name << ": file name expected";

  // TODO: options, help
  map.import_vmap1(args[0], opts);

}

void
action_export_vmap1(VMap & map, const vector<string> & args, const Opt & opts){
  string name("export_vmap1");
  if (args.size()<1) throw Err() << name << ": file name expected";

  // TODO: options, help
  map.export_vmap1(args[0], opts);

}
