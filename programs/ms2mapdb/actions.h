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
// delete map databases
class MapActionDelete : public MapAction{
public:
  MapActionDelete(){ }

  std::string get_name() const override {
    return "delete"; }
  std::string get_descr() const override {
    return "delete MapDB databases"; }

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<mapdb_folder>");
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=1) throw Err() << get_name()
      << ": one argument expected: MapDB folder";

    MapDB::delete_db(args[0]);
  }
};

/**********************************************************/
// add object
class MapActionAddObj : public MapAction{
public:
  MapActionAddObj(){
    const char *g = "MAPDB_ADD_OBJ";
    options.add("name",  1,'n',g, "Specify object name.");
    options.add("comm",  1,'c',g, "Specify object comment.");
    options.add("angle", 1,'a',g, "Specify object angle.");
    options.add("scale", 1,'s',g, "Specify object scale.");
    options.add("align", 1,'A',g, "Specify object alignment (NW,N,NE,E,SE,S,SW,C).");
  }

  std::string get_name() const override {
    return "add_obj"; }
  std::string get_descr() const override {
    return "add new object, print id"; }

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<mapdb_folder> <object_type> <coordinates>");
    pr.head(2, "Options");
    pr.opts({"MAPDB_ADDOBJ"});
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=3) throw Err() << get_name()
      << ": three arguments expected: <mapdb_folder> <object_type> <coordinates>";
    MapDB map(args[0], 0);
    MapDBObj obj(args[1]);
    obj.set_coords(args[2]);
    if (opts.exists("name"))  obj.name  = opts.get("name","");
    if (opts.exists("comm"))  obj.comm  = opts.get("comm","");
    if (opts.exists("angle")) obj.angle = opts.get("angle", 0.0);
    if (opts.exists("scale")) obj.scale = opts.get("scale", 0.0);

    std::string align = opts.get("align", "");
    if      (align == "NW") obj.align = MAPDB_ALIGN_NW;
    else if (align == "W")  obj.align = MAPDB_ALIGN_W;
    else if (align == "SW") obj.align = MAPDB_ALIGN_SW;
    else if (align == "S")  obj.align = MAPDB_ALIGN_S;
    else if (align == "SE") obj.align = MAPDB_ALIGN_SE;
    else if (align == "E")  obj.align = MAPDB_ALIGN_E;
    else if (align == "NE") obj.align = MAPDB_ALIGN_NE;
    else if (align == "N")  obj.align = MAPDB_ALIGN_N;
    else if (align == "C")  obj.align = MAPDB_ALIGN_C;
    else if (align != "")   throw Err() << "unknown --align argument: " << align;

    std::cout << map.add(obj) << "\n";
  }
};

/**********************************************************/
// print all object types in the map
class MapActionTypes : public MapAction{
public:
  MapActionTypes(){}

  std::string get_name() const override {
    return "types"; }
  std::string get_descr() const override {
    return "print all object types in the map"; }

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<mapdb_folder>");
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=1) throw Err() << get_name()
      << ": one argument: <mapdb_folder>";
    MapDB map(args[0], 0);
    auto types = map.get_types();
    for (auto const & t:types)
      std::cout << MapDBObj::print_type(t) << "\n";
  }
};

/**********************************************************/
// print bounding box of the map (using geohash data)
class MapActionBBox : public MapAction{
public:
  MapActionBBox(){}

  std::string get_name() const override {
    return "bbox"; }
  std::string get_descr() const override {
    return "print bounding box of the map (using geohash data)"; }

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<mapdb_folder>");
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=1) throw Err() << get_name()
      << ": one argument: <mapdb_folder>";
    MapDB map(args[0], 0);
    std::cout << map.bbox() << "\n";
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
    const char *g = "MAPDB_RENDER";
    options.add("obj_scale", 1,0,g, "Set object scaling.");
    options.add("define",    1,0,g, "definitions.");
    options.add("map_scale", 1,0,g, "Set map scaling (coordinates+objects).");
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


    // If "mkref" option exists build reference using options
    if (opts.exists("mkref")) map.set_ref(geo_mkref(opts)/opts.get("map_scale", 1.0));

    // get map reference
    GeoMap ref = map.get_ref(); // default map reference

    if (ref.empty()) throw Err() << "Map reference is not set";

    write_geoimg(fname, map, ref, opts);
  }
};


#endif
