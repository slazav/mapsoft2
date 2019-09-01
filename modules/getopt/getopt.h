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

/**
Main getopt wrapper. Parse cmdline options up to the first non-option
argument or to last_opt. Use ext_options structure. Mask is applied to
the group element of the ext_option structure to select some subset of
options. All options are returned as Opt object. */
Opt parse_options(int *argc, char ***argv,
                  const ext_option_list & ext_options,
                  int mask,
                  const char * last_opt = NULL);

/** Print options in help/pod format.
Mask is applied to the group element of the ext_option structure.*/
void print_options(const ext_option_list & ext_options,
                   int mask, std::ostream & s, bool pod=false);

/** Parse mixed options and non-option arguments (for simple programs).*/
Opt
parse_options_all(int *argc, char ***argv,
              const ext_option_list & ext_options,
              int mask, std::vector<std::string> & non_opts);


///@}
///@}
#endif
