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

// Command-line option, extention of getopt_long structure
struct GetOptEl{
  std::string name;    ///< see man getopt_long
  int         has_arg; ///< see man getopt_long
  int         val;     ///< see man getopt_long
  int         group;   ///< this setting is used to select group of options
  std::string desc;    ///< description, used in print_options()
};

// Container for GetOptEl
class GetOptSet: public std::vector<GetOptEl> {
public:

  // Add new option if it was not added yet:
  void add(const std::string & name,
           const int has_arg,
           const int val,
           const int group,
           const std::string & desc){
    if (exists(name)) return;
    push_back({name, has_arg, val, group, desc});
  }

  // Add new or replace old option
  void replace(const std::string & name,
           const int has_arg,
           const int val,
           const int group,
           const std::string & desc){
    for (auto & o:*this){
      if (o.name != name) continue;
      o = {name, has_arg, val, group, desc};
      return;
    }
    push_back({name, has_arg, val, group, desc});
  }

  // check if the option exists
  bool exists(const std::string & name) const{
    for (auto const & o:*this)
      if (o.name == name) return true;
    return false;
  }

  // remove option if it exists
  void remove(const std::string & name){
    for (auto i=begin(); i!=end(); ++i)
      if (i->name == name) erase(i);
  }

};

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
#define MS2OPT_IMAGE   1<<11 // image options (see modules/image/image_io)
#define MS2OPT_GEOFLT  1<<12 // geo filters (see modules/geo_data/filters)

// add MS2OPT_STD options
void ms2opt_add_std(GetOptSet & opts);

// add MS2OPT_OUT option
void ms2opt_add_out(GetOptSet & opts);

/********************************************************************/

/**
Main getopt wrapper. Parse cmdline options up to the first non-option
argument or to last_opt. Uses GetOptSet structure. Mask is applied to
the group element of the ext_option structure to select some subset of
options. All options are returned as Opt object. */
Opt parse_options(int *argc, char ***argv,
                  const GetOptSet & ext_options,
                  int mask,
                  const char * last_opt = NULL);


/** Parse mixed options and non-option arguments (for simple programs).*/
Opt
parse_options_all(int *argc, char ***argv,
              const GetOptSet & ext_options,
              int mask, std::vector<std::string> & non_opts);

/********************************************************************/
// Class for formatting help messages and man pages
class HelpPrinter{
private:
  std::ostream & s;
  std::string name_;
  bool pod;
  const GetOptSet & opts_;
  unsigned int printed; // mask for printed options
  bool usage_head; // has usage header been already printed?

public:
  HelpPrinter(std::ostream & s, bool pod, const GetOptSet & opts,
              const std::string & name);

  // print name section
  void name(const std::string & descr);

  // print usage line (header is printed before the first one)
  void usage(const std::string & text);

  // print a group of options
  void opts(unsigned int mask);

  // print header
  void head(int level, const std::string & text);

  // print a paragraph of text
  void par(const std::string & text);

  // finish printing, check if all options have been printed
  ~HelpPrinter();
};

///@}
///@}
#endif
