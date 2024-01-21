#include <iostream>
#include <cstdio>
#include <iomanip>
#include <queue>

#include "geo_data/geo_io.h"
#include "geo_data/conv_geo.h"
#include "geo_data/geo_utils.h"
#include "err/err.h"
#include "getopt/getopt.h"
#include "getopt/help_printer.h"
#include "time_fmt/time_fmt.h"

using namespace std;
GetOptSet options;

void usage(bool pod=false){
  HelpPrinter pr(pod, options, "ms2xyz");

  pr.name("writing track in a text form");
  pr.usage("<options> <input files>");

  pr.head(1, "General options");
  pr.opts({"NONSTD","HELP","POD","VERB","OUT"});

  pr.par("If output file is not specified the data is printed to stdout.");

  pr.head(1, "Geodata input options");
  pr.opts({"GEO_I", "GEO_IO"});

  pr.head(1, "Format");
  pr.par(
   "  %% -- % sign\n"
   "  %x -- wgs lon\n"
   "  %y -- wgs lat\n"
   "  %z -- altitude, m\n"

   "  %t -- time (seconds since 1970-01-01 00:00:00 UTC)\n"
   "  %T -- formatted date and time (default: ISO 8601 form, <yyyy-mm-dd>T<HH:MM:SS.FFF>Z)\n"
   "  %u -- time from part beginning, s\n"
   "  %U -- time from data beginning, s\n"
   "  %v -- time from the previous point, s\n"

   "  %d -- % distance from part beginning, km\n"
   "  %D -- % distance from data beginning, km\n"
   "  %e -- % distance from the previous point, m\n"

   "  %S -- % speed, km/h\n"

   "  %n -- %  point number from part beginning\n"
   "  %N -- %  point number from data beginning\n"
   "  %p -- %  part number\n"
  );

  // see modules/time_fmt
  pr.head(1, "Time format");
  pr.par(
   "  %% -- a literal %\n"
   "  %n -- a newline\n"
   "  %t -- a tab\n"

   "  %Y -- year\n"
   "  %y -- last two digits of year (00..99)\n"
   "  %m -- month (01..12)\n"
   "  %d -- day of month (e.g., 01)\n"
   "  %H -- hour (00..23)\n"
   "  %M -- minute (00..59)\n"
   "  %S -- second (00..60)\n"
   "  %F -- same as %Y-%m-%d\n"
   "  %T -- same as %H:%M:%S\n"

   "  %a -- abbreviated weekday name (e.g., Sun)\n"
   "  %b -- abbreviated month name (e.g., Jan)\n"

   "  %s -- seconds since 1970-01-01 00:00:00 UTC\n"
   "  %f -- fractional part of a second it it is non-zero (non-standard)\n"
  );
  throw Err();
}

/**********************************************************/
struct track_pt_t{
  GeoTpt pt;
  bool   start; // start - start of the part
  double time1, time2, dt; // seconds
  double dist1, dist2, dd; // meters
  double speed;            // km/h
  int    nn1, nn2, pn;

  track_pt_t(): start(false),
                time1(0.0), time2(0.0), dt(0.0),
                dist1(0.0), dist2(0.0), dd(0.0),
                speed(0.0), nn1(0), nn2(0), pn(0) {}

