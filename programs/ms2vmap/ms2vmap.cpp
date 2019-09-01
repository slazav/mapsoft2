///\cond HIDDEN (do not show this in Doxyden)
// Program for working with map database

#include <unistd.h>
#include <cassert>
#include <iostream>
#include <string>

#include "err/err.h"
#include "getopt/getopt.h"
#include "actions.h"

#define OPT_G   1  // general options (-v, -h)
#define OPT_A   2  // action options (mixed for all actions)

ext_option_list options = {
  {"help",                  0,'h', OPT_G|OPT_A, "show help message"},
  {"pod",                   0, 0,  OPT_G|OPT_A, "show this message as POD template"},
};

void usage(bool pod=false, std::ostream & S = std::cout){
  std::string head = pod? "\n=head1 ":"\n";
  const char * prog = "ms2vmap";
  S << prog << " -- working with vector maps\n"
    << head << "Usage:\n"
    << prog << " <vmap> (-h|--help|--pod)\n"
    << prog << " <vmap> <action> (-h|--help|--pod)\n"
    << prog << " <vmap> <action> [<action arguments and options>]\n"
  ;
  S << head << "General options:\n";
  print_options(options, OPT_G, S, pod);
  S << head << "Actions:\n"
    << " * import_mp <name> <options> -- import MP file to the map\n"
    << " * export_mp <name> <options> -- export the map to MP file\n"
    << " * import_vmap <name> <options> -- import VMAP1 file to the map\n"
    << " * export_vmap <name> <options> -- export the map to VMAP1 file\n"
  ;

  throw Err();
}

int
main(int argc, char *argv[]){
  try{

    // general options
    Opt GO = parse_options(&argc, &argv, options, OPT_G, NULL);
    if (GO.exists("help")) usage();
    if (GO.exists("pod"))  usage(true);

    if (argc<2) usage();
    const char *mapname = argv[0];
    std::string action(argv[1]);
    argc-=1; argv+=1; // now argv points to the action name!

    // action arguments and options
    std::vector<std::string> AA;
    Opt AO = parse_options_all(&argc, &argv, options, OPT_A, AA);

    if (action == "import_mp"){ action_import_mp(mapname, AA, AO); return 0; }
    if (action == "export_mp"){ action_export_mp(mapname, AA, AO); return 0; }
    if (action == "import_vmap"){ action_import_vmap(mapname, AA, AO); return 0; }
    if (action == "export_vmap"){ action_export_vmap(mapname, AA, AO); return 0; }

    throw Err() << "ms2vmap: unknown action: " << action;

  }
  catch (Err e) {
    if (e.str()!="") std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
