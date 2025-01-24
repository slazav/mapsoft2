///\cond HIDDEN (do not show this in Doxyden)
// Program for working with map database

#include <unistd.h>
#include <cassert>
#include <iostream>
#include <string>
#include <memory>

#include "err/err.h"
#include "getopt/getopt.h"
#include "getopt/help_printer.h"

#include "vmap2/vmap2.h"
#include "vmap2/vmap2io.h"
#include "vmap2/vmap2tools.h"
#include "geo_data/geo_utils.h"
#include "geo_data/geo_mkref.h"
#include "fig_geo/fig_geo.h"
#include "fig_opt/fig_opt.h"
#include "filename/filename.h"

#include "getopt/action_prog.h"


/**********************************************************/
// print list of types
class ActionListTypes : public Action{
public:
  ActionListTypes():
    Action("list", "list all known types"){
    ms2opt_add_std(options, {"OUT"});
    ms2opt_add_vmap2t(options);
    options.add("mode", 1, 'm', "VMAP2", "Print mode: minimal, names (default), full");
  }

  void help_impl(HelpPrinter & pr) override {
    pr.usage("[<options>]");
    pr.head(2, "Options:");
    pr.opts({"OUT", "VMAP2"});
  }


  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    auto mode = opts.get("mode", "names");
    for (const auto & t: VMap2types(opts)){
      if (mode == "minimal"){
        std::cout << VMap2obj::print_type(t.first) << "\n";
        continue;
      }
      if (mode == "names"){
        std::cout << VMap2obj::print_type(t.first)
                  << " " << t.second.name << "\n";
        continue;
      }
      if (mode == "full"){
        std::cout << "type:      " << VMap2obj::print_type(t.first) << "\n";
        std::cout << "group:     " << t.second.group    << "\n";
        std::cout << "name:      " << t.second.name     << "\n";
        std::cout << "name_ru:   " << t.second.name_ru  << "\n";
        std::cout << "comm:      " << t.second.comm     << "\n";
        std::cout << "fig_mask:  " << t.second.fig_mask << "\n";

        std::cout << "mp_start:  " << t.second.mp_start  << "\n";
        std::cout << "mp_end:    " << t.second.mp_end    << "\n";
        std::cout << "typ_order:        " << t.second.typ_order << "\n";
        std::cout << "typ_line_width:   " << t.second.typ_line_width << "\n";
        std::cout << "typ_border_width: " << t.second.typ_border_width << "\n";

        // Information for updating map labels
        std::cout << "label_type: " << t.second.label_type << "\n";
        std::cout << "label_def_scale:  " << t.second.label_def_scale << "\n";
        std::cout << "label_def_align:  " << t.second.label_def_align << "\n";
        std::cout << "label_def_mshift: " << t.second.label_def_mshift << "\n";
        std::cout << "label_maxnum:     " << t.second.label_maxnum << "\n";
        std::cout << "\n";
        continue;
      }
      throw Err() << "unknown pront mode: " << mode;
    }
  }
};

/**********************************************************/
// make fig library
class ActionFigLib : public Action{
public:
  ActionFigLib():
    Action("fig_lib", "make fig library using typeinfo file"){
    ms2opt_add_std(options, {"OUT"});
    ms2opt_add_vmap2t(options);
  }

  void help_impl(HelpPrinter & pr) override {
    pr.usage("[<options>]");
    pr.head(2, "Options:");
    pr.opts({"OUT", "VMAP2"});
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    std::string odir = opts.get("out", "");
    if (odir == "") throw Err()
      << "non-empty output directory expected (use -o option)";

    // make directory if needed
    file_mkdir(odir);

    // read file with type information
    for (const auto & t:VMap2types(opts)){
      if (t.second.fig_mask=="") continue;
      auto cl = VMap2obj::get_class(t.first);
      if (cl==VMAP2_TEXT) continue;
      Fig F;
      FigObj o = figobj_template(t.second.fig_mask);
      o.type=2; o.sub_type=1; // line
      switch (cl){
        case VMAP2_POINT:
        o.set_points(dLine("[[0,0]]"));
        // Picture
        if (t.second.fig_pic.size()){
          std::list<FigObj> pic = t.second.fig_pic;
          for (auto & p:pic) fig_add_opt(p, "MapType", "pic");
          F.insert(F.end(), pic.begin(), pic.end());
        }
        break;
        case VMAP2_LINE:
          o.set_points(dLine("[[0,-30],[470,0],[500,-100]]"));
        break;
        case VMAP2_POLYGON:
          o.sub_type = 3;
          o.set_points(dLine("[[0,0],[30,-100],[470,-70],[500,0],[0,0]]"));
        break;
        default: continue;
      }
      F.push_back(o);

      // save fig file
      std::ostringstream fname;
      fname << odir << "/" << VMap2obj::print_type(t.first) << ".fig";
      write_fig(fname.str(), F);
    }

  }
};


using namespace std;

int
main(int argc, char *argv[]){
  return ActionProg(argc, argv,
    "ms2vmaptypes", "interface to vmap type information", {
    std::shared_ptr<Action>(new ActionListTypes),
    std::shared_ptr<Action>(new ActionFigLib),
  });
}

///\endcond
