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
  double time1, time2, dt; // seconds
  double dist1, dist2, dd; // meters
  double speed;            // km/h
  int    nn1, nn2, pn;
  bool   br; // print linebreak before the point

  // print values according to fmt
  void print(const Opt & O) const{
    string tfmt   = O.get<string>("tfmt", "%F %T");
    string fmt    = O.get<string>("fmt", "%x %y %z %T %D %S");
    int llprec    = O.get<int>("llprec", 7);
    int zprec     = O.get<int>("zprec", 1);
    int dprec     = O.get<int>("dprec", 3);
    int sprec     = O.get<int>("sprec", 2);
    int tprec     = O.get<int>("tprec", 1);

    if (br) cout << endl;
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
read_data(const GeoData & data, const Opt & O){
  double tshift = O.get<double>("tshift", 0);
  double window = O.get<double>("window", 120.0);
  string br     = O.get<string>("break", "none");
  std::vector<track_pt_t> pt_data;

  double speed=0, dist = 0, Dist = 0, t0 = 0, T0 = -1;
  queue<pair<double, double> > timedist;

  size_t nn = 0;
  size_t NN = 0;
  size_t pn = 0;
  GeoTpt tp, pp;
  for (auto const & trk: data.trks) {
    for (size_t p = 0; p < trk.size(); ++p) {

      tp = trk[p];
      tp.t += tshift*3600*1000;

      if (T0<0) T0 = tp.t;

      bool do_br = false;
      if ( (br == "track" && (tp.start || p==0)) ||
           (br == "day"   && (nn != 0) &&
             (tp.t/86400000 != pp.t/86400000)) ){
        nn=0;
        pn++;
        do_br = true;
      }

      double dd=0;
      int    dt=0;
      if (nn != 0) {
        // update values:
        dd = geo_dist_2d(tp,pp);
        dt = tp.t - pp.t;
        dist +=dd;
        Dist +=dd;
      } else {
        dist = 0;
        t0 = tp.t;
      }

      timedist.push(make_pair(tp.t/1000.0, Dist));
      while (timedist.front().first < tp.t/1000.0 - window &&
             timedist.size() > 2) timedist.pop();

      double traveled = timedist.back().second - timedist.front().second;
      double time = timedist.back().first - timedist.front().first;
      if (time==0) speed=0;
      else speed = traveled/time * 3.6;

      pp = tp;
      ++nn;
      ++NN;

      // fill data
      track_pt_t pt;
      pt.pt = tp;
      pt.time1 = (tp.t-T0)/1000.0;
      pt.time2 = (tp.t-t0)/1000.0;
      pt.dt = dt/1000.0;
      pt.dist1 = Dist;
      pt.dist2 = dist;
      pt.dd = dd;
      pt.speed = speed;
      pt.nn1 = NN;
      pt.nn2 = nn;
      pt.pn = pn;
      pt.br = do_br;
      pt_data.push_back(pt);
    }
  }

  return pt_data;
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
      "(none | day | track, default: none)");
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
    GeoData data;
    for (auto const & f:infiles) read_geo(f, data, O);
    auto pt_data = read_data(data, O);

    // print data
    for (const auto & pt : pt_data) pt.print(O);
  }
  catch (Err & e) {
    if (e.str()!="") std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;

}
