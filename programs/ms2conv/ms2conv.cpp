///\cond HIDDEN (do not show this in Doxyden)

#include <string>
#include <vector>
#include "getopt/getopt.h"
#include "getopt/help_printer.h"
#include "geo_data/geo_io.h"
#include "geo_data/filters.h"
#include "geo_data/conv_geo.h"
#include <cstring>

using namespace std;

GetOptSet options;

void usage(bool pod=false){
  HelpPrinter pr(pod, options, "ms2conv");
  pr.name("mapsoft2 converter for geodata and raster maps");
  pr.usage("<options> <input files> -o <output file>");
  pr.head(1, "Description");

  pr.par(
    "ms2conv reads geodata from files (OziExplorer, GPX, KML, GeoJSON, "
    "GarminUtils, ZIP formats are supported). Then it applies filters and "
    "saves data to the output file.");

  pr.head(1, "General options");
  pr.opts({"HELP","POD","VERB","OUT"});
  pr.head(1, "Geodata input/output options");
  pr.opts({"GEO_I", "GEO_IO", "GEO_O"});
  pr.head(1, "Geodata filtering options");
  pr.opts({"GEOFLT"});

  throw Err();
}


int
main(int argc, char *argv[]){
  try{
    ms2opt_add_std(options, {"HELP","POD","VERB","OUT"});
    ms2opt_add_geo_i(options);
    ms2opt_add_geo_o(options);
    ms2opt_add_geo_io(options);
    ms2opt_add_geoflt(options);

    if (argc<2) usage();
    vector<string> infiles;
    Opt O = parse_options_all(&argc, &argv, options, {}, infiles);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);
    bool verb = O.exists("verbose");

    GeoData data;
    for (auto const & f:infiles) read_geo(f, data, O);

    geo_filters(data, O);

    // write output file if needed
    std::string ofile = O.get("out", "");
    if (ofile == "") return 0;

    // write geodata
    write_geo(ofile, data, O);

    return 0;
  }

  catch(Err & e){
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond
