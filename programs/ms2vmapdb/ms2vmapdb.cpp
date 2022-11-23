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
#include "geo_mkref/geo_mkref.h"
#include "fig_geo/fig_geo.h"

#include "getopt/action_prog.h"


/**********************************************************/
// create map database
class ActionCreate : public Action{
public:
  ActionCreate():
    Action("create", "create vmap2 database") {}

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<dbname>");
    pr.par("Create vmap2 database. <dbname> is a database name with or without "
           ".vmap2db extension.");
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {
    if (args.size()!=1) throw Err() << get_name()
      << ": one argument expected: database name";
    VMap2(args[0], true);
  }
};

/**********************************************************/
// delete map database
class ActionDelete : public Action{
public:
  ActionDelete():
    Action("delete", "delete vmap2 database") {}

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<dbname>");
    pr.par("Delete vmap2 database. <dbname> is a database name with or without "
           ".vmap2db extension.");
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {
    if (args.size()!=1) throw Err() << get_name()
      << ": one argument expected: database name";
    VMap2::remove_db(args[0]);
  }
};

/**********************************************************/
// dump geohash database
class ActionDumpGH : public Action{
public:
  ActionDumpGH():
    Action("dump_gh", "dump geohashes for vmap2 database") {}

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<dbname>");
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {
    if (args.size()!=1) throw Err() << get_name()
      << ": one argument expected: database name";

    VMap2 vmap(args[0]);
    vmap.geohash_dump();
  }
};

/**********************************************************/
// rebuild geohash database
class ActionRebuildGH : public Action{
public:
  ActionRebuildGH():
    Action("rebuild_gh", "rebuild geohashes for vmap2 database") {}

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<dbname>");
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {
    if (args.size()!=1) throw Err() << get_name()
      << ": one argument expected: database name";

    VMap2 vmap(args[0]);
    vmap.geohash_rebuild();
  }
};


/**********************************************************/
// add object
class ActionAddObj : public Action{
public:
  ActionAddObj():
      Action("add_obj", "add new object to vmap2 database, print id") {
    const char *g = "MAPDB_ADD_OBJ";
    options.add("name",  1,'n',g, "Specify object name.");
    options.add("comm",  1,'c',g, "Specify object comment.");
    options.add("angle", 1,'a',g, "Specify object angle.");
    options.add("scale", 1,'s',g, "Specify object scale.");
    options.add("align", 1,'A',g, "Specify object alignment (NW,N,NE,E,SE,S,SW,C).");
    options.add("ref_type",  1,0,g, "Specify ref_type.");
    options.add("ref_pt",    1,0,g, "Specify ref_pt.");
  }

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<dbname> <object_type> <coordinates>");
    pr.head(2, "Options");
    pr.opts({"MAPDB_ADD_OBJ"});
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=3) throw Err() << get_name()
      << ": three arguments expected: <dbname> <object_type> <coordinates>";
    VMap2 map(args[0], 0);
    VMap2obj obj(args[1]);
    obj.set_coords(args[2]);
    if (opts.exists("name"))  obj.name  = opts.get("name","");
    if (opts.exists("comm"))  obj.comm  = opts.get("comm","");
    if (opts.exists("angle")) obj.angle = opts.get("angle", 0.0);
    if (opts.exists("scale")) obj.scale = opts.get("scale", 0.0);
    if (opts.exists("align")) obj.align = VMap2obj::parse_align(opts.get("align"));
    if (opts.exists("ref_type")) obj.set_ref_type(opts.get("ref_type"));
    if (opts.exists("ref_pt"))   obj.ref_pt = opts.get("ref_pt", dPoint());
    std::cout << map.add(obj) << "\n";
  }
};

/**********************************************************/
// print all object types in the map
class ActionTypes : public Action{
public:
  ActionTypes():
    Action("types", "print all object types in vmap2 database"){}

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<dbname>");
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=1) throw Err() << get_name()
      << ": one argument expected: <dbname>";
    VMap2 map(args[0], 0);
    auto types = map.get_types();
    for (auto const & t:types)
      std::cout << VMap2obj::print_type(t) << "\n";
  }
};

/**********************************************************/
// print bounding box of the map (using geohash data)
class ActionBBox : public Action{
public:
  ActionBBox():
    Action("bbox", "print bounding box of the map (using geohash data)"){}

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<dbname>");
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=1) throw Err() << get_name()
      << ": one argument expected: <dbname>";
    VMap2 map(args[0], 0);
    std::cout << map.bbox() << "\n";
  }
};

