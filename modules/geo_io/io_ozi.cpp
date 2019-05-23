#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "iconv/iconv.h"
#include "time_fmt/time_fmt.h"

#include "io_ozi.h"


using namespace std;
string ozi_default_enc("WINDOWS-1251");

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
vector<string> unpack_ozi_csv(const string & str, unsigned int count){
  int p1=0, p2=0;
  vector<string> ret;
  do {
    p2=str.find(',', p1);
    string field = str.substr(p1,p2-p1);
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
/***************************************************************************/
void read_ozi (const char *fname, GeoData & data, const Opt & opts){
  if (opts.exists("verbose")) cerr <<
    "Reading OziExplorer file " << fname << endl;

  ifstream f(fname);
  if (!f.good()) throw Err()
      << "Can't read data from OziExplorer file: " << fname;

  IConv cnv(opts.get("ozi_enc", ozi_default_enc).c_str(), "UTF-8");

  /**/
}

/***************************************************************************/
/// Write PLT.
void write_ozi_plt (const char *fname, const GeoTrk & trk, const Opt & opts){
  if (opts.exists("verbose")) cerr <<
    "Writing track to OziExplorer file " << fname << endl;

  ofstream f(fname);
  if (!f.good()) throw Err()
      << "Can't write data to OziExplorer file: " << fname;

  IConv cnv("UTF-8", opts.get("ozi_enc", ozi_default_enc).c_str() );

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
        << setw(10)<< p.y << ','
        << setw(11)<< p.x << ','
        << ((p.start)? '1':'0') << ','
        << setprecision(1) << setw(6)
        << (p.have_alt() ? p.z/0.3048 : -777) << ','
        << setprecision(7) << setw(13)
        << write_ozi_time(p.t) << ','
        << write_fmt_time("%d-%b-%y, %T", p.t) << "\r\n";
    }

  if (!f.good()) throw Err()
      << "Can't write data to OziExplorer file: " << fname;
}

/***************************************************************************/
/// Write WPT.
void write_ozi_wpt (const char *fname, const GeoWptList & wpt, const Opt & opts){
  if (opts.exists("verbose")) cerr <<
    "Writing waypoints to OziExplorer file " << fname << endl;

  ofstream f(fname);
  if (!f.good()) throw Err()
      << "Can't write data to OziExplorer file: " << fname;

  IConv cnv("UTF-8", opts.get("ozi_enc", ozi_default_enc).c_str() );

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
        << fixed << setprecision(6) << p.y << ','
        << fixed << setprecision(6) << p.x << ','
        << write_ozi_time(p.t) << ','
        << p.opts.get("ozi_symb", 0) << ','
        << 1 << ','
        << p.opts.get("ozi_map_displ", 0) << ','
        << p.opts.get("color", 0xFF0000) << ','
        << p.opts.get("bgcolor", 0xFFFFFF) << ','
        << convert_ozi_text(cnv(p.comm)) << ','
        << p.opts.get("ozi_pt_dir", 0) << ','
        << p.opts.get("ozi_displ", 0) << ','
        << p.opts.get("ozi_prox_dist", 0) << ','
        << (p.have_alt() ? p.z/0.3048 : -777) << ','
        << p.opts.get("ozi_font_size", 0) << ','
        << p.opts.get("ozi_font_style", 0) << ','
        << p.opts.get("ozi_symb_size", 0)<< "\r\n";
        // skip fields 19..24
    }
    if (!f.good()) throw Err()
        << "Can't write data to OziExplorer file: " << fname;

}

