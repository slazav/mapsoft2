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
std::vector<std::string> unpack_ozi_csv(const std::string & str, unsigned int count){
  int p1=0, p2=0;
  std::vector<std::string> ret;
  do {
    p2=str.find(',', p1);
    std::string field = str.substr(p1,p2-p1);
    std::replace( field.begin(), field.end(), (char)209, ',');
    ret.push_back(field);
    if (count && ret.size()==count) break;
    p1=p2+1;
  }
  while (p2!=std::string::npos);
  if (count) ret.resize(count);
  return ret;
}
/*******************************************************************/
/// Pack Ozi-style CSV lines.
/// Commas will be substituted by character 209.
/// Newline characters will be substituted by space.
/// Empty fields in the end will be skipped.
std::string pack_ozi_csv(const std::vector<std::string> & vec){
  std::string ret;

  // find last non-empty field
  size_t end=vec.size();
  while (end>0 && vec[end-1]=="") end--;

  for (size_t i=0; i!=end; ++i){
    std::string field = vec[i];
    std::replace( field.begin(), field.end(), ',', (char)209);
    std::replace( field.begin(), field.end(), '\n', ' ');
    ret += (i==0?"":",") + field;
  }
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
void write_ozi_plt (const char *fname, const GeoTrk & trk, const Opt & opts){
  if (opts.exists("verbose")) cerr <<
    "Writing track to OziExplorer file " << fname << endl;

  ofstream f(fname);
  if (!f.good()) throw Err()
      << "Can't write data to OziExplorer file: " << fname;

  IConv cnv("UTF-8", opts.get("ozi_enc", ozi_default_enc).c_str() );
/*
  int num = trk.size();
  f << "OziExplorer Track Point File Version 2.0\r\n"
    << "WGS 84\r\n"
    << "Altitude is in Feet\r\n"
    << "Reserved 3\r\n"
    << "0,"
      << trk.width << ','
      << (trk.color & 0xFFFFFF) << ','
      << cnv.from_utf8(trk.comm)  << ','
      << trk.skip  << ','
      << trk.type.val  << ','
      << trk.fill.val  << ','
      << (trk.cfill & 0xFFFFFF) << "\r\n"
      << num << "\r\n";
    for (vector<g_trackpoint>::const_iterator p = trk.begin();.
       p!= trk.end(); p++){
      f << right << fixed << setprecision(6) << setfill(' ')
        << setw(10)<< p->y << ','
        << setw(11)<< p->x << ','
        << ((p->start)? '1':'0') << ','
        << setprecision(1) << setw(6).
        << (p->have_alt() ? p->z/0.3048 : -777) << ','
        << setprecision(7) << setw(13)
        << (p->t.value+2209161600.0)/3600.0/24.0 << ','
        << setfill('0') << p->t << "\r\n";
    }
*/

  if (!f.good()) throw Err()
      << "Can't write data to OziExplorer file: " << fname;
}

/***************************************************************************/
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

    for (GeoWptList::const_iterator p = wpt.begin(); p!= wpt.end(); p++){
      vector<string> v;
      int alt = p->have_alt() ? p->z/0.3048 : -777;
      string comm = cnv(p->comm);
      if (comm.size()>40) comm.resize(40);

      v.push_back(type_to_str(++n));     // Field 1 : Number
      v.push_back(cnv(p->name));         // Field 2 : Name
      v.push_back(type_to_str(p->y));    // Field 3 : Latitude
      v.push_back(type_to_str(p->x));    // Field 4 : Longitude
      v.push_back(write_ozi_time(p->t)); // Field 5 : Date
      v.push_back("0");              // Field 6 : Symbol
      v.push_back("1");              // Field 7 : Status - always set to 1
      v.push_back("");               // Field 8 : Map Display Format
      v.push_back("");               // Field 9 : Foreground Color (RGB value)
      v.push_back("");               // Field 10 : Background Color (RGB value)
      v.push_back(comm);             // Field 11 : Description (max 40), no commas
      v.push_back("0");              // Field 12 : Pointer Direction
      v.push_back("0");              // Field 13 : Garmin Display Format
      v.push_back("0");              // Field 14 : Proximity Distance - 0 is off any other number is valid
      v.push_back(type_to_str(alt)); // Field 15 : Altitude - in feet (-777 if not valid)
      v.push_back("6");  // Field 16 : Font Size - in points
      v.push_back("0");  // Field 17 : Font Style - 0 is normal, 1 is bold.
      v.push_back("17"); // Field 18 : Symbol Size - 17 is normal size
      v.push_back("");   // Field 19 : Proximity Symbol Position
      v.push_back("");   // Field 20 : Proximity Time
      v.push_back("");   // Field 21 : Proximity or Route or Both
      v.push_back("");   // Field 22 : File Attachment Name
      v.push_back("");   // Field 23 : Proximity File Attachment Name
      v.push_back("");   // Field 24 : Proximity Symbol Name
      f << pack_ozi_csv(v) << "\r\n";
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

  std::string map_proj; // TODO!

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

