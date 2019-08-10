#ifndef MS2VMAP_ACTIONS_H
#define MS2VMAP_ACTIONS__H

#include <vector>
#include <string>
#include "opt/opt.h"

typedef void(action_t)(
  const char * mapname,
  const std::vector<std::string> & action_args,
  const Opt & action_opts
);

action_t action_import_mp, action_export_mp,
         action_import_vmap, action_export_vmap;

#endif
