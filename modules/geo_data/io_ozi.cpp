#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "opt/opt.h"
#include "err/err.h"
#include "time_fmt/time_fmt.h"
#include "iconv/iconv.h"
#include "geom/poly_tools.h"
#include "filename/filename.h"

#include "conv_geo.h"
#include "geo_data.h"
#include "geo_utils.h"


using namespace std;
string ozi_default_enc("WINDOWS-1251");

void
crop_nl(string & s){
  if (s.size() && *s.rbegin()=='\n') s.resize(s.size()-1);
  if (s.size() && *s.rbegin()=='\r') s.resize(s.size()-1);
}

void
str_trim(string & s){
  size_t epos = s.find_last_not_of(" \t\r\n");
  if(epos != string::npos) s = s.substr( 0, epos+1);
  else {s=""; return;}

  size_t spos = s.find_first_not_of(" \t\r\n");
  if (spos != string::npos && spos!=0) s = s.substr(spos);
}

/***************************************************************************/
/// Unpack Ozi-style CSV lines.
/**
## Format description (from http://www.oziexplorer3.com/eng/help/fileformats.html)
 * Each field separated by a comma.
 * Comma's not allowed in text fields, character 209 can be used instead and a comma will be substituted.
 * Non essential fields need not be entered but comma separators must still be used (example ,,).
 * Defaults will be used for empty fields.
 * Any number of the last fields in a data line need not be included at all not even the commas.

 If (count>0) vector of this size is returned. Extra fields will be silently ignored,
 Missing fields are returned as empty strings.
*/
vector<string> unpack_ozi_csv(const string & str, unsigned int count, bool trim = false){
  int p1=0, p2=0;
  vector<string> ret;
  do {
    p2=str.find(',', p1);
    string field = str.substr(p1,p2-p1);
    crop_nl(field);
    if (trim) str_trim(field);
    replace( field.begin(), field.end(), (char)209, ',');
    ret.push_back(field);
    if (count && ret.size()==count) break;
    p1=p2+1;
  }
  while (p2!=string::npos);
  if (count) ret.resize(count);
  return ret;
}
/*******************************************************************/
/// Pack Ozi-style CSV lines.
/// Commas will be substituted by character 209.
/// Newline characters will be substituted by space.
/// Empty fields in the end will be skipped.
string pack_ozi_csv(const vector<string> & vec){
  string ret;

  // find last non-empty field
  size_t end=vec.size();
  while (end>0 && vec[end-1]=="") end--;

  for (size_t i=0; i!=end; ++i){
    string field = vec[i];
    replace( field.begin(), field.end(), ',', (char)209);
    replace( field.begin(), field.end(), '\n', ' ');
    ret += (i==0?"":",") + field;
  }
  return ret;
}

/*******************************************************************/
/// Convert text to Ozi format:
/// - replace commas by (char)209
/// - replace newlines by space
/// - resize to 40 symbols
string convert_ozi_text(const string & str){
  string ret(str);
  if (ret.size()>40) ret.resize(40);
  replace( ret.begin(), ret.end(), ',', (char)209);
  replace( ret.begin(), ret.end(), '\n', ' ');
  return ret;
}

/***************************************************************************/
string convert_ozi2proj(const string & s){
  if (s=="Latitude/Longitude")      return "+proj=lonlat";
  if (s=="Transverse Mercator")     return "+proj=tmerc";
  if (s=="Mercator")                return "+proj=merc";
  if (s=="Lambert Conformal Conic") return "+proj=lcc";
  throw Err() << "io_ozi: unsupported Ozi projection: " << s;
}

string convert_ozi2datum(const string & s){
  if (s=="WGS 84") return "+datum=WGS84";
  if (s=="Pulkovo 1942" ||
      s=="Pulkovo 1942 (1)" ||
      s=="Pulkovo 1942 (2)")  return "+ellps=krass +towgs84=28,-130,-95";
  throw Err() << "io_ozi: unsupported Ozi datum: " << s;
}

