///\cond HIDDEN (do not show this in Doxyden)

#include "getopt/action_prog.h"
#include "fig_geo/fig_geo.h"
#include "fig/fig_utils.h"
#include "geo_mkref/geo_mkref.h"
#include "geo_data/geo_io.h"
#include "geo_data/conv_geo.h"
#include "srtm/srtm.h"
#include "geom/poly_tools.h"
#include "geo_render/gobj_maps.h"

/*****************************************************************/
class ActionCreate : public Action{
public:
  ActionCreate():
    Action("create", "create referenced fig file") {
    ms2opt_add_std(options, {"OUT"});
    ms2opt_add_fig(options);
    ms2opt_add_mkref_opts(options);
    ms2opt_add_mkref_brd(options);
    ms2opt_add_geofig_ref(options);
  }

  void help_impl(HelpPrinter & pr) override {
    pr.usage("[<options>] -o <output file>");
    pr.head(2, "Options for making reference:");
    pr.opts({"MKREF_OPTS", "MKREF_BRD"});
    pr.head(2, "Options for writing fig file:");
    pr.opts({"OUT", "FIG", "GEOFIG_REF"});
    pr.head(1, "Examples:");

    pr.par("Create standard map n30-068 (1km at 1cm) as a FIG file:\n"
           "> ms2geofig create --mkref nom --name n30-068 -o n30-068.fig");
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()>0) throw Err() << "wrong argument: " << args[0];
    if (!opts.exists("out")) throw Err() << "no output files";
    std::string ofile = opts.get<std::string>("out");
    bool v = opts.get("verb", false);

    Opt opts1(opts);
    opts1.put("dpi", 1200/1.05);
    GeoMap ref = geo_mkref_opts(opts1);

    geo_mkref_brd(ref, opts);

    Fig F;
    fig_add_ref(F, ref, opts);
    write_fig(ofile, F, opts);
  }
};

/*****************************************************************/
class ActionAdd : public Action{
public:
  ActionAdd():
    Action("add", "add geodata (tracks, waypoints, maps) to referenced fig file") {

    ms2opt_add_std(options, {"OUT"});
    ms2opt_add_geo_i(options);
    ms2opt_add_geo_io(options);
    ms2opt_add_fig(options);
    ms2opt_add_geofig_data(options);
    ms2opt_add_drawmap(options);
    // replace help message for --map_dir (we use different default)
    options.replace("map_dir",   1,0,"GEOFIG_DATA",
      "Directory for raster map tiles (default <filename>.img).");

  }

  void help_impl(HelpPrinter & pr) override {
    pr.usage("[<options>] <files> -o <output file>");
    pr.head(2, "Options for reading data:");
    pr.opts({"GEO_I", "GEO_IO"});
    pr.head(2, "Options for writing fig file:");
    pr.opts({"OUT", "FIG", "GEOFIG_DATA", "DRAWMAP"});

    pr.head(1, "Examples:");
    pr.par("Add track to the file:\n"
           "> ms2geofig add Current.gpx -o n30-068.fig");
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (!opts.exists("out")) throw Err() << "no output files";
    std::string ofile = opts.get<std::string>("out");
    bool v = opts.get("verb", false);

    GeoData data;
    for (auto const & f:args) read_geo(f, data, opts);
    if (v) std::cerr << ",  Waypoint lists: " << data.wpts.size()
                     << ",  Tracks: " << data.trks.size() << "\n";

    Fig F;
    read_fig(ofile, F, opts);

    Opt opts1(opts);
    opts1.put_missing("map_dir", ofile+".img");

    if (v) std::cerr << "Writing data to " << ofile << "\n";
    GeoMap ref = fig_get_ref(F);
    fig_add_wpts(F, ref, data, opts1);
    fig_add_trks(F, ref, data, opts1);
    fig_add_maps(F, ref, data, opts1);

    write_fig(ofile, F, opts);
  }
};

/*****************************************************************/
class ActionDel : public Action{
public:
  ActionDel():
    Action("del", "delete geodata to referenced fig file") {

    ms2opt_add_std(options, {"OUT"});
    ms2opt_add_fig(options);
    std::string g = "GEOFIG_SEL";
    options.add("wpts", 0,'W',g, "Delete waypoints.");
    options.add("trks", 0,'T',g, "Delete tracks.");
    options.add("maps", 0,'M',g, "Delete maps.");
    options.add("all",  0,'A',g, "Delete waypoints, tracks, maps.");
    options.add("ref", 0,'R',g, "Delete reference.");
  }

