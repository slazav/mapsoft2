#include <iostream>
#include <cstdio>
#include <iomanip>
#include <queue>

#include "geo_data/geo_io.h"
#include "geo_data/conv_geo.h"
#include "geo_data/geo_utils.h"
#include "err/err.h"
#include "getopt/getopt.h"
#include "time_fmt/time_fmt.h"

using namespace std;
GetOptSet options;

void usage(bool pod=false, std::ostream & S = std::cout){
  std::string head = pod? "\n=head1 ":"\n";
  const char * fname = "ms2xyz";
  S << fname << " -- writing track in a text form\n"
             << "Usage: " << fname << " <options> <input files>\n";
  S << head << "General options:\n";
  print_options(options, MS2OPT_NONSTD | MS2OPT_STD | MS2OPT_OUT, S, pod);
  S << "If output file is not specified the data is printed to stdout.\n";

  S << head << "Geodata input options:\n";
  print_options(options, MS2OPT_GEO_I | MS2OPT_GEO_IO, S, pod);
  S << head << "Format:\n"
       << "  %% -- % sign\n"
       << "  %x -- wgs lon\n"
       << "  %y -- wgs lat\n"
       << "  %z -- altitude, m\n"

       << "  %t -- time (seconds since 1970-01-01 00:00:00 UTC)\n"
       << "  %T -- date and time in ISO 8601 form (<yyyy-mm-dd>T<HH:MM:SS.FFF>Z)\n"
       << "  %u -- time from the previous point, s\n"

       << "  %d -- % distance from part beginning, km\n"
       << "  %D -- % distance from data beginning, km\n"
       << "  %e -- % distance from the previous point, m\n"

       << "  %S -- % speed, km/h\n"

       << "  %n -- %  point number from part beginning\n"
       << "  %N -- %  point number from data beginning\n"
       << "  %p -- %  part number\n"
  ;
  throw Err();
}


int
main (int argc, char **argv) {
  try {

    int m = MS2OPT_NONSTD;
    options.add("tshift", 1, 0,  m, "time shift, hours");
    options.add("tfmt",   1,'t', m, "time format string for %T field (default: \"%F %T\")");
    options.add("fmt",    1,'f', m, "format string (default: \"%x %y %z %T %D %S\")");
    options.add("win",    1,'w', m, "window for speed calculation, sec (default: 120)");
    options.add("break",  1,'b', m, "place to break calculation and put empty line "
                                    "(none | day | track, default: none)");

    ms2opt_add_std(options);
    ms2opt_add_out(options);
    ms2opt_add_geo_i(options);
    ms2opt_add_geo_io(options);

    if (argc<2) usage();
    vector<string> infiles;
    Opt O = parse_options_all(&argc, &argv, options, ~0, infiles);
    if (O.exists("help")) usage();
    if (O.exists("pod"))  usage(true);

    GeoData data;
    for (auto const & f:infiles)
      if (!read_geo(f, data, O))
        throw Err() << "Can't determine input format for file: " << f;

    double tshift = O.get<double>("tshift", 0);
    string tfmt   = O.get<string>("tfmt", "%F %T");
    string fmt    = O.get<string>("fmt", "%x %y %z %T %D %S");
    double window = O.get<double>("window", 120.0);
    string br     = O.get<string>("break", "none");

    double speed=0, dist = 0, Dist = 0;
    queue<pair<double, double> > timedist;

    string ofile = O.get("out","");
    if (ofile != "") FILE * F=freopen(ofile.c_str(),"w",stdout);

    int nn = 0;
    int NN = 0;
    int pn = 0;
    GeoTpt tp, pp;
    for (auto const & trk: data.trks) {
      for (int p = 0; p < trk.size(); ++p) {

        tp = trk[p];
        tp.t += tshift*3600*1000;

        if ( (br == "track" && (tp.start || p==0)) ||
             (br == "day"   && (nn != 0) &&
               (tp.t/86400000 != pp.t/86400000)) ){
          nn=0;
          pn++;
          cout << endl;
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

        // print values according to fmt
        bool esc=false;
        for (string::const_iterator c=fmt.begin(); c!=fmt.end(); c++){
          if (!esc){
            if (*c=='%') esc=true;
            else cout << *c;
            continue;
          }
          switch(*c){
            case '%': cout << *c; break;
            case 'x': cout << fixed << setprecision(7) << tp.x; break;
            case 'y': cout << fixed << setprecision(7) << tp.y; break;
            case 'z': cout << fixed << setprecision(1) << tp.z; break;

            case 't': cout << int(tp.t/1000.0); break;
            case 'T': cout << write_fmt_time(tfmt.c_str(), tp.t); break;
            case 'u': cout << fixed << setprecision(3) << dt/1000.0; break;

            case 'd': cout << fixed << setprecision(2) << dist/1000.0; break;
            case 'D': cout << fixed << setprecision(3) << Dist/1000.0; break;
            case 'e': cout << fixed << setprecision(2) << dd; break;

            case 'S': cout << fixed << setprecision(2) << speed;    break;

            case 'n': cout << nn;    break;
            case 'N': cout << NN;    break;
            case 'p': cout << pn+1;    break;
            default:
              cout << '%' << *c;
          }
          esc=false;
        }
        cout << endl;

      }
    }
  }
  catch (Err e) {
    if (e.str()!="") std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;

}
