#include "geo_nom/geo_nom.h"
#include "conv_geo.h"
#include "filters.h"

using namespace std;

/********************************************************************/

// add MS2OPT_GEOFLT options
void
ms2opt_add_geoflt(GetOptSet & opts){
  int m = MS2OPT_GEOFLT;
  opts.add("skip",    1, 0, m, "Skip part of geodata. Argument is a string "
                               "which letters show what to skip: W - waypoints, "
                               "T - tracks, M - maps, t - time, z - altitude, "
                               "b - map borders." );
  opts.add("join",    0, 0, m, "Join all waypoint lists, tracks, map lists.");
  opts.add("nom_brd", 0, 0, m, "Set map border for a Soviet nomenclature map "
                               "(the map should have a valid name)");
}

/********************************************************************/

void
geo_filters(GeoData & data, const Opt & opt){

  if (opt.exists("skip"))
    filter_skip(data, opt);

  if (opt.exists("join"))
    filter_join(data, opt);

  if (opt.exists("nom_brd"))
    filter_nom_brd(data, opt);
}


/********************************************************************/

void
filter_skip(GeoData & data, const Opt & opt){

  string sk = opt.get("skip", "");
  if (sk == "") return;

  if (opt.exists("verbose")) cerr << "filter_skip: skip data: " << sk << endl;

  if (sk.find("M")!=string::npos) data.maps.clear();

  if (sk.find("W")!=string::npos) data.wpts.clear();

  if (sk.find("T")!=string::npos) data.trks.clear();

  if (sk.find("t")!=string::npos){
    for (auto & trk:data.trks)
      for (auto & pt:trk) pt.t = 0;
    for (auto & wpl:data.wpts)
      for (auto & pt:wpl) pt.t = 0;
  }

  if (sk.find("z")!=string::npos){
    for (auto & trk:data.trks)
      for (auto & pt:trk) pt.clear_alt();
    for (auto & wpl:data.wpts)
      for (auto & pt:wpl) pt.clear_alt();
  }

  if (sk.find("b")!=string::npos){
    for (auto & ml:data.maps)
      for (auto & m:ml) m.border.clear();
  }

}

/********************************************************************/

void
filter_join(GeoData & data, const Opt & opt){
  if (opt.exists("verbose")) cerr << "filter_join: "
    "join all tracks, all waypoint lists, all map lists" << endl;

  //join maps:
  if (data.maps.size()>1){
    auto m0 = data.maps.begin();
    auto m1 = m0; ++m1;
    while (m1!=data.maps.end()){
      m0->insert(m0->begin(), m1->begin(), m1->end());
      m1=data.maps.erase(m1);
    }
    m0->name = "JOIN";
  }

  //join wpts:
  if (data.wpts.size()>1){
    auto w0 = data.wpts.begin();
    auto w1 = w0; ++w1;
    while (w1!=data.wpts.end()){
      w0->insert(w0->begin(), w1->begin(), w1->end());
      w1=data.wpts.erase(w1);
    }
    w0->name = "JOIN";
  }

  //join trks:
  if (data.trks.size()>1){
    auto t0 = data.trks.begin();
    auto t1 = t0; ++t1;
    while (t1!=data.trks.end()){
      t0->insert(t0->begin(), t1->begin(), t1->end());
      t1=data.trks.erase(t1);
    }
    t0->name = "JOIN";
  }
}

/********************************************************************/

void
filter_nom_brd(GeoData & data, const Opt & opt){

  if (opt.exists("verbose")) cerr << "filter_nom_brd: "
            "set border for Soviet nomenclature maps" << endl;

  for (auto & ml:data.maps){
    for (auto & m: ml){

      nom_scale_t sc;
      dRect r = nom_to_range(m.name, sc, true);

      if (opt.exists("verbose")){
        if (r) cerr << " - setting border for the nom map " << m.name << endl;
        else cerr << " - skipping non-nomenclature map " << m.name << endl;
      }

      if (!r) return;
      double lon1 = r.x;
      double lat1 = r.y;
      double lon2 = lon1 + r.w;
      double lat2 = lat1 + r.h;

      // Map -> Pulkovo lat-lon
      ConvMap cnv(m, "+ellps=krass +towgs84=+28,-130,-95 +proj=lonlat");

      dLine brd;
      brd.push_back(dPoint(lon1,lat2));
      brd.push_back(dPoint(lon2,lat2));
      brd.push_back(dPoint(lon2,lat1));
      brd.push_back(dPoint(lon1,lat1));
      brd.close();
      m.border.clear();
      m.border.push_back(cnv.bck_acc(brd));
    }
  }
}