/***************************************************************************/
void write_ozi_map (const char *fname, const GeoMap & m, const Opt & opts){
  if (opts.exists("verbose")) cerr <<
    "Writing map to OziExplorer file " << fname << endl;

  ofstream f(fname);
  if (!f.good()) throw Err()
      << "Can't write data to OziExplorer file: " << fname;

  IConv cnv("UTF-8", opts.get("ozi_enc", ozi_default_enc).c_str() );

  string map_proj; // TODO!

  // header
  f << "OziExplorer Map Data File Version 2.2\r\n"
    << cnv(m.name) << "\r\n"
    << m.image << "\r\n"
      << "1 ,Map Code,\r\n"
      << "WGS 84,,   0.0000,   0.0000,WGS 84\r\n"
      << "Reserved 1\r\n"
      << "Reserved 2\r\n"
      << "Magnetic Variation,,,E\r\n"
      << "Map Projection," << map_proj
      << ",PolyCal,No,AutoCalOnly,No,BSBUseWPX,No\r\n";

  // reference points
  int n = 0;
  map<dPoint,dPoint>::const_iterator i;
  for (i=m.ref.begin(); i!=m.ref.end(); i++){
    n++;
    int x = (int)i->second.x;
    int y = (int)i->second.y;
    double lat = i->first.y;
    double lon = i->first.x;
    f << "Point" << setw(2) << setfill('0') << n << ",xy,"
      << setw(5) << setfill(' ') << x << ','
      << setw(5) << setfill(' ') << y << ','
      << "in, deg,"
      << fixed << setw(4) << abs(int(lat)) << ','
      << fixed << setw(8) << fabs(lat*60) - abs(int(lat))*60 << ','
      << (lat<0? 'S':'N') << ','
      << fixed << setw(4) << abs(int(lon))
      << ',' << fixed << setw(8) << fabs(lon*60) - abs(int(lon))*60 << ','
      << (lon<0? 'W':'E') << ','
      << " grid,   ,           ,           ,N\r\n";
    if (n==30) break;
  }

  // empty reference points
  for (n++; n<=30; n++){
    f << "Point" << setw(2) << setfill('0') << n << ",xy,"
      <<"     ,     ,in, deg,    ,        ,N,    ,        ,W"
      <<", grid,   ,           ,           ,N\r\n";
    continue;
  }

/*
  // projection setup
  if (map_proj == Proj("lonlat")){
    f << "Projection Setup,,,,,,,,,,\r\n";
  }
  else {
    double lon0 = m.proj_opts.get("lon0", 1e90);
    if (lon0==1e90){
      lon0=0;
      for (int i=0; i<m.size(); i++) lon0+=m[i].x;
      if (m.size()>1) lon0/=m.size();
      lon0 = floor( lon0/6.0 ) * 6 + 3;
    }
    f << "Projection Setup, "
      << m.proj_opts.get("lat0", 0.0) << ","
      << lon0 << ","
      << m.proj_opts.get("k", 1.0) << ","
      << m.proj_opts.get("E0", 500000.0) << ","
      << m.proj_opts.get("N0", 0.0) << ","
      << m.proj_opts.get("lat1", 0.0) << ","
      << m.proj_opts.get("lat2", 0.0) << ","
      << m.proj_opts.get("hgt", 0.0) << ",,\r\n";
  }

  // map features
  f << "Map Feature = MF ; Map Comment = MC     These follow if they exist\r\n"
    << "Track File = TF      These follow if they exist\r\n"
    << "Moving Map Parameters = MM?    These follow if they exist\r\n";

  // map border
  if (m.border.size()>0){
    f << "MM0,Yes\r\n"
      << "MMPNUM," << m.border.size() << "\r\n";
    int n=0;
    dLine::const_iterator it;
    for (it =m.border.begin(); it!=m.border.end(); it++){
      n++;
      f << "MMPXY," << n << ","
        << int(it->x) << "," << int(it->y) << "\r\n";.
    }
    n=0;
    convs::map2wgs cnv(m);
    f.precision(8);
    for (it =m.border.begin(); it!=m.border.end(); it++){
      n++;
      dPoint p1=*it; cnv.frw(p1);
      f << "MMPLL," << n << ",".
        << right << fixed << setprecision(6) << setfill(' ')
        << setw(10) << p1.x << ','
        << setw(11) << p1.y << "\r\n";.
    }

    f << "MM1B," << convs::map_mpp(m, m.map_proj) << "\r\n";
  }
  else{
    f << "MM0,No\r\n";
  }
*/

  if (!f.good()) throw Err()
      << "Can't write data to OziExplorer file: " << fname;
}