  // print values according to fmt
  void print(const Opt & O) const{
    string tfmt   = O.get<string>("tfmt", "%F %T");
    string fmt    = O.get<string>("fmt", "%x %y %z %T %D %S");
    int llprec    = O.get<int>("llprec", 7);
    int zprec     = O.get<int>("zprec", 1);
    int dprec     = O.get<int>("dprec", 3);
    int sprec     = O.get<int>("sprec", 2);
    int tprec     = O.get<int>("tprec", 1);

    if (start) cout << endl;
    bool esc=false;
    for (string::const_iterator c=fmt.begin(); c!=fmt.end(); c++){
      if (!esc){
        if (*c=='%') esc=true;
        else cout << *c;
        continue;
      }
      switch(*c){
        case '%': cout << *c; break;
        case 'x': cout << fixed << setprecision(llprec) << pt.x; break;
        case 'y': cout << fixed << setprecision(llprec) << pt.y; break;
        case 'z': cout << fixed << setprecision(zprec) << pt.z; break;

        case 't': cout << int(pt.t/1000.0); break;
        case 'T': cout << write_fmt_time(tfmt.c_str(), pt.t); break;
        case 'U': cout << fixed << setprecision(tprec) << time1; break;
        case 'u': cout << fixed << setprecision(tprec) << time2; break;
        case 'v': cout << fixed << setprecision(tprec) << dt;    break;

        case 'D': cout << fixed << setprecision(dprec) << dist1/1000.0; break;
        case 'd': cout << fixed << setprecision(dprec) << dist2/1000.0; break;
        case 'e': cout << fixed << setprecision(dprec) << dd; break;

        case 'S': cout << fixed << setprecision(sprec) << speed;    break;

        case 'N': cout << nn1;    break;
        case 'n': cout << nn2;    break;
        case 'p': cout << pn+1;    break;
        default:
          cout << '%' << *c;
      }
      esc=false;
    }
    cout << endl;
  }
};
/**********************************************************/

std::vector<track_pt_t>
read_data(const vector<string> & infiles, const Opt & O){
  double tshift = O.get<double>("tshift", 0);
  double window = O.get<double>("window", 120.0);
  string br     = O.get<string>("break", "none");
  std::vector<track_pt_t> pt_data;

  // read data
  GeoData data;
  for (auto const & f:infiles) read_geo(f, data, O);

  // fill point array, detect breakpoints
  int64_t tp = 0;
  for (auto const & trk: data.trks){
    bool trk_start = true;
    for (auto const & seg: trk){
      bool seg_start = true;
      for (auto const & pt: seg){
        track_pt_t p;
        p.pt = pt;
        p.pt.t += tshift*3600*1000;
        p.start =
           (br == "track"   && trk_start) ||
           (br == "segment" && seg_start) ||
           (br == "day"     && p.pt.t/86400000 != tp/86400000);
        tp = p.pt.t;
        pt_data.push_back(p);
        seg_start = trk_start = false;
      }
    }
  }
  if (pt_data.size()==0) return pt_data;

  // remove start flag on the first point
  pt_data[0].start = false;

  // reference times
  double time01 = pt_data[0].pt.t;
  double time02 = pt_data[0].pt.t;

  double speed=0, dist1=0, dist2=0;
  size_t nn1=0, nn2=0, pn=0;

  // first pass: fill times, distances, point numbers
  for (size_t i = 0; i < pt_data.size(); ++i) {

    auto & pt0 = pt_data[i];
    auto & ptp = pt_data[i>0? i-1:0];

    double dd = geo_dist_2d(pt0.pt, ptp.pt);
    double dt = pt0.pt.t - ptp.pt.t;

    // reset values in the beginning of a new part
    if (pt0.start){
      pn++;
      time02 = pt0.pt.t;
      dist2 = 0.0;
      nn2 = 0;
      // Remove td/dd information for gap between segments,
      // Exclude distance between segments from dist1.
      // Do we want this?
      dd = dt = 0.0;
    }
    dist1 += dd;
    dist2 += dd;
    nn1++;
    nn2++;

    // fill data
    pt0.time1 = (pt0.pt.t-time01)/1000.0;
    pt0.time2 = (pt0.pt.t-time02)/1000.0;
    pt0.dt = dt/1000.0;
    pt0.dist1 = dist1;
    pt0.dist2 = dist2;
    pt0.dd = dd;
    pt0.nn1 = nn1;
    pt0.nn2 = nn2;
    pt0.pn = pn;
  }

  // second pass: speed
  for (size_t i = 1; i < pt_data.size(); ++i) {
    // go back until we reach needed time window or beginning of the part
    size_t j;
    for (j=i-1; j!=0; j--){
      if (pt_data[i].time1 - pt_data[j].time1 >= window) break;
      if (pt_data[j].start) break;
    }

    // Shrink range to fit it within the window if possible. Do we want this?
    if (pt_data[i].time1 - pt_data[j].time1 > window && j+1<i) j++;

    double dist = pt_data[i].dist1 - pt_data[j].dist1;
    double time = pt_data[i].time1 - pt_data[j].time1;
    pt_data[i].speed = time!=0 ? dist/time*3.6 : 0;
//std::cerr << ">> " << dist << " " << time << " " << pt_data[i].speed << "\n";
  }

  return pt_data;
}

