#ifndef MS2VMAP_ACTIONS_H
#define MS2VMAP_ACTIONS__H

typedef void(action_t)(int argc, char *argv[], VMap & map);
action_t action_import_mp, action_export_mp, action_cat_mp;

#endif
