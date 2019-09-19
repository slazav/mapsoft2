#ifndef GETOPT_H
#define GETOPT_H

#include <getopt.h>
#include <vector>
#include <string>
#include "opt/opt.h"

///\addtogroup libmapsoft
///@{

///\defgroup Getopt Mapsoft getopt wrapper.
///@{


/********************************************************************/

/** Extention of getopt_long structure which contains option
description and group number which is used for selecting a few sets of options from a
single list. */
struct ext_option {
  std::string name;    ///< see man getopt_long
  int         has_arg; ///< see man getopt_long
  int         val;     ///< see man getopt_long
  int         group;   ///< this setting is used to select group of options
  std::string desc;    ///< description, used in print_options()
};

typedef std::vector<ext_option> ext_option_list;

/********************************************************************/

// define some masks for mapsoft2 options
#define MS2OPT_NONSTD  1<<0  // non-standard program-specific options
#define MS2OPT_STD     1<<1  // standard options (--verbose, --help, --pod)
#define MS2OPT_OUT     1<<2  // output option (-o)
#define MS2OPT_GEO_I   1<<3  // geodata input-only options (see modules/geo_data)
#define MS2OPT_GEO_O   1<<4  // geodata output-only options (see modules/geo_data)
#define MS2OPT_GEO_IO  1<<5  // geodata output and output options (see modules/geo_data)
#define MS2OPT_MKREF   1<<6  // making map reference (see modules/geo_ref)
#define MS2OPT_DRAWTRK 1<<7  // drawing tracks (see modules/geo_render/gobj_trk)
#define MS2OPT_DRAWWPT 1<<8  // drawing waypoints (see modules/geo_render/gobj_wpt)
#define MS2OPT_DRAWMAP 1<<9  // drawing maps (see modules/geo_render/gobj_map)
#define MS2OPT_DRAWGRD 1<<10 // drawing map grids (see modules/geo_render/draw_grid)

// add MS2OPT_STD options
void ms2opt_add_std(ext_option_list & opts);

// add MS2OPT_OUT option
void ms2opt_add_out(ext_option_list & opts);

/********************************************************************/

/**
Main getopt wrapper. Parse cmdline options up to the first non-option
argument or to last_opt. Use ext_options structure. Mask is applied to
the group element of the ext_option structure to select some subset of
options. All options are returned as Opt object. */
Opt parse_options(int *argc, char ***argv,
                  const ext_option_list & ext_options,
                  int mask,
                  const char * last_opt = NULL);


/** Parse mixed options and non-option arguments (for simple programs).*/
Opt
parse_options_all(int *argc, char ***argv,
              const ext_option_list & ext_options,
              int mask, std::vector<std::string> & non_opts);

/** Print options in help/pod format.
Mask is applied to the group element of the ext_option structure.*/
void print_options(const ext_option_list & ext_options,
                   int mask, std::ostream & s, bool pod=false);

///@}
///@}
#endif