/**********************************************************/
/* Project track to a reference track.
 We should consider a few possibilities:
 - Point can be far from the reference track
 - Point can be close to a wrong part of the reference track
   (track with a loop, back-and-forth motion, etc.)

 We do projection in the following way:
 For each new point we calculate distance from the previous one
 (or from beginning of the reference track) and use only
 part on the reference track which stays in this range x 2.
 In this range we project to the nearest point.
*/

void
project_data(std::vector<track_pt_t> & pt_data, const std::vector<track_pt_t> & ref_data){
  if (pt_data.size()<1 || ref_data.size()<1) return;

  size_t nref0 = 0; // index of the last used ref track point
  for (size_t n = 0; n<pt_data.size(); n++){
    auto & pp = n>0? pt_data[n-1]:ref_data[0];
    auto & pt = pt_data[n];

    // find nearest point from ref track in the correct range
    double dmin = INFINITY;
    size_t nmin = nref0;
    for (size_t nref = nref0; nref<ref_data.size(); nref++){
      const auto & ref = ref_data[nref];
      if (geo_dist_2d(ref.pt, pp.pt) > 2*geo_dist_2d(pt.pt, pp.pt)) break;

      auto d = geo_dist_2d(pt.pt, ref.pt);
      if (d<dmin) {dmin=d; nmin=nref;}
    }

    const auto & ref1 = ref_data[nmin];
    pt.pt.x = ref1.pt.x;
    pt.pt.y = ref1.pt.y;
    pt.dist1 = ref1.dist1;
    pt.dist2 = ref1.dist2;
    nref0 = nmin;
  }
}

/**********************************************************/

int
main (int argc, char **argv) {
  try {

    const char *g = "NONSTD";
    options.add("tshift", 1,0,g,
      "Time shift, hours.");
    options.add("tfmt",   1,'t', g,
      "Time format string for %T field (default: \"%F %T\")");
    options.add("fmt", 1,'f', g,
      "Format string (default: \"%x %y %z %T %D %S\")");
    options.add("win", 1,'w', g,
      "Window for speed calculation, sec (default: 120)");
    options.add("break", 1,'b', g,
      "Place to break calculation and put empty line "
      "(none | day | track | segment, default: none)");
    options.add("llprec", 1,0, g,
      "Precision for latitude and longitude values (default: 7)");
    options.add("zprec", 1,0, g,
      "Precision for altitude values (default: 1)");
    options.add("dprec", 1,0, g,
      "Precision for distance values (default: 3)");
    options.add("sprec", 1,0, g,
      "Precision for speed values (default: 2)");
    options.add("tprec", 1,0, g,
      "Precision for time values (default: 1)");
    options.add("ref", 1,0, g,
      "Print distance and coordinates as a projection to the reference track. "
      "Useful when track has only a few points and there is a more accurate reference track "
      "without timestamps. Default: \"\". --break option is ignored.");

    ms2opt_add_std(options, {"HELP","POD","VERB","OUT"});
    ms2opt_add_geo_i(options);
    ms2opt_add_geo_io(options);

    if (argc<2) usage();
    vector<string> infiles;
    Opt O = parse_options_all(&argc, &argv, options, {}, infiles);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);



    // open output file
    string ofile = O.get("out","");
    if (ofile != "") FILE * F=freopen(ofile.c_str(),"w",stdout);

    // read data
    auto pt_data = read_data(infiles, O);

    // project if needed
    string ref_fname = O.get<string>("ref", "");
    if (ref_fname != ""){
      O.put("break", "none");
      std::vector<std::string> ref_files;
      ref_files.push_back(ref_fname);
      auto ref_data = read_data(ref_files, O);
      project_data(pt_data, ref_data);
    }

    // print data
    for (const auto & pt : pt_data) pt.print(O);
  }
  catch (Err & e) {
    if (e.str()!="") std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;

}
