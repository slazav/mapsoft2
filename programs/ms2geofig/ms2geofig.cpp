///\cond HIDDEN (do not show this in Doxyden)

#include "getopt/getopt.h"
#include "fig_geo/fig_geo.h"
#include "geo_mkref/geo_mkref.h"
#include "geo_data/geo_io.h"
#include "geo_data/conv_geo.h"
#include "srtm/srtm.h"
#include "geom/poly_tools.h"
#include "geom_tools/line_utils.h"
#include "geo_render/gobj_maps.h"


using namespace std;
GetOptSet options;

void usage(bool pod=false){
  HelpPrinter pr(pod, options, "ms2geofig");
  pr.name("greate geo-referenced fig files, add geodata to them");
  pr.usage("create [<options>] -o <output file>");
  pr.usage("add [<options>] <files> -o <fig file>");
  pr.usage("del [<options>] -o <fig file>");
  pr.usage("srtm [<options>] -o <fig file>");

  pr.head(1, "General options:");
  pr.opts({"STD", "OUT", "FIG"});

  pr.head(1, "Making reference and writing it to FIG file in `create` action:");
  pr.opts({"MKREF_OPTS", "MKREF_BRD", "GEOFIG_REF"});

  pr.head(1, "Reading geodata, writing tracks and waypoints to FIG file (`add` action):");
  pr.opts({"GEO_I", "GEO_IO", "GEOFIG_DATA", "DRAWMAP"});

  pr.head(1, "Delete certain components of GeoFIG file (`del` action):");
  pr.opts({"GEOFIG_DEL"});
  pr.par("If no options are given then delete everything.");

  pr.head(1, "SRTM options (`srtm` actions):");
  pr.par("At the moment objects are added in a rectangular bounding box "
         "of map image, refpoints, and border. I plan to add border cropping later");
  pr.opts({"SRTM"});
  pr.opts({"GEOFIG_SRTM"});

  pr.head(1, "Examples:");

  pr.par("Create standart map sheen n30-068 (1km at 1cm) as a FIG file:\n"
         "> ms2geofig create --mkref nom --name n30-068 -o n30-068.fig");

  pr.par("Add track to the file:\n"
         "> ms2geofig add Current.gpx -o n30-068.fig");

  pr.par("Add objects from SRTM data (20/100m contours, areas with slope > 25deg, peaks):\n"
         "> ms2geofig srtm --srtm_dir ${srtm_dir} -o n30-068.fig\\\n"
         "   --cnt 1 --cnt_step 20 --scnt 1 --scnt_val 25 --peaks 1");

  throw Err();
}


int
main(int argc, char **argv){
  try {

    ms2opt_add_std(options);
    ms2opt_add_out(options);
    ms2opt_add_fig(options);
    ms2opt_add_geofig_ref(options);
    ms2opt_add_geofig_data(options);
    ms2opt_add_mkref_opts(options);
    ms2opt_add_mkref_brd(options);
    ms2opt_add_geo_i(options);
    ms2opt_add_geo_io(options);
    ms2opt_add_srtm(options);
    ms2opt_add_drawmap(options);

    {
      std::string g = "GEOFIG_DEL";
      options.add("wpts", 0,0,g, "Delete only waypoints.");
      options.add("trks", 0,0,g, "Delete only tracks.");
      options.add("maps", 0,0,g, "Delete only maps.");
      options.add("ref", 0,0,g, "Delete only reference.");
    }
    {
      std::string g = "GEOFIG_SRTM";
      options.add("cnt",        1,0,g, "Make contours (default: 1)");
      options.add("cnt_step",   1,0,g, "Contour step, m (default: 100)");
      options.add("cnt_smult",  1,0,g, "Thick contour multiplier (default: 5, every 5th contour is thick)");
      options.add("cnt_minpts", 1,0,g, "Min.number of points in a contour (default: 6)");
      options.add("cnt_templ1", 1,0,g, "FIG template for contours (default: 2 1 0 1 #D0B090 7 90 -1 -1 0.000 1 1 0 0 0)");
      options.add("cnt_templ2", 1,0,g, "FIG template for thick contours (default: 2 1 0 2 #D0B090 7 90 -1 -1 0.000 1 1 0 0 0)");
      options.add("scnt",       1,0,g, "Make large slope contours (default: 1)");
      options.add("scnt_minpts",1,0,g, "Min.number of point in a slope contour (default: 12)");
      options.add("scnt_val",   1,0,g, "Threshold value for slope contour (in degrees, default: 35)");
      options.add("scnt_templ", 1,0,g, "FIG template for large slopes (default: 2 3 0 0 0 24 91 -1 20 0.000 0 0 -1 0 0)");
      options.add("peaks",      1,0,g, "Make peaks points (default: 1)");
      options.add("peaks_dh",   1,0,g, " DH parameter for peak finder [m], default - 20.");
      options.add("peaks_ps",   1,0,g, " PS parameter fr peak finder [pts], default - 1000.");
      options.add("peaks_templ",1,0,g, "FIG template for peaks (default: 2 1 0 3 24 7  57 -1 -1 0.000 0 1 -1 0 0)");
    }

    // replace helsp message for --map_dir (we use different default)
    options.replace("map_dir",   1,0,"GEOFIG_DATA", "Directory for raster map tiles (default <filename>.img).");


    Opt O = parse_options(&argc, &argv, options, {"STD"});
    if (O.exists("help")) {usage(); return 0;}
    if (O.exists("pod")) {usage(true); return 0;}

    if (argc<1) usage();

    vector<string> pars;
    string action = argv[0];

    /*****************************************/
    if (action == "create"){

      Opt O = parse_options_all(&argc, &argv, options,
        {"STD", "OUT", "FIG", "GEOFIG_REF", "MKREF_OPTS", "MKREF_BRD"}, pars);
      if (O.exists("help")){ usage(); return 0;}
      if (O.exists("pod")) { usage(true); return 0;}
      if (!O.exists("out"))
        throw Err() << "no output files";
      O.put("dpi", 1200/1.05);
      Fig F;
      GeoMap ref = geo_mkref_opts(O);
      geo_mkref_brd(ref, O);

      fig_add_ref(F, ref, O);
      write_fig(O.get<string>("out"), F, O);
      return 0;
    }
    /*****************************************/

    if (action == "add"){
      Opt O = parse_options_all(&argc, &argv, options,
        {"STD", "OUT", "GEO_I", "GEO_IO", "FIG", "GEOFIG_DATA", "DRAWMAP"}, pars);
      if (O.exists("help")) {usage(); return 0;}
      if (O.exists("pod")) {usage(true); return 0;}
      bool v = O.exists("verbose");
      if (!O.exists("out"))
        throw Err() << "no output files";

      GeoData data;
      for (auto const & f:pars) read_geo(f, data, O);

      if (v) cerr << ",  Waypoint lists: " << data.wpts.size()
                  << ",  Tracks: " << data.trks.size() << "\n";
      string ofile = O.get<string>("out");

      Fig F;
      read_fig(ofile, F, O);

      O.put_missing("map_dir", ofile+".img");

      if (v) cerr << "Writing data to " << ofile << "\n";
      GeoMap ref = fig_get_ref(F);
      fig_add_wpts(F, ref, data, O);
      fig_add_trks(F, ref, data, O);
      fig_add_maps(F, ref, data, O);

      write_fig(ofile, F, O);
      return 0;
    }

    /*****************************************/

    if (action == "del"){
      Opt O = parse_options_all(&argc, &argv, options,
        {"STD", "OUT", "GEOFIG_DEL"}, pars);
      if (O.exists("help")) {usage(); return 0;}
      if (O.exists("pod")) {usage(true); return 0;}
      bool v = O.exists("verbose");

      if (argc>0) throw Err() << "wrong argument: " << argv[0];
      if (!O.exists("out")) throw Err() << "no output files";

      string ofile = O.get<string>("out");

      Fig F;
      read_fig(ofile, F, O);

      bool all = !O.exists("wpts") && !O.exists("trks") &&
                 !O.exists("maps") && !O.exists("ref");
      if (O.exists("wpts") || all) fig_del_wpts(F);
      if (O.exists("trks") || all) fig_del_trks(F);
      if (O.exists("maps") || all) fig_del_maps(F);
      if (O.exists("ref")  || all) fig_del_ref(F);
      write_fig(ofile, F, O);
      return 0;
    }

    /*****************************************/

    if (action == "srtm"){
      Opt O = parse_options_all(&argc, &argv, options,
        {"STD", "OUT", "FIG", "SRTM", "GEOFIG_SRTM"}, pars);
      if (O.exists("help")) {usage(); return 0;}
      if (O.exists("pod")) {usage(true); return 0;}
      bool v = O.exists("verbose");

      if (argc>0) throw Err() << "wrong argument: " << argv[0];
      if (!O.exists("out")) throw Err() << "no output files";

      string ofile = O.get<string>("out");

      // get parameters
      bool cnt   = O.get("cnt",   true);
      bool scnt  = O.get("scnt",  true);
      bool peaks = O.get("peaks", true);
      int cnt_step  = O.get<int>("cnt_step", 100);
      int cnt_smult = O.get<int>("cnt_smult",  5);
      int cnt_minpts = O.get<int>("cnt_minpts",  6);
      int scnt_minpts = O.get<int>("scnt_minpts",  12);
      double scnt_val = O.get<double>("scnt_val",     35.0);
      auto peaks_dh = O.get<int>("peaks_dh",   20);
      auto peaks_ps = O.get<int>("peaks_ps",  1000);
      std::string cnt_templ1 = O.get("cnt_templ1",
        "2 1 0 1 #D0B090 7 90 -1 -1 0.000 1 1 0 0 0");
      std::string cnt_templ2 = O.get("cnt_templ2",
        "2 1 0 2 #D0B090 7 90 -1 -1 0.000 1 1 0 0 0");
      std::string scnt_templ = O.get("scnt_templ",
        "2 3 0 0 0 24 91 -1 20 0.000 0 0 -1 0 0");
      std::string peaks_templ = O.get("peaks_templ",
        "2 1 0 3 24 7  57 -1 -1 0.000 0 1 -1 0 0");

      double acc=5;

      // read fig file
      Fig F;
      read_fig(ofile, F, O);
      if (v) cerr << "Writing data to " << ofile << "\n";

      // Conversion FIG -> WGS
      GeoMap ref = fig_get_ref(F);
      ConvMap cnv(ref);

      // FIG range in WGS coords
      dRect wgs_range = cnv.frw_acc(ref.bbox(), 1e-7);

      // create SRTM interface
      SRTM srtm(O);

      // Contours
      if (cnt) {
        if (v) std::cout << "Finding contours\n";
        auto cnt_data = srtm.find_contours(wgs_range, cnt_step);
        for(auto & c:cnt_data){
          cnv.bck(c.second);
          line_filter_v1(c.second, acc);
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
        if (v) std::cout << "Finding areas with large slope: ";
        auto scnt_data = srtm.find_slope_contours(wgs_range, scnt_val, 1);
        cnv.bck(scnt_data);
        line_filter_v1(scnt_data, acc);
        remove_holes(scnt_data);
        for(auto c = scnt_data.begin(); c!= scnt_data.end(); c++){
          if (c->size() < scnt_minpts) {
            scnt_data.erase(c--);
            continue;
          }

          FigObj fo = figobj_template(scnt_templ);
          fo.set_points(*c);
          F.push_back(fo);
        }
        if (v) std::cout << scnt_data.size() << "\n";
      }

      // Summits
      if (peaks) {
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

      write_fig(ofile, F, O);
      return 0;
    }


    /*****************************************/
    else throw Err() << "Unknown action: " << action;

  }
  catch(Err & e){
    if (e.str()!="") cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}


