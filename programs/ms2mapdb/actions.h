#ifndef MAPDB_ACTIONS
#define MAPDB_ACTIONS

#include <vector>
#include <string>
#include "mapdb/mapdb.h"

// abstract class
class MapAction {
  protected:
    GetOptSet options;
  public:

    // constructor: add standard options
    MapAction(){
      ms2opt_add_std(options);
      options.remove("verbose");
      options.remove("pod");
    }

    // action name
    virtual std::string get_name() const = 0;

    // action short description
    virtual std::string get_descr() const = 0;

    // print help message
    void help(bool pod=false){
      std::string fullname = std::string("ms2vmap ") + get_name();
      HelpPrinter pr(pod, options, fullname);
      pr.head(1, get_name() + " -- " + get_descr());
      // We do not want to show it in the help message.
      // Now MS2OPT_STD should be empty. If not, HelpPrinter will
      // print an errror.
      options.remove("help");
      help_impl(pr);
    }

    // implementation-specific part
    virtual void help_impl(HelpPrinter & pr) = 0;

    // parse options and run the action
    void run(int *argc, char **argv[]){
      std::vector<std::string> args;
      Opt opts = parse_options_all(argc, argv, options, {}, args);
      if (opts.exists("help")) {help(); throw Err();}
      run_impl(args, opts);
    };

    // implementation-specific part
    virtual void run_impl(
      const std::vector<std::string> & args,
      const Opt & opts) = 0;
};

/**********************************************************/
// create map
class MapActionCreate : public MapAction{
public:
  MapActionCreate(){ }

  std::string get_name() const override {
    return "create"; }
  std::string get_descr() const override {
    return "create MapDB map"; }

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<mapdb_folder>");
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=1) throw Err() << get_name()
      << ": one argument expected: MapDB folder";

    MapDB map(args[0], 1);
  }
};

/**********************************************************/
// import MP
class MapActionImportMP : public MapAction{
public:
  MapActionImportMP(){
    ms2opt_add_mapdb_mp_imp(options);
  }

  std::string get_name() const override {
    return "import_mp";}
  std::string get_descr() const override {
    return "import MP file to MapDB";}

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<mapdb_folder> <file_name> <options>");
    pr.opts({"MAPDB_MP_IMP"});
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=2) throw Err() << get_name()
      << ": two arguments expected: MapDB folder, file name";

    MapDB map(args[0]);
    map.import_mp(args[1], opts);
  }
};

/**********************************************************/
// export MP
class MapActionExportMP : public MapAction{
public:
  MapActionExportMP(){
    ms2opt_add_mapdb_mp_exp(options);
  }

  std::string get_name() const override  {
    return "export_mp";}
  std::string get_descr() const override {
    return "export MP file from MapDB";}

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<mapdb_folder> <file_name> <options>");
    pr.opts({"MAPDB_MP_EXP"});
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=2) throw Err() << get_name()
      << ": two arguments expected: MapDB folder, file name";

    MapDB map(args[0]);
    map.export_mp(args[1], opts);
  }
};

/**********************************************************/
// import VMAP
class MapActionImportVMAP : public MapAction{
public:
  MapActionImportVMAP(){
    ms2opt_add_mapdb_vmap_imp(options);
  }

  std::string get_name() const override { return "import_vmap";}
  std::string get_descr() const override {
    return "import mapsoft1 VMAP file to MapDB";}

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<mapdb_folder> <file_name> <options>");
    pr.opts({"MAPDB_VMAP_IMP"});
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=2) throw Err() << get_name()
      << ": two arguments expected: MapDB folder, file name";

    MapDB map(args[0]);
    map.import_vmap(args[1], opts);
  }
};

/**********************************************************/
// export VMAP
class MapActionExportVMAP : public MapAction{
public:
  MapActionExportVMAP(){
    ms2opt_add_mapdb_vmap_exp(options);
  }

  std::string get_name() const override { return "export_vmap";}
  std::string get_descr() const override {
    return "export mapsoft1 VMAP file from MapDB";}

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<mapdb_folder> <file_name> <options>");
    pr.opts({"MAPDB_VMAP_EXP"});
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=2) throw Err() << get_name()
      << ": two arguments expected: map folder, file name";

    // TODO: options, help
    MapDB map(args[0]);
    map.export_vmap(args[1], opts);
  }
};

/**********************************************************/
// Render
#include "geo_render/write_geoimg.h"
#include "geo_mkref/geo_mkref.h"
#include "mapdb/gobj_mapdb.h"

class MapActionRender : public MapAction{
public:

  // TODO: many common code with module/geo_render/write_geoimg
  // one should write a standard function for making a raster image from GObj + GeoMap.

  MapActionRender(){
    ms2opt_add_mkref(options);
    ms2opt_add_mapdb_render(options);
    ms2opt_add_geoimg(options);
    options.remove("img_in_fmt");
    options.remove("img_out_fmt");
  }

  std::string get_name() const override {return "render";}
  std::string get_descr() const override {return "Render image from MapDB";}

  void help_impl(HelpPrinter & pr) override{
    pr.usage("<mapdb_folder> <options>");
    pr.head(2, "Options for rendering MapDB");
    pr.opts({"MAPDB_RENDER"});
    pr.head(2, "Options for making map reference");
    pr.opts({"MKREF"});
    pr.head(2, "Options for saving images");
    pr.opts({"IMAGE", "IMAGE_CMAP", "OUT", "GEO_O"});

  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=1) throw Err() << get_name()
      << ": one argument expected: MapDB folder";

    std::string mapdir = args[0];
    std::string fname  = opts.get("out");

    // open map, make GObj
    GObjMapDB map(mapdir, opts);

    // get reference (todo: make reference from options)
    GeoMap ref = map.get_ref(); // default map reference

    // If "mkref" option exists build reference using options
    if (opts.exists("mkref")) ref = geo_mkref(opts);

    if (ref.empty()) throw Err() << "Map reference is not set";

    write_geoimg(fname, map, ref, opts);
  }
};


#endif