string get_proj_par(const string & proj, const string & key){
  string exp = expand_proj_aliases(proj);

  string kv = string("+") + key + "=";
  size_t kl = kv.size();

  size_t n1 = exp.find(kv);
  size_t n2 = exp.find(" ", n1);
  return n1!=string::npos ? exp.substr(n1+kl,n2-n1-kl) : "";
}

string convert_proj2ozi(const string & s){
  string pr = get_proj_par(s, "proj");
  if (pr == "") throw Err() << "io_ozi: can't find proj setting: " << s;

  if (pr=="latlong") return "Latitude/Longitude";
  if (pr=="longlat") return "Latitude/Longitude";
  if (pr=="latlon") return "Latitude/Longitude";
  if (pr=="lonlat") return "Latitude/Longitude";
  if (pr=="tmerc")   return "Transverse Mercator";
  if (pr=="merc")    return "Mercator";
  if (pr=="webmerc") return "Mercator"; // not exact
  if (pr=="lcc")     return "Lambert Conformal Conic";
  throw Err() << "io_ozi: unsupported projection: " << pr;
}

string convert_datum2ozi(const string & s){
  string dt = get_proj_par(s, "datum");
  string el = get_proj_par(s, "ellps");
  string to = get_proj_par(s, "towgs84");

  if (dt == "WGS84") return "WGS 84";
  if (el == "krass" && (to == "28,-130,-95" || to == "+28,-130,-95"))
    return "Pulkovo 1942 (2)";

  throw Err() << "io_ozi: unsupported datum: " << s;
}

