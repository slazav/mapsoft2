///\cond HIDDEN (do not show this in Doxyden)

//#include <string>
//#include <vector>

#include "getopt/getopt.h"
#include "getopt/help_printer.h"
#include "image/io.h"

using namespace std;

GetOptSet options;

void usage(bool pod=false){
  HelpPrinter pr(pod, options, "ms2img");
  pr.name("mapsoft2 converter for raster images");
  pr.usage("<options> <input file> <output file>");
  pr.head(1, "Description");

  pr.par(
    "ms2conv converts raster images (PNG, TIFF, GIF, JPG). "
    "Can be used to change image format, colormap or scale.");

  pr.head(1, "General options");
  pr.opts({"HELP","POD","VERB"});

  pr.head(2, "Options for saving images");
  pr.opts({"IMAGE", "IMAGE_CMAP"});

  throw Err();
}


int
main(int argc, char *argv[]){
  try{
    ms2opt_add_std(options, {"HELP","POD","VERB"});
    ms2opt_add_image(options);

    options.add("scale", 1,0, "IMAGE", "Downscaling factor, double value (default: 1.0)");

    if (argc<2) usage();
    vector<string> files;
    Opt O = parse_options_all(&argc, &argv, options, {}, files);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);

    bool verb = O.exists("verbose");
    double scale = O.get("scale", 1.0);

    if (files.size() != 2) usage();
    auto img = image_load(files[0], scale, O);
    image_save(img, files[1], O);
  }

  catch(Err & e){
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond
