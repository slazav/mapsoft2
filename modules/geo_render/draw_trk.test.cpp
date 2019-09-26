///\cond HIDDEN (do not show this in Doxyden)

#include "geo_data/geo_io.h"
#include "getopt/getopt.h"
#include "geo_data/geo_io.h"

#include "draw_pulk_grid.h"

#include "write_geoimg.h"

using namespace std;

GetOptSet options;

void usage(bool pod=false, ostream & S = cout){
  HelpPrinter pr(S, pod, options, "draw_trk");
  pr.name("draw_trk test program");
  pr.usage("[<options>] <input files>");
  pr.head(1, "General options:");
  pr.opts(MS2OPT_STD);
  pr.head(1, "Options for making map reference:");
  pr.opts(MS2OPT_MKREF);
  pr.head(1, "Options for drawing tracks:");
  pr.opts(MS2OPT_DRAWTRK);
  pr.head(1, "Options for drawing waypoints:");
  pr.opts(MS2OPT_DRAWWPT);
  pr.head(1, "Options for drawing map grid:");
  pr.opts(MS2OPT_DRAWGRD);
  pr.head(1, "Options for reading geodata:");
  pr.opts(MS2OPT_GEO_I | MS2OPT_GEO_IO);
  pr.head(1, "Options for writing map in OziExplorer format:");
  pr.opts(MS2OPT_GEO_O);
  throw Err();
}

int
main(int argc, char **argv){
  try {

    options.add("grid", 0,0, MS2OPT_DRAWGRD, "draw grid");
    options.add("map",  1,'m', MS2OPT_GEO_O, "write map file in OziExprorer format");
    options.add("map",  1,'m', MS2OPT_GEO_O, "output file, or \"view\"");

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
    GeoData data;
    for (auto const &f:files) read_geo(f, data, opts);
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

