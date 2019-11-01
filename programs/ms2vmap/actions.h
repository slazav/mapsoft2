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
    virtual void help(bool pod=false){
      std::string fullname = std::string("ms2vmap:") + get_name();
      HelpPrinter pr(pod, options, fullname);
      pr.head(1, get_name() + " -- " + get_descr());
      // We do not want to show it in the help message.
      // Now MS2OPT_STD should be empty. If not, HelpPrinter will
      // print an errror.
      options.remove("help");
      help_impl(pr);
    }

    // implementation-specific part
    virtual void help_impl(const HelpPrinter & pr) const = 0;

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

class MapActionImportMP : public MapAction{
public:
  MapActionImportMP(){ }

  std::string get_name() const override {
    return "import_mp";}
  std::string get_descr() const override {
    return "import MP file to MapDB";}

  void help_impl(const HelpPrinter & pr) const override{ }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=2) throw Err() << get_name()
      << ": two arguments expected: map name, file name";

    // TODO: options, help
    MapDB map(args[0], 1);
    map.import_mp(args[1], opts);
  }
};

/**********************************************************/

class MapActionExportMP : public MapAction{
public:
  MapActionExportMP(){ }

  std::string get_name() const override {
    return "export_mp";}
  std::string get_descr() const override {
    return "export MP file from MapDB";}

  void help_impl(const HelpPrinter & pr) const override{ }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=2) throw Err() << get_name()
      << ": two arguments expected: map name, file name";

    // TODO: options, help
    MapDB map(args[0], 0);
    map.export_mp(args[1], opts);
  }
};

/**********************************************************/

class MapActionImportVMAP : public MapAction{
public:
  MapActionImportVMAP(){ }

  std::string get_name() const override {
    return "import_vmap";}
  std::string get_descr() const override {
    return "import mapsoft1 VMAP file to MapDB";}

  void help_impl(const HelpPrinter & pr) const override{ }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=2) throw Err() << get_name()
      << ": two arguments expected: map name, file name";

    // TODO: options, help
    MapDB map(args[0], 1);
    map.import_vmap(args[1], opts);
  }
};

/**********************************************************/

class MapActionExportVMAP : public MapAction{
public:
  MapActionExportVMAP(){ }

  std::string get_name() const override {
    return "export_vmap";}
  std::string get_descr() const override {
    return "export mapsoft1 VMAP file from MapDB";}

  void help_impl(const HelpPrinter & pr) const override{ }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=2) throw Err() << get_name()
      << ": two arguments expected: map name, file name";

    // TODO: options, help
    MapDB map(args[0], 0);
    map.export_vmap(args[1], opts);
  }
};

/**********************************************************/

class MapActionRender : public MapAction{
public:
  MapActionRender(){ }

  std::string get_name() const override {return "render";}
  std::string get_descr() const override {return "Render image from MapDB";}

  void help_impl(const HelpPrinter & pr) const override{ }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=2) throw Err() << get_name()
      << ": two arguments expected: map name, file name";

    // TODO: options, help
    MapDB map(args[0], 0);
    //...
  }
};


#endif