/***************************************************************************/
/***************************************************************************/
void read_ozi (const string &fname, GeoData & data, const Opt & opts){
  bool vv = opts.get("verbose", false);
  if (vv) cerr << "Reading OziExplorer file " << fname << endl;

  std::string prefix = file_get_prefix(fname);

  ifstream f(fname);
  if (!f.good()) throw Err()
      << "Can't read data from OziExplorer file: " << fname;

  IConv cnv(opts.get("ozi_enc", ozi_default_enc), "UTF-8");

  string s1,s2,s3;
  f >> s1 >> s2;
  getline(f,s3);

  if (s1!="OziExplorer") throw Err() << "Not an OziExplorer file: " << fname;

  //// TRACK
  if (s2 == "Track"){
    GeoTrk trk;
    getline(f,s1);
    crop_nl(s1);
    if (s1 != "WGS 84") throw Err() << "Unsupported datum: " << s1;
    getline(f,s1); // Altitude is in Feet
    getline(f,s1); // Reserved
    getline(f,s1);
    vector<string> v = unpack_ozi_csv(s1,8);
    if (v[1] != "") trk.opts.put("thickness", v[1]);
    if (v[2] != "") trk.opts.put("color", v[2]);
    if (v[3] != "") trk.name = cnv(v[3]);
    if (v[4] != "") trk.opts.put("ozi_skip", v[4]);
    if (v[5] != "") trk.opts.put("ozi_type", v[5]);
    if (v[6] != "") trk.opts.put("ozi_fill", v[6]);
    if (v[7] != "") trk.opts.put("bgcolor",  v[7]);
    getline(f,s1); // number of points -- ignored

    while (!f.eof()){
      GeoTpt pt;
      getline(f,s1);
      vector<string> v = unpack_ozi_csv(s1,7);
      if (v[0] == "" && v[1] == "") continue; // skip empty lines
      pt.y = str_to_type<double>(v[0]);
      pt.x = str_to_type<double>(v[1]);
      if (v[2] != "") pt.start = str_to_type<bool>(v[2]);
      if (v[3] != "" && v[3]!="-777") pt.z = str_to_type<double>(v[3]) * 0.3048;
      if (v[4] != "") pt.t = parse_ozi_time(v[4]);
      // fields 6,7 are ignored.
      trk.push_back(pt);
    }
    if (vv) cerr << "  Reading track: " << trk.name
                 << " (" << trk.size() << " points)" << endl;
    data.trks.push_back(trk);

    return;
  }

  //// WAYPOINTS
  if (s2 == "Waypoint"){
    GeoWptList wptl;
    getline(f,s1);
    crop_nl(s1);
    if (s1 != "WGS 84") throw Err() << "Unsupported datum: " << s1;
    getline(f,s1); // Reserved
    getline(f,s1); // symbol set (not used)

    while (!f.eof()){
      GeoWpt pt;
      getline(f,s1);
      vector<string> v = unpack_ozi_csv(s1,24);
      if (v[2] == "" && v[3] == "") continue; // skip empty lines
      pt.name = cnv(v[1]);
      pt.y = str_to_type<double>(v[2]);
      pt.x = str_to_type<double>(v[3]);
      if (v[4] != "") pt.t = parse_ozi_time(v[4]);
      if (v[5] != "") pt.opts.put("ozi_symb", v[5]);
      if (v[7] != "") pt.opts.put("ozi_map_displ", v[7]);
      if (v[8] != "") pt.opts.put("color",   v[8]);
      if (v[9] != "") pt.opts.put("bgcolor", v[9]);
      if (v[10] != "") pt.comm = cnv(v[10]);
      if (v[11] != "") pt.opts.put("ozi_pt_dir", v[11]);
      if (v[12] != "") pt.opts.put("ozi_displ", v[12]);
      if (v[13] != "") pt.opts.put("ozi_prox_dist", v[13]);
      if (v[14] != "" && v[14]!="-777") pt.z = str_to_type<double>(v[14]) * 0.3048;
      if (v[15] != "") pt.opts.put("ozi_font_size", v[15]);
      if (v[16] != "") pt.opts.put("ozi_font_style", v[16]);
      if (v[17] != "") pt.opts.put("ozi_symb_size", v[17]);
      if (v[17] != "") pt.opts.put("ozi_prox_pos",   v[18]);
      if (v[19] != "") pt.opts.put("ozi_prox_time",  v[19]);
      if (v[20] != "") pt.opts.put("ozi_prox_route", v[20]);
      if (v[21] != "") pt.opts.put("ozi_file",       v[21]);
      if (v[22] != "") pt.opts.put("ozi_prox_file",  v[22]);
      if (v[23] != "") pt.opts.put("ozi_prox_symb",  v[23]);
      wptl.push_back(pt);
    }
    if (vv) cerr << "  Reading waypoints: " << wptl.name
                 << " (" << wptl.size() << " points)" << endl;
    data.wpts.push_back(wptl);
    return;
  }

  //// MAP
  if (s2 == "Map"){
    GeoMapList ml;
    GeoMap m;
    string proj0; // for converting datum only!

    // map name
    getline(f,s1); crop_nl(s1);
    m.name = cnv(s1);

    // file name: no charset conversion! Add prefix if path is not absolute
    getline(f,s1); crop_nl(s1);
    if (s1.size()>0 && s1[0]!='/') m.image = prefix + s1;
    else m.image = s1;

    // 1 TIFF scale factor or Map Code  -- ignore
    getline(f,s1);

    // Datum -- use only first field, skip datum shift information
    getline(f,s1);
    vector<string> v = unpack_ozi_csv(s1, 1, 1);
    proj0 = m.proj = convert_ozi2datum(v[0]);

    // Two reserved lines:
    getline(f,s1);
    getline(f,s1);

    // Magnetic Variation -- ignore
    getline(f,s1);

    // Map Projection -- use only first field
    getline(f,s1);
    v = unpack_ozi_csv(s1, 2, 1);
    m.proj += " " + convert_ozi2proj(v[1]);

    if (f.fail()) throw Err() << "io_ozi: can't read map header";

    // POINTS
    map<dPoint, bool> geo; // geo or grid coordinates
    while(1) {
      getline(f,s1);
      v = unpack_ozi_csv(s1, 17, 1);
      if (v[0].compare(0,5, "Point")!=0) break;
      dPoint p1, p2;
      int type=0;

      if (v[2]=="" || v[3]=="") continue;
      p1.x = str_to_type<double>(v[2]);
      p1.y = str_to_type<double>(v[3]);

      if (v[6]!="" && v[7]!="" && v[8]!="" &&
          v[9]!="" && v[10]!="" && v[11]!="" ){
        p2.y = (str_to_type<double>(v[6]) +
                str_to_type<double>(v[7])/60.0) * (v[8]=="S"?-1:+1);
        p2.x = (str_to_type<double>(v[9]) +
                str_to_type<double>(v[10])/60.0) * (v[11]=="W"?-1:+1);
        type=1;
      }

      if (v[14]!="" && v[15]!="") {
        p2.x = str_to_type<double>(v[14]);
        p2.y = str_to_type<double>(v[15]);
        type=2;
      }
      if (type == 0) throw Err() << "io_ozi: no coordinates in " << v[0]
                                 << ": [" << s1 << "]";

      m.add_ref(p1,p2);
      geo.insert(pair<dPoint,bool>(p1,type==1));
    }

    if (f.fail()) throw Err() << "io_ozi: can't read map reference points";

    // Projection setup parmeters (use only 8 fields)
    v = unpack_ozi_csv(s1, 8, 1);
    if (v[0]!="Projection Setup")
      throw Err() << "io_ozi: Projection Setup expected";

    if (v[1]!="" && atof(v[1].c_str())!=0) m.proj += " +lat_0=" + v[1];
    if (v[2]!="" && atof(v[2].c_str())!=0) m.proj += " +lon_0=" + v[2];
    if (v[3]!="" && atof(v[3].c_str())!=1) m.proj += " +k="     + v[3];
    if (v[4]!="" && atof(v[4].c_str())!=1) m.proj += " +x_0="   + v[4];
    if (v[5]!="" && atof(v[5].c_str())!=1) m.proj += " +y_0="   + v[5];
    if (v[6]!="" && atof(v[6].c_str())!=0) m.proj += " +lat_1=" + v[6];
    if (v[7]!="" && atof(v[7].c_str())!=0) m.proj += " +lat_2=" + v[7];

    // now we know projection and can convert reference points
    proj0 += " +proj=latlong";
    ConvGeo gcnv1(proj0);  // map geo ->wgs
    ConvGeo gcnv2(m.proj); // map grid ->wgs
    for (auto & p: m.ref){
      if (geo[p.first]) gcnv1.frw(p.second);
      else gcnv2.frw(p.second);
    }

    // We read rest of the file in a relaxed way, without checking the correct
    // structure and selecting only MMPXY fields for border points
    // and IWH field for image size.
    // We care also about MC and MF fields because they
    // contain arbitrary trailing lines which should be skipped.

    dLine brd;
    while(!f.eof()) {
      getline(f,s1);
      v = unpack_ozi_csv(s1, 4, 1);
      if (v[0]=="MC"){ // map comment - skip one more line
        getline(f,s1);
      }
      if (v[0]=="MF"){ // map feature - skip two more lines
        getline(f,s1);
        getline(f,s1);
      }
      if (v[0]=="MMPXY"){
         dPoint p;
         p.x = atof(v[2].c_str());
         p.y = atof(v[3].c_str());
         brd.push_back(p);
      }
      if (v[0]=="IWH"){ // map width/height
        m.image_size.x = atoi(v[2].c_str());
        m.image_size.y = atoi(v[3].c_str());
      }
    }
    m.border.push_back(brd);
    ml.push_back(m);
    ml.name = m.name;
    data.maps.push_back(ml);
    if (vv) cerr << "  Reading map: " << m.name << endl;
    return;
  }
}

