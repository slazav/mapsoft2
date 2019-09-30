#include <cstring>
#include "geo_nom/geo_nom.h"
#include "geom/poly_tools.h"
#include "getopt/getopt.h"

using namespace std;

GetOptSet options;

void usage(bool pod=false){

  HelpPrinter pr(pod, options, "ms2nom");
  pr.name("Soviet nomenclature map calculations");

  pr.usage("[-E] -r <point> -s <scale> -- map at the point");
  pr.usage("[-E] -r <range> -s <scale> -- maps at the range");
  pr.usage("[-E] -n <name> -- map range");
  pr.usage("[-E] -c -n <name> -- map center");
  pr.usage("[-E] -n <name> --shift [x_shift,y_shift] -- adjacent map");
  pr.usage("[-E] -n <name> -s <scale> -- convert map to a different scale");
  pr.usage("[-E] -n <name> -r <range>  -- check if the map touches the range");

  pr.head(1, "Options");
  pr.opts(MS2OPT_NONSTD);
  pr.opts(MS2OPT_STD);

  pr.par(
   "Program \"ms2nom\" does some calculations with standard Soviet "
   "nomenclature map names.");

  pr.par(
    "Option --ext turns on 'extended mode': single sheets (like Q10-001) are allowed "
    "on input and always returned on output; for a single sheet suffix '.<N>x<M>' is "
    "allowed to multiply the range (like n49-001.3x2).");

  pr.par(
    "If both --range and --name options are given "
    "program returns with exit code 0 or 1 depending "
    "on whether the coordinate range intersects with the tile range.");

  pr.par(
    "At the moment combination of --ext and --shift options with such a "
    "\"multiplied\" name returns non-multiplied adjecent sheets. This is not "
    "very useful and maybe changed later.");

  pr.par(
    "Soviet nomenclature maps are drawn in Krassovsky-1942 datum "
    "(+ellps=krass +towgs84=28,-130,-95).");

  pr.par(
    "Supported scales: 1:1000000, 1:500000, 1:200000, 1:100000, 1:50000."
    "Scale can be written in following forms:\n"
    "* 1:1000000, 1:1'000'000, 1000000, 1M, 10km/cm\n"
    "* 1:500000, 1:500'000, 500000, 500k, 5km/cm\n"
    "* 1:200000, 1:200'000, 200000, 100k, 1km/cm\n"
    "* 1:100000, 1:100'000, 100000, 100k, 1km/cm\n"
    "* 1:50000, 1:50'000, 50000, 50k, 500m/cm\n"
  );
  throw Err();
}

int
main(int argc, char **argv){
  try {

    options.add("range",  1,'r',MS2OPT_NONSTD,
      "Show maps which cover a given figure. "
      "Figure is a point, rectangle or line in WGS84 coordinates. "
      "Option --scale should be set.");

    options.add("scale",  1,'s',MS2OPT_NONSTD,
      "Set map scale. "
      "Scale should be set when --range option is used. If used with "
      "--name option then name will be converted to a new scale "
      "instead of printing the range.");

    options.add("name", 1,'n',MS2OPT_NONSTD,
      "Show coordinate range for a given map.");

    options.add("ext", 0,'E',MS2OPT_NONSTD,
      "Use 'extended mode': single sheets (like Q10-001) are allowed "
      "on input and always returned on output; for a single sheet "
      "suffix '.<N>x<M>' is allowed to multiply the range (like n49-001.3x2).");

    options.add("center", 0,'c',MS2OPT_NONSTD,
      "Instead of printing a coordinate range print its central point.");

    options.add("shift", 1,'S',MS2OPT_NONSTD,
      "Shift a map. Should be used with --name option. Not compatable with --range option. "
      "Argument is an array of two integer numbers [dx,dy].");

    ms2opt_add_std(options);
    options.remove("verbose");

    if (argc<2) usage();
    vector<string> nonopt;
    Opt O = parse_options_all(&argc, &argv, options, ~0, nonopt);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);

    if (nonopt.size()) throw Err() << "unexpected value: " << nonopt[0];

    bool ex  = O.get("ext", false);
    bool cnt = O.get("center", false);

    O.check_conflict({"range", "shift"});

    // get map name if needed
    std::string name; // given map name
    dRect range_n; // calculated coordinate range for the name
    if (O.exists("name")){
      name = O.get("name", "");
      nom_scale_t sc;
      range_n = nom_to_range(name, sc, ex);

      iPoint sh = O.get("shift", iPoint());
      if (sh!=iPoint()){
        cout << nom_shift(name, sh, ex) << "\n";
        return 0;
      }

      if (O.exists("scale")){
        nom_scale_t scn = str_to_type<nom_scale_t>(O.get("scale", ""));
        dRect r = expand(nom_to_range(name, sc, ex), -0.001, -0.001);
        set<string> names = range_to_nomlist(r, scn, ex);
        for (auto const & n:names) cout << n << "\n";
        return 0;
      }

      if (!O.exists("range")){
        if (cnt) cout << range_n.cnt() << "\n";
        else     cout << range_n << "\n";
        return 0;
      }
    }


    // get coordinate range if needed
    dRect range; // given coordinate range
    set<string> names_r; // calculated map names for given coordinates
    if (O.exists("range")){

      range = figure_bbox<double>(O.get("range",""));
      if (range.is_empty()) throw Err()
        << "wrong coordinate range: " << O.get("range","");

      if (!O.exists("name")){

        if (!O.exists("scale")) throw Err()
          << "scale is not set";
        nom_scale_t sc = str_to_type<nom_scale_t>(O.get("scale", ""));
        names_r = range_to_nomlist(range, sc, ex);

        for (auto const &n:names_r) cout << n << "\n";
        return 0;
      }
    }

    if (O.exists("name") && O.exists("range"))
      return intersect(range, range_n).is_empty();

    throw Err() << "--name or --range option expected";

  }
  catch (Err e) {
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

