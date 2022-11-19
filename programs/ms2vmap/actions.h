#ifndef VMAP2_ACTIONS
#define VMAP2_ACTIONS

#include <vector>
#include <string>
#include "vmap2/vmap2.h"
#include "vmap2/vmap2io.h"

#include "geo_mkref/geo_mkref.h"
#include "fig_geo/fig_geo.h"

// abstract class
class Action {
  protected:
    GetOptSet options;
  public:

    // constructor: add standard options
    Action(){
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
      // print an error.
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
// create map database
class ActionDBCreate : public Action{
public:
  ActionDBCreate(){ }

  std::string get_name() const override { return "db_create"; }
  std::string get_descr() const override { return "create vmap2 database"; }
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
class ActionDBDelete : public Action{
public:
  ActionDBDelete(){ }

  std::string get_name() const override { return "db_delete"; }
  std::string get_descr() const override { return "delete vmap2 database"; }
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
class ActionDBDumpGH : public Action{
public:
  ActionDBDumpGH(){ }

  std::string get_name() const override { return "db_dump_gh"; }
  std::string get_descr() const override { return "dump geohashes for vmap2 database"; }
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
class ActionDBRebuildGH : public Action{
public:
  ActionDBRebuildGH(){ }

  std::string get_name() const override { return "db_rebuild_gh"; }
  std::string get_descr() const override { return "rebuild geohashes for vmap2 database"; }
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
// import objects to a database (keep existing objects)
class ActionDBImport : public Action{
public:
  ActionDBImport(){
    const char *g = "VMAP2_DBIMPORT";
    options.add("out",   1,'o',g, "Output database name (with or without .vmap2db extension).");
    options.add("types", 1,'t',g, "File with type information"
                                  "(for reading vmap, mp, fig).");
    ms2opt_add_mp(options);  // MP group, reading/writing mp files
    ms2opt_add_fig(options); // FIG group, reading/writing fig files
  }

  std::string get_name() const override { return "db_import"; }
  std::string get_descr() const override { return "import objects to vmap2 database"; }
  void help_impl(HelpPrinter & pr) override {
    pr.usage("<file1> <file2> ... -o <dbname> -t <type info file>");
    pr.par("Read map objects in different formats and add to a database.");
    pr.head(2, "Options:");
    pr.opts({"VMAP2_DBIMPORT"});
    pr.head(2, "Options for reading/writing MP and FIG formats:");
    pr.opts({"MP", "FIG"});
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
    any_to_vmap2(args, types, vmap2, opts);
  }
};


/**********************************************************/
// add object
class ActionDBAddObj : public Action{
public:
  ActionDBAddObj(){
    const char *g = "MAPDB_ADD_OBJ";
    options.add("name",  1,'n',g, "Specify object name.");
    options.add("comm",  1,'c',g, "Specify object comment.");
    options.add("angle", 1,'a',g, "Specify object angle.");
    options.add("scale", 1,'s',g, "Specify object scale.");
    options.add("align", 1,'A',g, "Specify object alignment (NW,N,NE,E,SE,S,SW,C).");
  }

  std::string get_name() const override {
    return "db_add_obj"; }
  std::string get_descr() const override {
    return "add new object to vmap2 database, print id"; }

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
    std::cout << map.add(obj) << "\n";
  }
};

/**********************************************************/
// print all object types in the map
class ActionDBTypes : public Action{
public:
  ActionDBTypes(){}

  std::string get_name() const override {
    return "db_types"; }
  std::string get_descr() const override {
    return "print all object types in vmap2 database"; }

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
class ActionDBBBox : public Action{
public:
  ActionDBBBox(){}

  std::string get_name() const override {
    return "db_bbox"; }
  std::string get_descr() const override {
    return "print bounding box of the map (using geohash data)"; }

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
// convert vector map
class ActionConvert : public Action{
public:
  ActionConvert(){
    ms2opt_add_vmap2io(options);
  }

  std::string get_name() const override { return "convert"; }
  std::string get_descr() const override { return "convert vector maps"; }
  void help_impl(HelpPrinter & pr) override {
    pr.usage("<file1> <file2> ... -o <out file> [<options>]");

    pr.par("Converts collection of vector map objects between different "
           "formats (vmap2, vmap2db, vmap, mp, fig).");

    pr.head(2, "Options:");
    pr.opts({"VMAP2IO"});
    pr.head(2, "Options for reading/writing MP and FIG formats:");
    pr.opts({"MP", "FIG"});
    pr.head(2, "Options for creating FIG reference:");
    pr.opts({"GEOFIG_REF", "MKREF_OPTS", "MKREF_BRD"});
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    vmap2_convert(args, opts);
  }
};

#endif