/***************************************************************************/
/// Write PLT.
void write_ozi_plt (const string &fname, const GeoTrk & trk, const Opt & opts){

  bool vv = opts.get("verbose", false);
  if (vv) cerr << "Writing track to OziExplorer file: " << fname << endl;

  ofstream f(fname);
  if (!f.good()) throw Err()
      << "Can't write data to OziExplorer file: " << fname;

  IConv cnv("UTF-8", opts.get("ozi_enc", ozi_default_enc) );

  int num = trk.size();
  f << "OziExplorer Track Point File Version 2.0\r\n"
    << "WGS 84\r\n"
    << "Altitude is in Feet\r\n"
    << "Reserved 3\r\n";

  // Field 1 : always zero
  // Field 2 : width of track plot line on screen - 1 or 2 are usually the best
  // Field 3 : track color (RGB)
  // Field 4 : track description (no commas allowed)
  // Field 5 : track skip value - reduces number of track points plotted, usually set to 1
  // Field 6 : track type - 0 = normal , 10 = closed polygon , 20 = Alarm Zone
  // Field 7 : track fill style - 0 =bsSolid; 1 =bsClear; 2 =bsBdiagonal;
  //  3 =bsFdiagonal; 4 =bsCross; 5 =bsDiagCross; 6 =bsHorizontal; 7 =bsVertical;
  // Field 8 : track fill color (RGB)
  f << "0"
    << "," << trk.opts.get("thickness", 1)
    << "," << trk.opts.get("color", 0x0000FF)
    << "," << convert_ozi_text(cnv(trk.name))
    << "," << trk.opts.get("ozi_skip", 1)
    << "," << trk.opts.get("ozi_type", 0)
    << "," << trk.opts.get("ozi_fill", 0)
    << "," << trk.opts.get("bgcolor", 0xFFFFFF)
    << "\r\n";
  f << num << "\r\n";

  GeoTrk::const_iterator p;
  for (auto p:trk){
    // Field 1 : Latitude - decimal degrees.
    // Field 2 : Longitude - decimal degrees.
    // Field 3 : Code - 0 if normal, 1 if break in track line
    // Field 4 : Altitude in feet (-777 if not valid)
    // Field 5 : Date - see Date Format below, if blank a preset date will be used
    // Field 6 : Date as a string (ignored)
    // Field 7 : Time as a string (ignored)
    f << right << fixed << setprecision(6) << setfill(' ')
        << setw(11) << p.y << ','
        << setw(11) << p.x << ','
        << ((p.start)? '1':'0') << ','
        << setprecision(1) << setw(7)
        << (p.have_alt() ? p.z/0.3048 : -777) << ','
        << setprecision(7) << setw(13)
        << write_ozi_time(p.t) << ','
        << write_fmt_time(" %d-%b-%y, %T", p.t) << "\r\n";
    }

  if (!f.good()) throw Err()
      << "Can't write data to OziExplorer file: " << fname;
}