  void help_impl(HelpPrinter & pr) override {
    pr.usage("[<options>] -o <output file>");
    pr.head(2, "Options for selecting data:");
    pr.opts({"GEOFIG_SEL"});
    pr.head(2, "Options for writing fig file:");
    pr.opts({"OUT", "FIG"});
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()>0) throw Err() << "wrong argument: " << args[0];
    if (!opts.exists("out")) throw Err() << "no output files";
    std::string ofile = opts.get<std::string>("out");
    bool v = opts.get("verb", false);

    Fig F;
    read_fig(ofile, F, opts);

    bool all = opts.exists("all");
    if (opts.exists("wpts") || all) fig_del_wpts(F);
    if (opts.exists("trks") || all) fig_del_trks(F);
    if (opts.exists("maps") || all) fig_del_maps(F);
    if (opts.exists("ref")) fig_del_ref(F);
    write_fig(ofile, F, opts);
  }
};

/*****************************************************************/
class ActionGet : public Action{
public:
  ActionGet():
    Action("get", "extract geodata from fig file") {

    ms2opt_add_std(options, {"OUT"});
    ms2opt_add_geo_o(options);
    ms2opt_add_geo_io(options);
    std::string g = "GEOFIG_SEL";
    options.add("wpts", 0,'W',g, "Get waypoints.");
    options.add("trks", 0,'T',g, "Get tracks.");
    options.add("maps", 0,'M',g, "Get maps.");
    options.add("all",  0,'A',g, "Get waypoints, tracks, maps.");
    options.add("ref", 0,'R',g, "Get fig reference.");
  }

  void help_impl(HelpPrinter & pr) override {
    pr.usage("[<options>] <fig file> -o <output file>");
    pr.head(2, "Options for selecting data:");
    pr.opts({"GEOFIG_SEL"});
    pr.head(2, "Options for writing geodata file:");
    pr.opts({"OUT", "GEO_O", "GEO_IO"});
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()!=1) throw Err() << "argument expected: fig file";
    if (!opts.exists("out")) throw Err() << "no output files";
    std::string ifile = args[0];
    std::string ofile = opts.get<std::string>("out");
    bool v = opts.get("verb", false);

    Fig F;
    read_fig(ifile, F, opts);
    GeoMap ref = fig_get_ref(F);
    GeoData data;

    bool all = opts.exists("all");
    if (opts.exists("wpts") || all) fig_get_wpts(F, ref, data);
    if (opts.exists("trks") || all) fig_get_trks(F, ref, data);
    if (opts.exists("maps") || all) fig_get_maps(F, ref, data);
    if (opts.exists("ref")){
      GeoMapList ml; ml.push_back(ref);
      data.maps.push_back(ml);
    }

    write_geo(ofile, data, opts);
  }
};

/*****************************************************************/
class ActionSRTM : public Action{
public:
  ActionSRTM():
    Action("srtm", "add DEM data to referenced fig file") {

    ms2opt_add_std(options, {"OUT"});
    ms2opt_add_fig(options);
    ms2opt_add_srtm(options);

    std::string g = "GEOFIG_SRTM";
    options.add("cnt",        1,0,g, "Make contours (default: 1)");
    options.add("cnt_step",   1,0,g, "Contour step, m (default: 100)");
    options.add("cnt_smult",  1,0,g, "Thick contour multiplier (default: 5, every 5th contour is thick)");
    options.add("cnt_minpts", 1,0,g, "Min.number of points in a contour (default: 6)");
    options.add("cnt_templ1", 1,0,g, "FIG template for contours (default: 2 1 0 1 #D0B090 7 90 -1 -1 0.000 1 1 0 0 0)");
    options.add("cnt_templ2", 1,0,g, "FIG template for thick contours (default: 2 1 0 2 #D0B090 7 90 -1 -1 0.000 1 1 0 0 0)");
    options.add("scnt",       1,0,g, "Make large slope contours (default: 1)");
    options.add("scnt_minpts",1,0,g, "Min.number of point in a slope contour (default: 5)");
    options.add("scnt_val",   1,0,g, "Threshold value for slope contour (in degrees, default: 35)");
    options.add("scnt_templ", 1,0,g, "FIG template for large slopes (default: 2 3 0 1 24 24 91 -1 -1 0.000 1 1 7 0 0)");
    options.add("smooth",     1,0,g, "Smooth data with a given radius when finding altitude and slope contours (default 0.0 - no smoothing)");
    options.add("peaks",      1,0,g, "Make peaks points (default: 1)");
    options.add("peaks_dh",   1,0,g, " DH parameter for peak finder [m], default - 20.");
    options.add("peaks_ps",   1,0,g, " PS parameter fr peak finder [pts], default - 1000.");
    options.add("peaks_templ",1,0,g, "FIG template for peaks (default: 2 1 0 3 24 7  57 -1 -1 0.000 0 1 -1 0 0)");
    options.add("replace",    0,0,g, "remove existing objects before adding new ones");
  }

