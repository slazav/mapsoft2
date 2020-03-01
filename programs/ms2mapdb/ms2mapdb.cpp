///\cond HIDDEN (do not show this in Doxyden)
// Program for working with map database

#include <unistd.h>
#include <cassert>
#include <iostream>
#include <string>
#include <memory>

#include "err/err.h"
#include "getopt/getopt.h"

#include "actions.h"

using namespace std;
GetOptSet options;

// all actions
std::vector<std::shared_ptr<MapAction> > actions = {
  std::shared_ptr<MapAction>(new MapActionCreate),
  std::shared_ptr<MapAction>(new MapActionDelete),
  std::shared_ptr<MapAction>(new MapActionAddObj),
  std::shared_ptr<MapAction>(new MapActionTypes),
  std::shared_ptr<MapAction>(new MapActionBBox),
  std::shared_ptr<MapAction>(new MapActionImportMP),
  std::shared_ptr<MapAction>(new MapActionExportMP),
  std::shared_ptr<MapAction>(new MapActionImportVMAP),
  std::shared_ptr<MapAction>(new MapActionExportVMAP),
  std::shared_ptr<MapAction>(new MapActionRender),
};


/**********************************************************/
void usage(bool pod=false){
  HelpPrinter pr(pod, options, "ms2mapdb");

  pr.name("working with vector maps in MapDB storage");
  pr.usage("(-h|--help|--pod)");
  pr.usage("<action> (-h|--help)");
  pr.usage("<action> [<action arguments and options>]");

  pr.head(1, "General options:");
  pr.opts({"STD"});
  pr.head(1, "Actions:");

  // print list of actions
  for (auto const & a: actions){
    pr.usage(a->get_name() + " <arguments> -- " + a->get_descr());
  }

  // print halp message for each action:
  if (pod){
    for (auto const & a: actions) a->help(pod);
  }

  throw Err();
}
/**********************************************************/

int
main(int argc, char *argv[]){
  try{

    ms2opt_add_std(options);
    options.remove("verbose");

    // general options -- up to first non-option argument
    Opt O = parse_options(&argc, &argv, options, {}, NULL);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);
    if (argc<1) usage();

    // run the action
    for (auto const & a: actions){
      if (a->get_name() == argv[0]) {a->run(&argc, &argv); return 0;}
    }

    throw Err() << "ms2mapdb: unknown action: " << argv[0];

  }
  catch (Err e) {
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
