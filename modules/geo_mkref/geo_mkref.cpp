#include <sstream>
#include <iomanip>

#include "geo_data/conv_geo.h"
#include "geo_nom/geo_nom.h"
#include "geom/line.h"
#include "err/err.h"
//#include "geo_tiles/geo_tiles.h"

#include "geo_mkref.h"

using namespace std;


/********************************************************************/

GeoMap
geo_mkref(Opt & o){
  GeoMap map;

  if (!o.exists("ref"))
    throw Err() << "mk_ref: reference type (option ref) should be set";

  string reftype = o.get("ref",string());


  /***************************************/
  if (reftype == "nom"){

    // map name
    map.name = o.get("name",string());
    if (map.name == "")
      throw "mk_ref: nomenclature name should be set (name option)";

    // Map range (in pulkovo coordinates)
    nom_scale_t sc;
    dRect R = nom_to_range(map.name, sc, true);
    // lon0 projection parameter.
    // Should be correct at least for normal (non-multiplied) maps.
    double lon0 = lon2lon0(R.x + 1e-6);

    // map projection
    map.proj = "+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=500000 +lon_0="
             + type_to_str(lon0);
    string proj_pulk = "+ellps=krass +towgs84=28,-130,-95 +proj=lonlat";
    // conversion map_projection -> pulkovo
    ConvGeo cnv1(map.proj, proj_pulk);
    // conversion pulkovo -> wgs84
    ConvGeo cnv2(proj_pulk);

    // map resolution
    map.image_dpi = o.get("dpi",300);
    double mag = o.get("mag",1);

    // factor (map coordinates (m))/(map point)
    double k = (int)sc/mag * 25.4e-3 /*m/in*/ / map.image_dpi;
    cnv1.rescale_src(k); // now cnv1: map points -> pulkovo

    // Border in map points (1pt accuracy);
    // We convert a closed line, then removing the last point.
    dLine brd = cnv1.bck_acc(rect_to_line(R, true), 1);
    brd.resize(brd.size()-1);

    // Refpoints:
    dLine pts_r = rect_to_line(R, false),
          pts_w = pts_r;
    cnv1.bck(pts_r); // pulkovo -> map points
    cnv2.frw(pts_w);  // pulkovo -> wgs

    // image origin and size
    // TODO: image margins
    iPoint tlc = floor(pts_r.bbox().tlc());
    iPoint brc = floor(pts_r.bbox().brc());
    brd -= tlc;
    pts_r -= tlc;
    map.image_size = brc-tlc;

    // Add map border:
    map.border.push_back(brd);

    // Add refpoints:
    for (int i = 0; i<pts_r.size(); i++)
       map.ref.insert(make_pair(pts_r[i],pts_w[i]));

    return map;
  }

  /***************************************/
  if (reftype == "tms_tile" || reftype == "google_tile"){
    return map;
  }

  /***************************************/
  if (reftype == "proj"){
    return map;
  }

  /***************************************/
  throw Err() << "mk_ref: unknown reference type: " << reftype;
}
/*
  // default values
  double dpi=300;
  double google_dpi=-1;
  double rscale=100000;
  double rs_factor=1.0; // proj units/m
  bool verbose=o.exists("verbose");
  bool sw=!o.exists("swap_y");
  std::string proj = "+datum=WGS84 +proj=lonlat";

  // first step: process geom, nom, google options
  // -- create border and 4 refpoints in wgs84 lonlat
  // -- set map_proj and proj options
  // -- change defauld dpi and rscale if needed
  dRect geom;
  dLine refs;
  Options proj_opts;

  // rectangular map: no border, no margins, arbitrary projection
  // rectangular range in this projection
  if (o.exists("geom")){
    incompat_warning (o, "geom", "wgs_geom");
    incompat_warning (o, "geom", "wgs_brd");
    incompat_warning (o, "geom", "trk_brd");
    incompat_warning (o, "geom", "nom");
    incompat_warning (o, "geom", "google");\

    dRect geom = o.get<dRect>("geom");
    if (geom.empty()) throw Err() << "mk_ref: bad geometry";

    proj = o.get<Proj>("proj", "+datum=WGS84 +proj=lonlat");

    if ((proj == Proj("tmerc")) && (geom.x>=1e6)){
      double lon0 = convs::lon_pref2lon0(geom.x);
      geom.x = convs::lon_delprefix(geom.x);
      proj_opts.put<double>("lon0", lon0);
    }
    if (o.exists("lon0"))
    proj_opts.put("lon0", o.get<double>("lon0"));
    convs::pt2pt cnv(Datum("wgs84"), Proj("lonlat"), Options(),
                     datum, proj, proj_opts);

    refs = rect2line(geom, true, sw);
    ret.border = cnv.line_bck(refs, 1e-6);
    refs.resize(4);
    cnv.line_bck_p2p(refs);
  }
  else if (o.exists("wgs_geom")){
    incompat_warning (o, "wgs_geom", "geom");
    incompat_warning (o, "wgs_geom", "wgs_brd");
    incompat_warning (o, "wgs_geom", "trk_brd");
    incompat_warning (o, "wgs_geom", "nom");
    incompat_warning (o, "wgs_geom", "google");\

    dRect geom = o.get<dRect>("wgs_geom");
    if (geom.empty())
     throw Err() << "error: bad geometry";

    proj = o.get<Proj>("proj", Proj("tmerc"));
    proj_opts.put<double>("lon0",
      o.get("lon0", convs::lon2lon0(geom.x+geom.w/2)));

    convs::pt2pt cnv(Datum("wgs84"), Proj("lonlat"), Options(),
                     Datum("wgs84"), proj, proj_opts);

    if (verbose) cerr << "mk_ref: geom = " << geom << "\n";
    refs = rect2line(geom, true, sw);
    refs.resize(4);
    // border is set to be rectanglar in proj:
    ret.border =
      cnv.line_bck(rect2line(cnv.bb_frw(geom, 1e-6), true, sw), 1e-6);
  }
  else if (o.exists("wgs_brd")){
    incompat_warning (o, "wgs_brd", "geom");
    incompat_warning (o, "wgs_brd", "wgs_geom");
    incompat_warning (o, "wgs_brd", "trk_brd");
    incompat_warning (o, "wgs_brd", "nom");
    incompat_warning (o, "wgs_brd", "google");\

    ret.border = o.get<dLine>("wgs_brd");
    if (ret.border.size()<3)
     throw Err() << "error: bad border line";

    ret.border.push_back(ret.border[0]);

    proj = o.get<Proj>("proj", Proj("tmerc"));
    proj_opts.put<double>("lon0",
      o.get("lon0", convs::lon2lon0(ret.border.range().CNT().x)));

    if (verbose) cerr << "mk_ref: brd = " << ret.border << "\n";
    refs = generalize(ret.border, -1, 5); // 5pt
    refs.resize(4);
  }
  else if (o.exists("trk_brd")){
    incompat_warning (o, "wgs_brd", "geom");
    incompat_warning (o, "wgs_brd", "wgs_geom");
    incompat_warning (o, "wgs_brd", "wgs_brd");
    incompat_warning (o, "wgs_brd", "nom");
    incompat_warning (o, "wgs_brd", "google");\

    geo_data W;
    io::in(o.get<string>("trk_brd"), W);
    if (W.trks.size()<1)
      throw Err() << "error: file with a track is expected in trk_brd option";

    ret.border = W.trks[0];
    if (ret.border.size()<3)
     throw Err() << "error: bad border line";

    ret.border.push_back(ret.border[0]);

    proj = o.get<Proj>("proj", Proj("tmerc"));
    proj_opts.put<double>("lon0",
      o.get("lon0", convs::lon2lon0(ret.border.range().CNT().x)));

    if (verbose) cerr << "mk_ref: brd = " << ret.border << "\n";
    refs = generalize(ret.border, -1, 5); // 5pt
    refs.resize(4);
  }
  // nom map
  else if (o.exists("nom")){
    incompat_warning (o, "nom", "geom");
    incompat_warning (o, "nom", "wgs_geom");
    incompat_warning (o, "nom", "wgs_brd");
    incompat_warning (o, "nom", "trk_brd");
    incompat_warning (o, "nom", "google");

    proj=Proj("tmerc");
    datum=Datum("pulkovo");

    int rs;
    string name=o.get<string>("nom", string());
    dRect geom = convs::nom_to_range(name, rs);
    if (geom.empty())
      throw Err() << "mk_ref: can't get geometry for map name \"" << name << "\"";

    rscale = rs;
    double lon0 = convs::lon2lon0(geom.x+geom.w/2.0);
    proj_opts.put("lon0", lon0);
    convs::pt2pt cnv(Datum("wgs84"), Proj("lonlat"), Options(),
                     datum, Proj("lonlat"), proj_opts);

    if (verbose) cerr << "mk_ref: geom = " << geom << "\n";
    refs = rect2line(geom, true, sw);
    ret.border = cnv.line_bck(refs, 1e-6);
    refs.resize(4);
    cnv.line_bck_p2p(refs);
  }
  // google tile
  else if (o.exists("google")){
    incompat_warning (o, "google", "geom");
    incompat_warning (o, "google", "wgs_geom");
    incompat_warning (o, "google", "wgs_brd");
    incompat_warning (o, "google", "trk_brd");
    incompat_warning (o, "google", "nom");

    datum=Datum("wgs84");
    proj=Proj("google");

    vector<int> crd = read_int_vec(o.get<string>("google"));
    if (crd.size()!=3)
      throw Err() << "error: wrong --google coordinates";

    int x=crd[0];
    int y=crd[1];
    int z=crd[2];
    //

    Tiles tcalc;
    dRect geom = tcalc.gtile_to_range(x,y,z);

    if (verbose) cerr << "mk_ref: geom = " << geom << "\n";
    refs = rect2line(geom, true,sw);
    ret.border = refs;
    refs.resize(4);

    rscale=o.get<double>("rscale", rscale);

    double lat=refs.range().CNT().y;
    rs_factor = 1/cos(M_PI*lat/180.0);

    // m -> tile pixel
    double k = 1/tcalc.px2m(z);
    dpi = k * 2.54/100.0*rscale*rs_factor;
  }
  else
    throw Err() << "error: can't make map reference without"
                << " --geom or --nom or --google setting";

  ret.map_proj=proj;
  ret.map_datum=datum;
  ret.proj_opts=proj_opts;

  // step 2: calculate conversion coeff between map proj units and
  // output map points

  rscale=o.get<double>("rscale", rscale);
  if (o.get<string>("dpi", "") == "fig") dpi= 1200/1.05;
  else dpi=o.get<double>("dpi", dpi);

  dpi*=o.get<double>("mag", 1.0);

  // put real dpi and rscale back to options
  o.put("dpi", dpi);
  o.put("rscale", rscale);

  double k = 100.0/2.54 * dpi / rscale / rs_factor;

  if (verbose) cerr << "mk_ref: rscale = " << rscale
    << ", dpi = " << dpi << ", k = " << k << "\n";

  // step 3:  setting refpoints
  convs::pt2wgs cnv(datum, proj, proj_opts);
  dLine refs_r(refs);
  cnv.line_bck_p2p(refs_r);

  refs_r *= k; // to out units
  refs_r -= refs_r.range().TLC();
  double h = refs_r.range().h;

  // swap y if needed
  if (sw)
    for (int i=0;i<refs_r.size();i++)
      refs_r[i].y = h - refs_r[i].y;

  // add refpoints to our map
  for (int i=0;i<refs.size();i++)
    ret.push_back(g_refpoint(refs[i], refs_r[i]));

  // step 3:  converting border
  convs::map2wgs brd_cnv(ret);
  ret.border = brd_cnv.line_bck(ret.border);
  ret.border = generalize(ret.border, 1, -1); // 1 unit accuracy
  ret.border.resize(ret.border.size()-1);
  return ret;
}

*/