  void help_impl(HelpPrinter & pr) override {
    pr.usage("[<options>] -o <output file>");
    pr.head(2, "Options for writing DEM data:");
    pr.opts({"SRTM", "GEOFIG_SRTM"});
    pr.head(2, "Options for writing fig file:");
    pr.opts({"OUT", "FIG"});
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (args.size()>0) throw Err() << "wrong argument: " << args[0];
    if (!opts.exists("out")) throw Err() << "no output files";
    std::string ofile = opts.get<std::string>("out");
    bool v = opts.get("verb", false);

    // get parameters
    bool cnt   = opts.get("cnt",   true);
    bool scnt  = opts.get("scnt",  true);
    bool peaks = opts.get("peaks", true);
    double smooth = opts.get<int>("smooth", 0.0);
    int cnt_step  = opts.get<int>("cnt_step", 100);
    int cnt_smult = opts.get<int>("cnt_smult",  5);
    int cnt_minpts = opts.get<int>("cnt_minpts",  6);
    int scnt_minpts = opts.get<int>("scnt_minpts",  5);
    double scnt_val = opts.get<double>("scnt_val",     35.0);
    auto peaks_dh = opts.get<int>("peaks_dh",   20);
    auto peaks_ps = opts.get<int>("peaks_ps",  1000);
    std::string cnt_templ1 = opts.get("cnt_templ1",
      "2 1 0 1 #D0B090 7 90 -1 -1 0.000 1 1 0 0 0");
    std::string cnt_templ2 = opts.get("cnt_templ2",
      "2 1 0 2 #D0B090 7 90 -1 -1 0.000 1 1 0 0 0");
    std::string scnt_templ = opts.get("scnt_templ",
      "2 3 0 1 24 24 91 -1 -1 0.000 1 1 7 0 0");
    std::string peaks_templ = opts.get("peaks_templ",
      "2 1 0 3 24 7  57 -1 -1 0.000 0 1 -1 0 0");
    bool replace = opts.get("replace",   true);

    double acc=5; // FIG units

    // read fig file
    Fig F;
    read_fig(ofile, F, opts);
    if (v) std::cerr << "Writing data to " << ofile << "\n";

    // Conversion FIG -> WGS
    GeoMap ref = fig_get_ref(F);
    ConvMap cnv(ref);

    // FIG range in WGS coords
    dRect wgs_range = cnv.frw_acc(ref.bbox(), 1e-7);

    // create SRTM interface
    SRTM srtm(opts);

    // Contours
    if (cnt) {
      if (replace){
        fig_remove_templ(F, cnt_templ1);
        fig_remove_templ(F, cnt_templ2);
      }
      if (v) std::cout << "Finding contours\n";
      auto cnt_data = srtm.find_contours(wgs_range, cnt_step, smooth);
      for(auto & c:cnt_data){
        cnv.bck(c.second);
        line_filter_v1(c.second, acc, -1);
        bool isth = c.first%(cnt_step*cnt_smult); // is it a thin contour
        if (v) std::cout << c.first << " ";
        for (const auto & l:c.second){
          if (l.size() < cnt_minpts) continue;
          FigObj fo = figobj_template(isth? cnt_templ1: cnt_templ2);
          fo.comment.push_back(type_to_str(c.first));
          fo.set_points(l);
          F.push_back(fo);
        }
      }
      if (v) std::cout << "\n";
    }

    /// Slope contour
    if (scnt) {
      if (replace)
        fig_remove_templ(F, scnt_templ);

      if (v) std::cout << "Finding areas with large slope: ";
      auto scnt_data = srtm.find_slope_contours(wgs_range, scnt_val, 1, smooth);
      cnv.bck(scnt_data);

      // remove small pieces
      for(auto c = scnt_data.begin(); c!= scnt_data.end(); c++){
        if (c->size() < scnt_minpts) {
          scnt_data.erase(c--);
          continue;
        }
      }

      // reduce number of points
      line_filter_v1(scnt_data, acc, -1);

      // create fig objects
      for(auto c = scnt_data.begin(); c!= scnt_data.end(); c++){
        FigObj fo = figobj_template(scnt_templ);
        fo.set_points(*c);
        F.push_back(fo);
      }
      if (v) std::cout << scnt_data.size() << "\n";
    }

    // Summits
    if (peaks) {
      if (replace)
        fig_remove_templ(F, peaks_templ);

      if (v) std::cout << "Finding summits: ";
      auto peak_data = srtm.find_peaks(wgs_range, peaks_dh, peaks_ps);
      for(auto const & pt:peak_data){
        FigObj fo = figobj_template(peaks_templ);
        dPoint p(flatten(pt.first));
        cnv.bck(p);
        fo.push_back((iPoint)p);
        auto comm = std::string("(") + type_to_str(pt.second) + ")";
        fo.comment.push_back(comm);
        F.push_back(fo);
      }
      if (v) std::cout << peak_data.size() << "\n";
    }

    write_fig(ofile, F, opts);
  }
};

