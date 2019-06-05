#include <iostream>
#include "vmap2/vmap.h"

void
action_import_mp(int argc, char *argv[], VMap & map){
  std::string name("import_mp");
  if (argc<1) throw Err() << name << ": file name expected";

  // TODO: options, help
  Opt opts;
  map.import_mp(argv[0], opts);

}

void
action_export_mp(int argc, char *argv[], VMap & map){
  std::string name("export_mp");
  if (argc<1) throw Err() << name << ": file name expected";

  // TODO: options, help
  Opt opts;
  map.export_mp(argv[0], opts);

}


void
action_import_vmap1(int argc, char *argv[], VMap & map){
  std::string name("import_vmap1");
  if (argc<1) throw Err() << name << ": file name expected";

  // TODO: options, help
  Opt opts;
  map.import_vmap1(argv[0], opts);

}

void
action_export_vmap1(int argc, char *argv[], VMap & map){
  std::string name("export_vmap1");
  if (argc<1) throw Err() << name << ": file name expected";

  // TODO: options, help
  Opt opts;
  map.export_vmap1(argv[0], opts);

}
