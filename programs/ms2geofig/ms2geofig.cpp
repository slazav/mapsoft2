///\cond HIDDEN (do not show this in Doxyden)

#include "getopt/action_prog.h"
#include "fig_geo/fig_geo.h"
#include "fig/fig_utils.h"
#include "geo_mkref/geo_mkref.h"
#include "geo_data/geo_io.h"
#include "geo_data/conv_geo.h"
#include "geo_data/geo_utils.h"
#include "srtm/srtm.h"
#include "geom/line_rectcrop.h"
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
    options.add("cnt",        1,0,g, "Make contours, default: 1");
    options.add("cnt_step",   1,0,g, "  Contour step [m], default: 100");
    options.add("cnt_vtol",   1,0,g, "  Tolerance for smoothing contours [m], default: 5.0");
    options.add("cnt_rmin",   1,0,g, "  Radius for smoothing contours [srtm grid units], default: 10.0");
    options.add("cnt_smult",  1,0,g, "  Thick contour multiplier, default: 5, every 5th contour is thick");
    options.add("cnt_minpts", 1,0,g, "  Min.number of points in a closed contour, default: 6");
    options.add("cnt_templ1", 1,0,g, "  FIG template for contours, default: 2 1 0 1 #D0B090 7 90 -1 -1 0.000 1 1 0 0 0");
    options.add("cnt_templ2", 1,0,g, "  FIG template for thick contours, default: 2 1 0 2 #D0B090 7 90 -1 -1 0.000 1 1 0 0 0");
    options.add("scnt",       1,0,g, "Make large slope contours, default: 1");
    options.add("scnt_minpts",1,0,g, "  Min.number of point in a slope contour, default: 5");
    options.add("scnt_val",   1,0,g, "  Threshold value for slope contour, [deg], default: 35");
    options.add("scnt_vtol",  1,0,g, "  Tolerance for smoothing slope contours [deg], default: 5.0");
    options.add("scnt_rmin",  1,0,g, "  Radius for smoothing slope contours [srtm grid units], default: 10.0");
    options.add("scnt_templ", 1,0,g, "  FIG template for large slopes, default: 2 3 0 1 24 24 91 -1 -1 0.000 1 1 7 0 0");
    options.add("peaks",      1,0,g, "Make peaks points, default: 1");
    options.add("peaks_dh",   1,0,g, "  DH parameter for peak finder [m], default: 20");
    options.add("peaks_ps",   1,0,g, "  PS parameter for peak finder [pts], default: 1000");
    options.add("peaks_templ",1,0,g, "  FIG template for peaks, default: 2 1 0 3 24 7  57 -1 -1 0.000 0 1 -1 0 0");
    options.add("riv"     ,   1,0,g, "Trace rivers, default: 0");
    options.add("riv_ps",     1,0,g, "  PS parameter for river tracing [pts], default: 10000");
    options.add("riv_mina",   1,0,g, "  Sink area threshold for river tracing [mk^2], default: 1.0");
    options.add("riv_templ",  1,0,g, "  FIG template for rivers, default: 2 1 0 1 #5066FF 7 86 -1 -1 0.000 1 1 0 0 0");
    options.add("mnt"     ,   1,0,g, "Trace mountain ridges (default: 0)");
    options.add("mnt_ps",     1,0,g, "  PS parameter for ridge tracing [pts], default - 10000.");
    options.add("mnt_mina",   1,0,g, "  Sink area threshold for ridge tracing [mk^2], default: 0.5");
    options.add("mnt_templ",  1,0,g, "  FIG template for ridges, default: 2 1 0 2 26 7 89 -1 -1 0.000 1 1 0 0 0");
    options.add("replace",    0,0,g, "Remove existing objects before adding new ones.");
    options.add("compound",   0,0,g, "Put all new objects into a fig compound.");
    options.add("add_comm",   1,0,g, "Add a comment to all created fig objects.");
    options.add("crop_nom",   1,0,g, "Crop objects to nomenclature region.");
    options.add("crop_rect",  1,0,g, "Crop objects to a rectangle.");
    options.add("line_flt",   1,0,g, "tolerance for RDP line filtering [fig units], default 5.");
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
    bool riv   = opts.get("riv",   false);
    bool mnt   = opts.get("mnt",   false);
    int cnt_step  = opts.get<int>("cnt_step", 100);
    double cnt_smooth_dh  = opts.get<double>("cnt_smooth_dh", 0.0);
    double cnt_smooth_dr  = opts.get<double>("cnt_smooth_dr", 0.0);
    double cnt_vtol = opts.get<double>("cnt_vtol", 5.0);
    double cnt_rmin = opts.get<double>("cnt_rmin", 10.0);
    int cnt_smult = opts.get<int>("cnt_smult",  5);
    int cnt_minpts = opts.get<int>("cnt_minpts",  6);
    int scnt_minpts = opts.get<int>("scnt_minpts",  5);
    double scnt_val = opts.get<double>("scnt_val",     35.0);
    double scnt_vtol = opts.get<double>("scnt_vtol", 5.0);
    double scnt_rmin = opts.get<double>("scnt_rmin", 10.0);
    auto peaks_dh = opts.get<int>("peaks_dh",   20);

    auto peaks_ps = opts.get<int>("peaks_ps",  1000);
    auto riv_ps = opts.get<int>("riv_ps",  10000);
    auto mnt_ps = opts.get<int>("mnt_ps",  10000);

    auto riv_mina = opts.get<double>("riv_mina", 1.0);
    auto mnt_mina = opts.get<double>("mnt_mina", 0.5);

    int riv_sm = 2;
    int mnt_sm = 2;
    int riv_minpt = 3;
    int mnt_minpt = 3;
    double riv_mindh = 40.0;
    double mnt_mindh = 40.0;
    double riv_dist = 2.0;
    double mnt_dist = 2.0;

    std::string cnt_templ1 = opts.get("cnt_templ1",
      "2 1 0 1 #D0B090 7 90 -1 -1 0.000 1 1 0 0 0");
    std::string cnt_templ2 = opts.get("cnt_templ2",
      "2 1 0 2 #D0B090 7 90 -1 -1 0.000 1 1 0 0 0");
    std::string scnt_templ = opts.get("scnt_templ",
      "2 3 0 1 24 24 91 -1 -1 0.000 1 1 7 0 0");
    std::string peaks_templ = opts.get("peaks_templ",
      "2 1 0 3 24 7  57 -1 -1 0.000 0 1 -1 0 0");
    std::string riv_templ = opts.get("riv_templ",
      "2 1 0 1 #5066FF 7 86 -1 -1 0.000 1 1 0 0 0");
    std::string mnt_templ = opts.get("mnt_templ",
      "2 1 0 2 26 7 89 -1 -1 0.000 1 1 0 0 0");

    bool replace = opts.get("replace",   false);
    bool compound  = opts.get("compound", false);
    // comment can contain newline, will be splitted properly when writing fig
    std::string add_comm  = opts.get("add_comm");

    opts.check_conflict({"crop_rect", "crop_nom"});
    dRect crop_rect(opts.get("crop_rect", dRect()));
    if (opts.exists("crop_nom"))  crop_rect = nom_to_wgs(opts.get("crop_nom"));
    auto line_flt = opts.get<double>("line_flt", 5.0);

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

    if (compound) {
      auto fo = figobj_template("6");
      dRect rng = cnv.bck_acc(wgs_range, 1);
      fo.push_back(rng.tlc());
      fo.push_back(rng.brc());
      F.push_back(fo);
    }

    // Contours
    if (cnt) {
      if (v) std::cout << "Finding contours\n";

      // remove old objects
      if (replace){
        fig_remove_templ(F, cnt_templ1);
        fig_remove_templ(F, cnt_templ2);
      }

      // find contours
      auto cnt_data = srtm.find_contours(wgs_range, cnt_step, cnt_vtol, cnt_rmin);

      for(auto & c:cnt_data){
        int v0 = rint(c.first);

        // crop
        if (crop_rect) c.second = rect_crop_multi(crop_rect, c.second, 0);

        // convert wgs->fig
        cnv.bck(c.second);

        // filter points
        line_filter_rdp(c.second, line_flt);

        bool isth = v0%(cnt_step*cnt_smult); // is it a thin contour?
        if (v) std::cout << v0 << " ";

        for (const auto & l:c.second){
          if (l.size() < 2) continue;
          if (l.size() < cnt_minpts && dist(l[0], l[l.size()-1]) < line_flt) continue;
          FigObj fo = figobj_template(isth? cnt_templ1: cnt_templ2);
          fo.comment.push_back(type_to_str(v0));
          if (add_comm!="") fo.comment.push_back(add_comm);
          fo.set_points(l);
          F.push_back(fo);
        }
      }
      if (v) std::cout << "\n";
    }

    /// Slope contour
    if (scnt) {
      if (v) std::cout << "Finding areas with large slope: ";

      // remove old data
      if (replace) fig_remove_templ(F, scnt_templ);

      // find slope contours
      auto scnt_data = srtm.find_slope_contours(wgs_range, scnt_val, scnt_vtol, scnt_rmin);

      // crop
      if (crop_rect) scnt_data = rect_crop_multi(crop_rect, scnt_data, 1);

      // convert wgs -> fig
      cnv.bck(scnt_data);

      // remove small pieces
      for(auto c = scnt_data.begin(); c!= scnt_data.end(); c++){
        if (c->size() < scnt_minpts) {
          scnt_data.erase(c--);
          continue;
        }
      }

      // reduce number of points
      line_filter_rdp(scnt_data, line_flt);

      // join crossing segments
      join_cross(scnt_data);

      // create fig objects
      for(auto c = scnt_data.begin(); c!= scnt_data.end(); c++){
        FigObj fo = figobj_template(scnt_templ);
        fo.set_points(*c);
        if (add_comm!="") fo.comment.push_back(add_comm);
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
        dPoint p(flatten(pt));
        if (crop_rect && !crop_rect.contains(p)) continue;
        cnv.bck(p);
        fo.push_back((iPoint)p);
        auto comm = std::string("(") + type_to_str((int)pt.z) + ")";
        fo.comment.push_back(comm);
        if (add_comm!="") fo.comment.push_back(add_comm);
        F.push_back(fo);
      }
      if (v) std::cout << peak_data.size() << "\n";
    }

    // rivers
    if (riv) {
      if (replace) fig_remove_templ(F, riv_templ);
      if (v) std::cout << "Finding rivers: ";
      auto data = srtm.trace_map(wgs_range, riv_ps, true, riv_mina,
                                 riv_sm, riv_minpt, riv_mindh, riv_dist);

      if (crop_rect) data = rect_crop_multi(crop_rect, data, 0);

      cnv.bck(data);

      line_filter_rdp(data, line_flt);

      for(const auto & l:data){
        FigObj fo = figobj_template(riv_templ);
        if (add_comm!="") fo.comment.push_back(add_comm);
        fo.set_points(l);
        F.push_back(fo);
      }
      if (v) std::cout << data.size() << " segments, " << data.npts() << "points\n";
    }

    // mountain ridges
    if (mnt) {
      if (replace) fig_remove_templ(F, mnt_templ);
      if (v) std::cout << "Finding mountain ridges: ";
      auto data = srtm.trace_map(wgs_range, mnt_ps, false, mnt_mina,
                                 mnt_sm, mnt_minpt, mnt_mindh, mnt_dist);

      if (crop_rect) data = rect_crop_multi(crop_rect, data, 0);

      cnv.bck(data);

      line_filter_rdp(data, line_flt);

      for(const auto & l:data){
        FigObj fo = figobj_template(mnt_templ);
        if (add_comm!="") fo.comment.push_back(add_comm);
        fo.set_points(l);
        F.push_back(fo);
      }
      if (v) std::cout << data.size() << " segments, " << data.npts() << "points\n";
    }

    if (compound) F.push_back(figobj_template("-6"));
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