/*****************************************************************/
class ActionMakeRef : public Action{
public:
  ActionMakeRef():
    Action("make_ref", "make fig reference using matching geodata") {

    ms2opt_add_std(options, {"OUT"});
    ms2opt_add_fig(options);
    ms2opt_add_geo_i(options);
    ms2opt_add_geo_io(options);
  }

  void help_impl(HelpPrinter & pr) override {
    pr.usage("[<options>] <files> -o <output file>");

    pr.par("\nFig file should contain track or waypoint objects (lines or points with"
           " TRK <name> or WPT <name> comment) with same name as in the geodata."
           " For each point of such data a reference point is added.");

    pr.par("This can be used to make reference for a map: add raster image witn \"MAP <name>\""
           " comment; optionally draw border with \"BRD <name>\" comment;"
           " draw a line through some points with \"TRK <name1>\" comment;"
           " somewhere else make a track through same points on a referenced"
           " map with <name1> name; do make_ref <track file> -o <fig file>;"
           " extract map reference from fig file using ms2geofig get -M <fig file> -o <map file>.");

    pr.head(2, "Options for reading data:");
    pr.opts({"GEO_I", "GEO_IO"});
    pr.head(2, "Options for writing fig file:");
    pr.opts({"OUT", "FIG"});
  }

  virtual void run_impl(const std::vector<std::string> & args,
                   const Opt & opts) override {

    if (!opts.exists("out")) throw Err() << "no output files";
    std::string ofile = opts.get<std::string>("out");
    bool v = opts.get("verb", false);

    GeoData data;
    for (auto const & f:args) read_geo(f, data, opts);

    // reading Fig file
    Fig F;
    read_fig(ofile, F, opts);

    // remove reference
    fig_del_ref(F);

    bool add = false;

    // replace TRK with reference points
    for (const auto & o:F){
      if (o.comment.size()==0 || o.comment[0].substr(0,3) != "TRK")
        continue;
      std::string name = o.comment[0].substr(4);
      for (const auto & tr:data.trks){
        if (tr.name != name) continue;
        if (v) std::cerr << "matching track: " << name << "\n";

        size_t n=0;
        for (const auto & seg: tr){
          for (const auto & pt: seg){
            if (n>=o.size()) break;
            std::ostringstream st;
            st << "REF " << pt.x << " " << pt.y;

            FigObj o1(o);
            o1.clear();
            o1.comment.clear();
            o1.comment.push_back(st.str());

            o1.push_back(iPoint(o[n].x,o[n].y));
            F.push_back(o1);
            add = true;
            n++;
          }
        }
      }
    }

    // replace WPT with reference points
    for (const auto & o:F){
      if (o.comment.size()==0 ||
          o.comment[0].substr(0,3) != "WPT" ||
          o.size()<1) continue;

      std::string name = o.comment[0].substr(4);
      for (const auto & wl:data.wpts){
        for (const auto & w:wl){
          if (w.name != name) continue;
          if (v) std::cerr << "matching waypoint: " << name << "\n";

          std::ostringstream st;
          st << "REF " << w.x << " " << w.y;

          FigObj o1(o);
          o1.clear();
          o1.comment.clear();
          o1.comment.push_back(st.str());

          o1.push_back(iPoint(o[0].x,o[0].y));
          F.push_back(o1);
          add = true;
        }
      }
    }
    if (!add) throw Err()
      << "can not make reference: no matching tracks or waypoints";

    write_fig(ofile, F, opts);
  }
};
/*****************************************************************/

int
main(int argc, char **argv){
  return ActionProg(argc, argv,
    "ms2geofig", "working with geo-referenced fig files", {
    std::shared_ptr<Action>(new ActionCreate),
    std::shared_ptr<Action>(new ActionAdd),
    std::shared_ptr<Action>(new ActionDel),
    std::shared_ptr<Action>(new ActionGet),
    std::shared_ptr<Action>(new ActionSRTM),
    std::shared_ptr<Action>(new ActionMakeRef)
  });
}


