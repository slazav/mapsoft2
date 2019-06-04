///\cond HIDDEN (do not show this in Doxyden)
// Program for working with map database

#include <unistd.h>
#include <cassert>
#include <iostream>
#include <string>

#include "err/err.h"
#include "getopt/getopt.h"
#include "vmap2/vmap.h"
#include "actions.h"

#define OPT_GG   1  // general options (-v, -h)
static struct ext_option options[] = {
  {"help",                  0,'h', OPT_GG, "show help message"},
  {"pod",                   0, 0,  OPT_GG, "show this message as POD template"},
  {0,0,0,0}
};

void usage(bool pod=false, std::ostream & S = std::cout){
  std::string head = pod? "\n=head1 ":"\n";
  const char * prog = "ms2vmap";
  S << prog << " -- working with vector maps\n"
    << head << "Usage:\n"
    << prog << " <vmap> (-h|--help|--pod)\n"
    << prog << " <vmap> <action> [<options>]\n"
  ;
  S << head << "General options:\n";
  print_options(options, OPT_GG, S, pod);
  S << head << "Actions:\n"
    << " * import_mp <name> --\n"
    << " * export_mp <name> --\n"
    << " * cat_mp <name1> <name2> --\n"
  ;

  throw Err();
}

int
main(int argc, char *argv[]){
  try{

    Opt O = parse_options(&argc, &argv, options, OPT_GG, NULL);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);

    if (argc<2) usage();
    std::string mapname(argv[0]);
    std::string action(argv[1]);
    argc-=2; argv+=2;

    VMap map;

std::cerr << ">> " << action << "\n";
    if (action == "import_mp"){
      action_import_mp(argc, argv, map);
      return 0;
    }

    if (action == "export_mp"){
      action_export_mp(argc, argv, map);
      return 0;
    }

    if (action == "cat_mp"){
      action_cat_mp(argc, argv, map);
      return 0;
    }
    throw Err() << "ms2vmap: unknown action: " << action;

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