/***************************************************************************/
/// Write WPT.
void write_ozi_wpt (const string &fname, const GeoWptList & wpt, const Opt & opts){
  bool vv = opts.get("verbose", false);
  if (vv) cerr << "Writing waypoints to OziExplorer file: " << fname << endl;

  ofstream f(fname);
  if (!f.good()) throw Err()
      << "Can't write data to OziExplorer file: " << fname;

  IConv cnv("UTF-8", opts.get("ozi_enc", ozi_default_enc) );

  int num = wpt.size();
  int n=0;
  f << "OziExplorer Waypoint File Version 1.1\r\n"
    << "WGS 84\r\n"
    << "Reserved 2\r\n"
    << "garmin\r\n"; // symbol set is not used in Ozi format

    for (auto p:wpt){

      // Field 1 : Number
      // Field 2 : Name - the waypoint name, use the correct length name to suit the GPS type.
      // Field 3 : Latitude - decimal degrees.
      // Field 4 : Longitude - decimal degrees.
      // Field 5 : Date - see Date Format below, if blank a preset date will be used
      // Field 6 : Symbol - 0 to number of symbols in GPS
      // Field 7 : Status - always set to 1
      // Field 8 : Map Display Format
      // Field 9 : Foreground Color (RGB value)
      // Field 10 : Background Color (RGB value)
      // Field 11 : Description (max 40), no commas
      // Field 12 : Pointer Direction
      // Field 13 : Garmin Display Format
      // Field 14 : Proximity Distance - 0 is off any other number is valid
      // Field 15 : Altitude - in feet (-777 if not valid)
      // Field 16 : Font Size - in points
      // Field 17 : Font Style - 0 is normal, 1 is bold.
      // Field 18 : Symbol Size - 17 is normal size
      // Field 19 : Proximity Symbol Position
      // Field 20 : Proximity Time
      // Field 21 : Proximity or Route or Both
      // Field 22 : File Attachment Name
      // Field 23 : Proximity File Attachment Name
      // Field 24 : Proximity Symbol Name
      f << ++n << ','
        << convert_ozi_text(cnv(p.name)) << ','
        << right << fixed << setprecision(6) << setfill(' ')
        << setw(11) << p.y << ','
        << setw(11) << p.x << ',';
      vector<string> v;
      v.push_back(write_ozi_time(p.t));
      v.push_back(p.opts.get("ozi_symb",       ""));
      v.push_back(" 1");
      v.push_back(p.opts.get("ozi_map_displ",  ""));
      v.push_back(p.opts.get("color",          ""));
      v.push_back(p.opts.get("bgcolor",        ""));
      v.push_back(convert_ozi_text(cnv(p.comm)));
      v.push_back(p.opts.get("ozi_pt_dir",     ""));
      v.push_back(p.opts.get("ozi_displ",      ""));
      v.push_back(p.opts.get("ozi_prox_dist",  ""));
      v.push_back(p.have_alt() ? type_to_str(p.z/0.3048) : "-777");
      v.push_back(p.opts.get("ozi_font_size",  ""));
      v.push_back(p.opts.get("ozi_font_style", ""));
      v.push_back(p.opts.get("ozi_symb_size",  ""));
      v.push_back(p.opts.get("ozi_prox_pos",   ""));
      v.push_back(p.opts.get("ozi_prox_time",  ""));
      v.push_back(p.opts.get("ozi_prox_route", ""));
      v.push_back(p.opts.get("ozi_file",       ""));
      v.push_back(p.opts.get("ozi_prox_file",  ""));
      v.push_back(p.opts.get("ozi_prox_symb",  ""));
      f << pack_ozi_csv(v) << "\r\n";
        // skip fields 19..24
    }
    if (!f.good()) throw Err()
        << "Can't write data to OziExplorer file: " << fname;

}

