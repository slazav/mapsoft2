///\cond HIDDEN (do not show this in Doxyden)

//#include <string>
//#include <vector>

#include "getopt/getopt.h"
#include "getopt/help_printer.h"
#include "image/io.h"
#include "image/image_colors.h"

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

  pr.head(1, "Filter order");
  pr.par(
    "filters are applied in the following order: invert, autolevel, crop, autocrop.");

  throw Err();
}


int
main(int argc, char *argv[]){
  try{
    ms2opt_add_std(options, {"HELP","POD","VERB"});
    ms2opt_add_image(options);

    options.add("scale",        1,0, "IMAGE", "Downscaling factor, double value (default: 1.0)");
    options.add("invert",       0,0, "IMAGE", "Invert image");
    options.add("autolevel",    1,0, "IMAGE", "Do auto color levels. Color components are transformed as f(x) = a-c/(a-x) "
                                              "in such a way that the darkest color becomes black, lightest one becomes white, "
                                              "average color becomes the value set by the option argument. "
                                              "Argument is one floating-point number (for all components) or three "
                                              "comma-separated numbers in the range 0..1, reasonable value is near 0.5. "
                                              "If --invert option is set then the transformation happens after the inversion. "
                                              "Color min/max/avr values are calculated in the central part determined "
                                              "by the --border option.");
    options.add("autolevel_th", 1,0, "IMAGE", "Skip some fraction on white/black colors in the autolevel calculation. "
                                              "Argument is one or two (for white and black) numbers in the range 0..1. "
                                              "Default is 0,0.");
    options.add("crop",         1,0, "IMAGE", "Crop image to a rectangular area (intersection with the image coordinate range). "
                                              "Argument is json array with 4 numbers: [<x>,<y>,<w>,<h>]");
    options.add("autocrop",     0,0, "IMAGE", "Crop image automatically, remove all 'bad' lines from each side "
                                              "within region specified by --border.");
    options.add("border",       1,0, "IMAGE", "Border for --autolevel and --autocrop calculations (pixels, default 50)");

    if (argc<2) usage();
    vector<string> files;
    Opt O = parse_options_all(&argc, &argv, options, {}, files);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);

    bool verb = O.exists("verbose");
    auto scale = O.get<double>("scale", 1.0);
    auto brd = O.get<unsigned int>("border", 50);

    if (files.size() != 2) usage();

    // load file
    auto img = image_load(files[0], scale, O);

    // invert filter
    if (O.exists("invert")) image_invert(img);

    // autolevel filter
    if (O.exists("autolevel")){
      auto mm = str_to_type_dvec(O.get("autolevel", "0.51,0.55,0.59"));
      auto tt = str_to_type_dvec(O.get("autolevel_th", "0.00, 0.00"));
      if (mm.size()==1) {mm.push_back(mm[0]); mm.push_back(mm[0]);}
      if (mm.size()!=3) throw Err() << "argument of --autolevel option should contain three float numbers";
      if (mm.size()==1) {tt.push_back(tt[0]);}
      if (tt.size()!=2) throw Err() << "argument of --autolevel_th option should contain two float numbers";
      image_autolevel(img, brd, mm[0], mm[1], mm[2], tt[0], tt[1]);
    }

    // crop filter
    if (O.exists("crop")){
      iRect crop = O.get<iRect>("crop");
      img = image_crop(img, crop);
    }

    // autocrop filter
    if (O.exists("autocrop")){
      iRect crop = image_autocrop(img, brd, 0.8);
      img = image_crop(img, crop);
    }

    // save file
    image_save(img, files[1], O);
  }

  catch(Err & e){
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond
