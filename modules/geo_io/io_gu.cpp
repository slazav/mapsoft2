#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "iconv/iconv.h"
#include "time_fmt/time_fmt.h"
#include "io_gu.h"

using namespace std;
string gu_default_enc("KOI8-R");

void read_gu (const char *fname, GeoData & data, const Opt & opts){
  IConv cnv(opts.get("gu_enc", gu_default_enc).c_str(), "UTF-8");
  if (opts.exists("verbose")) cerr <<
    "Reading GarminUtils file: " << fname << endl;

  ifstream s(fname);
  int mode = 0;
  GeoWptList wpt;
  GeoTrk trk;
  while (!s.eof()){
    string l;
    std::getline(s, l);

    if (l.compare(0, 10, "[waypoints")==0) {
      wpt.clear(); trk.clear(); mode = 1;
      continue;
    }
    if (l.compare(0,  7, "[tracks")==0) {
      wpt.clear(); trk.clear(); mode = 2;
      continue;
    }
    if (l.compare(0,  4, "[end")==0) {
      if (wpt.size()) data.wpts.push_back(wpt);
      if (trk.size()) data.trks.push_back(trk);
      wpt.clear(); trk.clear(); mode = 0;
      continue;
    }
    if (mode == 1){
      GeoWpt p;
      int symb, displ;
      char c;
      istringstream s1(l);
      s1 >> p.name >> p.y >> p.x >> symb >> c >> displ >> ws;
      std::getline(s1, p.comm);
      if (s1.fail() || !s1.eof() || c != '/')
        throw Err() << "io_gu: can't parse a waypoint: [" << l << "]";
      p.name = cnv(p.name);
      p.comm = cnv(p.comm);
      wpt.push_back(p);
      continue;
    }
    if (mode == 2){
      GeoTpt p;
      std::string t1,t2,st;
      istringstream s1(l);
      s1 >> p.y >> p.x >> t1 >> t2 >> ws;
      p.t = parse_utc_time(t1 + " " + t2);
      if (!s1.eof()){
        s1 >> st;
        if (st != "start")
          throw Err() << "io_gu: can't parse a trackpoint: [" << l << "]";
        p.start=1;
      }
      if (s1.fail() || !s1.eof())
        throw Err() << "io_gu: can't parse a trackpoint: [" << l << "]";
      if (trk.size()==0) p.start=1;
      trk.push_back(p);
      continue;
    }
  }
}


void write_gu_waypoints(std::ostream & s, const GeoWptList & wp, const IConv & cnv){
  int num = wp.size();
  s << "[waypoints, " << num << " records]\n";
  vector<GeoWpt>::const_iterator p;
  int symb  = 0;
  int displ = 0;
  for (p = wp.begin(); p!=wp.end(); p++){
    string name = cnv(p->name);
    string comm = cnv(p->comm);
    std::replace(name.begin(), name.end(), ' ', '_');

    s << left << setw(6) << setfill(' ') << name << " "
      << right << fixed << setprecision(6)
      << setw(10) << p->y << " "
      << setw(11) << p->x << " "
      << setw(5) << symb << "/" << displ << " "
      << comm  << "\n";
  }
  s << "[end transfer, " << num << "/" << num << " records]\n";
  if (s.fail()) throw Err() << "io_gu: Can't write waypoints";
}


void write_gu_track(std::ostream & s, const GeoTrk & tr){
  int num = tr.size();
  s << "[tracks, " << num << " records]\n";
  GeoTrk::const_iterator p;
  for (p = tr.begin(); p != tr.end(); p++){
    s << right << fixed << setprecision(6) << setfill(' ')
      << setw(10)<< p->y << " "
      << setw(11)<< p->x << " "
      << setfill('0') << write_fmt_time("%F %T", p->t)
      << ((p->start)? " start":"") << "\n";
  }
  s << "[end transfer, " << num << "/" << num << " records]\n";
  if (s.fail()) throw Err() << "io_gu: Can't write track";
}


void write_gu (const char *fname, const GeoData & data, const Opt & opts){
  IConv cnv("UTF-8", opts.get("gu_enc", gu_default_enc).c_str());
  if (opts.exists("verbose")) cerr <<
    "Writing GarminUtils file: " << fname << endl;

  ofstream s(fname);
  s << "[product 00, version 000: MAPSOFT2]\n";
  vector<GeoWptList>::const_iterator w;
  for (w = data.wpts.begin(); w!=data.wpts.end(); w++)
    write_gu_waypoints(s, *w, cnv);

  vector<GeoTrk>::const_iterator t;
  for (t = data.trks.begin(); t!=data.trks.end(); t++)
    write_gu_track(s, *t);
}