/***************************************************************************/
void write_ozi_map (const string &fname, const GeoMap & m, const Opt & opts){

  bool vv = opts.get("verbose", false);
  if (vv) cerr << "Writing map to OziExplorer file " << fname << endl;

  ofstream f(fname);
  if (!f.good()) throw Err()
      << "Can't write data to OziExplorer file: " << fname;

  IConv cnv("UTF-8", opts.get("ozi_enc", ozi_default_enc));

  // Usually wgs84 datum can be used here.
  string ozi_datum = "WGS 84";

  //if wgs84=false try to use map datum if it is known.
  if (!opts.get("ozi_map_wgs", false)){
    try { ozi_datum = convert_datum2ozi(m.proj); }
    catch (Err e) {};
  }

  // header
  f << "OziExplorer Map Data File Version 2.2\r\n"
    << cnv(m.name) << "\r\n"
    << m.image << "\r\n" // no charset conversion for file name!
      << "1 ,Map Code,\r\n"
      << ozi_datum << ",,   0.0000,   0.0000,WGS 84\r\n"
      << "Reserved 1\r\n"
      << "Reserved 2\r\n"
      << "Magnetic Variation,,,E\r\n"
      << "Map Projection," << convert_proj2ozi(m.proj)
      <<   ",PolyCal,No,AutoCalOnly,No,BSBUseWPX,No\r\n";

  // reference points
  int n = 0;

  int grid = opts.get("ozi_map_grid", 0);

  string proj0 = convert_ozi2datum(ozi_datum);
  proj0 += " +proj=latlong";
  ConvGeo gcnv1(proj0); // geo -> wgs
  ConvGeo gcnv2(m.proj); // grid -> wgs

  for (auto p:m.ref){
    n++;
    f << "Point" << setw(2) << setfill('0') << n << ",xy,"
      << setw(5) << setfill(' ') << (int)rint(p.first.x) << ','
      << setw(5) << setfill(' ') << (int)rint(p.first.y) << ','
      << "in, deg,";
    dPoint pp(p.second);
    if (!grid) {
      gcnv1.bck(pp);
      int xd = abs(int(pp.x));
      int yd = abs(int(pp.y));
      double xm = fabs(pp.x*60) - xd*60;
      double ym = fabs(pp.y*60) - yd*60;
      if (fabs(xm-60)<1e-5) {xm=0; xd--;}
      if (fabs(ym-60)<1e-5) {ym=0; yd--;}
      f << fixed << setprecision(6)
        << setw(4) << yd << ','
        << setw(6) << ym << ','
        << (pp.y<0? 'S':'N') << ','
        << setw(4) << xd << ','
        << setw(6) << xm << ','
        << (pp.x<0? 'W':'E') << ','
        << " grid,   ,           ,           ,N\r\n";
    }
    else {
      gcnv2.bck(pp);
      f << "    ,        ,N,    ,        ,E, grid,   ,"
        << setw(11) << int(rint(pp.x)) << ","
        << setw(11) << int(rint(pp.y)) << ",N\r\n";
    }
    if (n==30) break;
  }

  // empty reference points
  for (n++; n<=30; n++){
    f << "Point" << setw(2) << setfill('0') << n << ",xy,"
      <<"     ,     ,in, deg,    ,        ,N,    ,        ,W"
      <<", grid,   ,           ,           ,N\r\n";
    continue;
  }

  // projection setup
  vector<string> v;
  v.push_back("Projection Setup");
  v.push_back(get_proj_par(m.proj, "lat_0"));
  v.push_back(get_proj_par(m.proj, "lon_0"));
  v.push_back(get_proj_par(m.proj, "k"));
  v.push_back(get_proj_par(m.proj, "x_0"));
  v.push_back(get_proj_par(m.proj, "y_0"));
  v.push_back(get_proj_par(m.proj, "lat_1"));
  v.push_back(get_proj_par(m.proj, "lat_2"));
  f << pack_ozi_csv(v) << "\r\n";

  // map features
  f << "Map Feature = MF ; Map Comment = MC     These follow if they exist\r\n"
    << "Track File = TF      These follow if they exist\r\n"
    << "Moving Map Parameters = MM?    These follow if they exist\r\n";

  // map border
  // TODO: reduce number of points to 100

  // convert border to a single-segment line
  dLine brd = join_polygons(m.border);
  if (brd.size()>0){
    f << "MM0,Yes\r\n"
      << "MMPNUM," << brd.size() << "\r\n";
    int n=0;
    for (auto p:brd){
      n++;
      f << "MMPXY," << n << ","
        << int(rint(p.x)) << "," << int(rint(p.y)) << "\r\n";
    }

    n=0;
    ConvMap gcnv3(m, proj0); // image points -> latlong in map datum
    f.precision(8);
    for (auto p: brd ){
      n++;
      dPoint p1(p); gcnv3.frw(p1);
      f << "MMPLL," << n << ","
        << right << fixed << setprecision(6) << setfill(' ')
        << setw(10) << p1.x << ','
        << setw(11) << p1.y << "\r\n";
    }

    // calculate m/pix
    dPoint p1 = m.bbox_ref_img().cnt();
    dPoint p2 = p1 + dPoint(0,1);
    dPoint p3 = p1 + dPoint(1,0);
    gcnv3.frw(p1);
    gcnv3.frw(p2);
    gcnv3.frw(p3);
    double mpp = (geo_dist_2d(p1,p2) + geo_dist_2d(p1,p3))/2;
    f << "MM1B," << mpp << "\r\n";

  }
  else{
    f << "MM0,No\r\n";
  }
  f << "MOP,Map Open Position,0,0\r\n";
  f << "IWH,Map Image Width/Height,"
     << m.image_size.x << "," << m.image_size.y << "\r\n";

  if (!f.good()) throw Err()
      << "Can't write data to OziExplorer file: " << fname;
}

