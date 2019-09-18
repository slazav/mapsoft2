///\cond HIDDEN (do not show this in Doxyden)

#include "mapsoft_data/mapsoft_data.h"
#include "getopt/getopt.h"
#include "geo_data/geo_io.h"

#include "draw_pulk_grid.h"

#include "write_geoimg.h"

using namespace std;

ext_option_list options = {
  {"grid", 0,0, MS2OPT_DRAWGRD, "draw grid"},
  {"map",  1,'m', MS2OPT_GEO_O, "write map file in OziExprorer format"},
  {"map",  1,'m', MS2OPT_GEO_O, "output file, or \"view\""},
};

void usage(bool pod=false, ostream & S = cout){
  string head = pod? "\n=head1 ":"\n";
  const char * prog = "draw_trk";
  S << prog << " -- draw_trk test program\n"
    << head << "Usage:\n"
    << prog << "  [<options>] <input files>\n"
    << "\n";
  S << head << "General options:\n";
  print_options(options, MS2OPT_STD, S, pod);
  S << head << "Options for making map reference:\n";
  print_options(options, MS2OPT_MKREF, S, pod);
  S << head << "Options for drawing tracks:\n";
  print_options(options, MS2OPT_DRAWTRK, S, pod);
  S << head << "Options for drawing waypoints:\n";
  print_options(options, MS2OPT_DRAWWPT, S, pod);
  S << head << "Options for drawing map grid:\n";
  print_options(options, MS2OPT_DRAWGRD, S, pod);
  S << head << "Options for reading geodata:\n";
  print_options(options, MS2OPT_GEO_I | MS2OPT_GEO_IO, S, pod);
  S << head << "Options for writing map in OziExplorer format:\n";
  print_options(options, MS2OPT_GEO_O, S, pod);
  throw Err();
}

int
main(int argc, char **argv){
  try {
    ms2opt_add_std(options);
    ms2opt_add_out(options);
    ms2opt_add_geo_i(options);
    ms2opt_add_geo_io(options);

    if (argc<2) usage();

    std::vector<std::string> files;
    Opt opts = parse_options_all(&argc, &argv, options, ~0, files);

    if (opts.exists("help")) usage();
    if (opts.exists("pod")) usage(true);

    // read geodata
    MapsoftData data;
    for (auto const &f:files) mapsoft_read(f, data, opts);
    std::shared_ptr<Opt> optsp(new Opt(opts));

    // get output file name
    std::string fname = opts.get("out", "");
    if (fname == "")
      throw Err() << "No output file specified (use -o option)";

    if (!write_geoimg(fname, data, opts))
      throw Err() << "write_geoimg: unknown output format";

  }
  catch (Err e) {
    if (e.str()!="") std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond

