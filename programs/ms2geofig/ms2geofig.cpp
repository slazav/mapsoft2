///\cond HIDDEN (do not show this in Doxyden)

#include "getopt/getopt.h"
#include "fig_geo/fig_geo.h"
#include "geo_mkref/geo_mkref.h"
#include "geo_data/geo_io.h"


using namespace std;
GetOptSet options;

void usage(bool pod=false){
  HelpPrinter pr(pod, options, "ms2geofig");
  pr.name("greate geo-referenced fig files, add geodata to them");
  pr.usage("create [<options>] -o <output file>");
  pr.usage("add [<options>] <files> -o <fig file>");
  pr.usage("del [<options>] -o <fig file>");

  pr.head(1, "General options:");
  pr.opts({"STD", "OUT", "FIG"});

  pr.head(1, "Making reference and writing it to FIG file in `create` action:");
  pr.opts({"MKREF_OPTS", "MKREF_BRD", "GEOFIG_REF"});

  pr.head(1, "Reading geodata (`add` action):");
  pr.opts({"GEO_I", "GEO_IO"});

  pr.head(1, "Delete sertain components of GeoFIG file (`del` action):");
  pr.opts({"GEOFIG_DEL"});
  pr.par("If no options are given then delete everything.");

  pr.head(1, "Writing tracks and waypoints to FIG file (`add` action):");
  pr.opts({"GEOFIG_DATA"});

  throw Err();
}


int
main(int argc, char **argv){
  try {

    ms2opt_add_std(options);
    ms2opt_add_out(options);
    ms2opt_add_fig(options);
    ms2opt_add_geofig_ref(options);
    ms2opt_add_mkref_opts(options);
    ms2opt_add_mkref_brd(options);
    ms2opt_add_geo_i(options);
    ms2opt_add_geo_io(options);

    options.add("wpts", 0,0,"GEOFIG_DEL", "Delete only waypoints.");
    options.add("trks", 0,0,"GEOFIG_DEL", "Delete only tracks.");
    options.add("maps", 0,0,"GEOFIG_DEL", "Delete only maps.");
    options.add("ref", 0,0, "GEOFIG_DEL", "Delete only reference.");

    Opt O = parse_options(&argc, &argv, options, {"STD"});
    if (O.exists("help")) {usage(); return 0;}
    if (O.exists("pod")) {usage(true); return 0;}

    if (argc<1) usage();

    vector<string> pars;
    string action = argv[0];

    /*****************************************/
    if (action == "create"){

      Opt O = parse_options_all(&argc, &argv, options,
        {"STD", "OUT", "FIG", "GEOFIG_REF", "MKREF_OPTS", "MKREF_BRD"}, pars);
      if (O.exists("help")){ usage(); return 0;}
      if (O.exists("pod")) { usage(true); return 0;}
      if (!O.exists("out"))
        throw Err() << "no output files";
      O.put("dpi", 1200/1.05);
      Fig F;
      GeoMap ref = geo_mkref_opts(O);
      geo_mkref_brd(ref, O);

      fig_add_ref(F, ref, O);
      write_fig(O.get<string>("out"), F, O);
      return 0;
    }
    /*****************************************/

    if (action == "add"){
      Opt O = parse_options_all(&argc, &argv, options,
        {"STD", "OUT", "GEO_I", "GEO_IO", "FIG", "GEOFIG_DATA"}, pars);
      if (O.exists("help")) {usage(); return 0;}
      if (O.exists("pod")) {usage(true); return 0;}
      if (!O.exists("out"))
        throw Err() << "no output files";
      bool v = O.exists("verbose");

      GeoData data;
      for (auto const & f:pars) read_geo(f, data, O);

      if (v) cerr << ",  Waypoint lists: " << data.wpts.size()
                  << ",  Tracks: " << data.trks.size() << "\n";
      string ofile = O.get<string>("out");

      Fig F;
      read_fig(ofile, F, O);

      if (v) cerr << "Writing data to " << ofile << "\n";
      GeoMap ref = fig_get_ref(F);
      fig_add_wpts(F, ref, data, O);
      fig_add_trks(F, ref, data, O);

      write_fig(ofile, F, O);
      return 0;
    }

    /*****************************************/

    if (action == "del"){
      Opt O = parse_options_all(&argc, &argv, options,
        {"STD", "OUT", "GEOFIG_DEL"}, pars);
      if (O.exists("help")) {usage(); return 0;}
      if (O.exists("pod")) {usage(true); return 0;}

      if (argc>0) throw Err() << "wrong argument: " << argv[0];
      if (!O.exists("out")) throw Err() << "no output files";

      string ofile = O.get<string>("out");

      Fig F;
      read_fig(ofile, F, O);

      bool all = !O.exists("wpts") && !O.exists("trks") &&
                 !O.exists("maps") && !O.exists("ref");
      if (O.exists("wpts") || all) fig_del_wpts(F);
      if (O.exists("trks") || all) fig_del_trks(F);
      if (O.exists("maps") || all) fig_del_maps(F);
      if (O.exists("ref")  || all) fig_del_ref(F);
      write_fig(ofile, F, O);
      return 0;
    }

    /*****************************************/
    else throw Err() << "Unknown action: " << action;

  }
  catch(Err & e){
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}