/**********************************************************/
// update labels
class ActionUpdLabels : public Action{
public:
  ActionUpdLabels():
    Action("update_labels", "update labels"){
    ms2opt_add_vmap2t(options);
  }

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<dbname> -t <typeinfo file>");
    pr.head(2, "Options:");
    pr.opts({"VMAP2"});
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=1) throw Err() << get_name()
      << ": one argument expected: <dbname>";

    // Read file with type information if it's available
    VMap2types types;
    if (opts.exists("types")) types.load(opts.get("types"));

    VMap2 map(args[0], 0);
    do_update_labels(map, types);
  }
};

/**********************************************************/
// crop to nomenclature map page
class ActionCropNom : public Action{
public:
  ActionCropNom():
    Action("crop_nom", "crop to nomenclature map page"){}

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<dbname> <nom name>");
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=2) throw Err() << get_name()
      << ": two arguments expected: <dbname> <nom name>";

    VMap2 map(args[0], 0);
    do_crop_rect(map, nom_to_wgs(args[1]));
  }
};

/**********************************************************/
// crop to rectangular range
class ActionCropRect : public Action{
public:
  ActionCropRect():
    Action("crop_rect", "crop to rectangular range") {}

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<dbname> <rectangle>");
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=2) throw Err() << get_name()
      << ": two arguments expected: <dbname> <rectangle>";

    VMap2 map(args[0], 0);
    do_crop_rect(map, dRect(args[1]));
  }
};

/**********************************************************/
// import objects to a database (keep existing objects)
class ActionImport : public Action{
public:
  ActionImport():
    Action("import", "import objects to vmap2 database"){
    ms2opt_add_std(options, {"OUT"});
    ms2opt_add_vmap2t(options);  // VMAP2 group, -t
    ms2opt_add_vmap2(options, 1, 0);
  }

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<file1> <file2> ... -o <dbname> -t <type info file>");
    pr.par("Read map objects in different formats and add to a database.");
    pr.head(2, "Options:");
    pr.opts({"OUT", "VMAP2"});
    pr.head(2, "Options for reading MP and FIG formats:");
    pr.opts({"MP", "FIG"});
    pr.head(2, "Options for reading VMAP format:");
    pr.opts({"VMAP"});
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()<1) throw Err() << get_name()
      << ": at least one input file expected";

    std::string ofile = opts.get("out", "");
    if (ofile == "") throw Err() << get_name()
      << ": non-empty database name expected (use -o option)";

    // Read file with type information if it's available
    VMap2types types;
    if (opts.exists("types")) types.load(opts.get("types"));

    VMap2 vmap2(ofile);
    vmap2_import(args, types, vmap2, opts);
  }
};

/**********************************************************/
// export vector map
class ActionExport : public Action{
public:
  ActionExport():
    Action("export", "export vector map to a different format"){
    ms2opt_add_std(options, {"OUT"});
    ms2opt_add_vmap2t(options);
    ms2opt_add_vmap2(options, 0, 1);
  }

  void help_impl(HelpPrinter & pr) override {
    pr.usage("<dbname> -o <out file> [<options>]");

    pr.par("Supported formats: vmap2, vmap2db, vmap, mp, fig");

    pr.head(2, "Options:");
    pr.opts({"OUT", "VMAP2"});
    pr.head(2, "Options for writing MP format:");
    pr.opts({"MP"});
    pr.head(2, "Options for writing FIG format:");
    pr.opts({"FIG"});
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=1) throw Err() << get_name()
      << ": one argument expected: <dbname>";

    std::string ofile = opts.get("out", "");
    if (ofile == "") throw Err()
      << "non-empty output file expected (use -o option)";

    // read file with type information if it's available
    VMap2types types;
    if (opts.exists("types")) types.load(opts.get("types"));

    VMap2 map(args[0], 0);
    vmap2_export(map, types, ofile, opts);
  }
};

using namespace std;

int
main(int argc, char *argv[]){
  return ActionProg(argc, argv,
    "ms2vmapdb", "working with vector map database", {
    std::shared_ptr<Action>(new ActionCreate),
    std::shared_ptr<Action>(new ActionDelete),
    std::shared_ptr<Action>(new ActionRebuildGH),
    std::shared_ptr<Action>(new ActionDumpGH),
    std::shared_ptr<Action>(new ActionAddObj),
    std::shared_ptr<Action>(new ActionTypes),
    std::shared_ptr<Action>(new ActionBBox),
    std::shared_ptr<Action>(new ActionUpdLabels),
    std::shared_ptr<Action>(new ActionCropNom),
    std::shared_ptr<Action>(new ActionCropRect),
    std::shared_ptr<Action>(new ActionImport),
    std::shared_ptr<Action>(new ActionExport),
  });
}

///\endcond
