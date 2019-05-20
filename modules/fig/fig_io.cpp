#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>

#include "fig.h"
#include "iconv/iconv.h"

using namespace std;
string default_enc("KOI8-R");

/******************************************************************/
// extract a value from a single line
template <typename T>
void read_line(std::istream & s, T & val){
  std::string l;
  std::getline(s,l);
  std::istringstream s1(l);
  s1 >> val >> std::ws;
  if (s1.fail() || !s1.eof())
    throw Err() << "Fig: can't read fig file";
}

// read object text (char by char, with \XXX conversions).
// Returns true if '\\001' did not found.
// Data will be appended to text variable
bool read_text(std::istream & ss, string & text){
  while (1){
    char c = ss.get();
    if (ss.eof()) return true;
    if (ss.fail())
      throw Err() << "";
    if (c == '\\'){
      char c1,c2,c3;
      c1 = ss.get();
      if (ss.fail())
        throw Err() << "";
      if (c1 < '0' || c1 > '3'){
        c = c1;
      }
      else {
        if ((c2 = ss.get()) < '0' || c2 > '7' || ss.fail() ||
            (c3 = ss.get()) < '0' || c3 > '7' || ss.fail())
          throw Err() << "bad \\nnn octal number";

        c1 -= '0'; c2 -= '0'; c3 -= '0';
        c = (c1 << 6) +  (c2 << 3) + c3;
      }
    }
    if (c == 1) return false;
    text.push_back(c);
  }
  throw Err() << "broken code";
}

// read object comments, return first non-comment line
// (object header)
std::string
read_comments(std::istream & s, vector<string> & comment, const IConv & cnv){
  std::string ret;
  while (getline(s, ret)){
    if (s.fail()) throw Err() << "FigObj: can't read object";
    if (ret.size()<1) continue; // skip empty lines
    if (ret[0] != '#') break; // start reading the object ret
    if (ret.size()<2 || ret[1] != ' ')
      std::cerr << "FigObj: broken comment: [" << ret << "]\n";
    comment.push_back(cnv(ret.substr(2, -1)));
  }
  return ret;
}

// convert fig colors to/from rgb
int
color_fig2rgb(const int c, const map<int,int> & custom){
  map<int,int>::const_iterator i;
  i=Fig::colors.find(c);
  if (i!=Fig::colors.end()) return i->second;
  i=custom.find(c);
  if (i!=custom.end()) return i->second;
  cerr << "Fig: Unknown color: " << c << "\n";
  return c;
}
int
color_rgb2fig(const int c, const map<int,int> & custom){
  map<int,int>::const_iterator i;
  for (i=Fig::colors.begin(); i!=Fig::colors.end(); i++){
    if (i->first == -1 ) continue; // avoid -1 value ("default color")
    if (i->second == c) return i->first;
  }
  for (i=custom.begin(); i!=custom.end(); i++){
    if (i->second == c) return i->first;
  }
  cerr << "Fig: Unknown color: " << c << "\n";
  return -1;
}

// Add new rgb color to custom colormap if needed. Return true if color was added
bool
color_rgbadd(const int c, map<int,int> & custom){
  int maxc=0;
  map<int,int>::const_iterator i;
  for (i=Fig::colors.begin(); i!=Fig::colors.end(); i++){
    if (i->first > maxc) maxc=i->first;
    if (i->second == c) return false;
  }
  for (i=custom.begin(); i!=custom.end(); i++){
    if (i->first > maxc) maxc=i->first;
    if (i->second == c) return false;
  }
  custom.insert(pair<int,int>(maxc+1, c));
  return true;
}

/******************************************************************/
// Read object header string
int read_figobj_header(FigObj & o, const std::string & header){
  std::istringstream ss(header);
  ss >> o.type >> ws;
  if (ss.fail())
    throw Err() << "FigObj: can't read object: [" << header << "]";

  int ret = 0;
  int n;
  int x1,x2,x3,y1,y2,y3;
  switch (o.type) {
    case FIG_ELLIPSE:
      ss >> o.sub_type >> o.line_style >> o.thickness >> o.pen_color >> o.fill_color >> o.depth
         >> o.pen_style >> o.area_fill >> o.style_val >> o.direction >> o.angle
         >> x1 >> y1 >> o.radius_x >> o.radius_y >> o.start_x >> o.start_y >> o.end_x >> o.end_y
         >> std::ws;
      if (ss.fail() || !ss.eof())
        throw Err() << "FigObj: can't read ellipse object: [" << header << "]";
      o.push_back(iPoint(x1,y1));
      break;
    case FIG_POLYLINE:
      ss >> o.sub_type >> o.line_style >> o.thickness >> o.pen_color >> o.fill_color >> o.depth
         >> o.pen_style >> o.area_fill >> o.style_val >> o.join_style >> o.cap_style >> o.radius
         >> o.forward_arrow >> o.backward_arrow >> ret
         >> std::ws;
      if (ss.fail() || !ss.eof())
        throw Err() << "FigObj: can't read line object: [" << header << "]";
      break;
    case FIG_SPLINE:
      ss >> o.sub_type >> o.line_style >> o.thickness >> o.pen_color >> o.fill_color >> o.depth
         >> o.pen_style >> o.area_fill >> o.style_val >> o.cap_style
         >> o.forward_arrow >> o.backward_arrow >> ret
         >> std::ws;
      if (ss.fail() || !ss.eof())
        throw Err() << "FigObj: can't read spline object: [" << header << "]";
      break;
    case FIG_TXT:
      ss >> o.sub_type >> o.pen_color >> o.depth >> o.pen_style
         >> o.font >> o.font_size >> o.angle >> o.font_flags
         >> o.height >> o.length >> x1 >> y1;
      ss.get(); // read space;
      if (ss.fail())
        throw Err() << "FigObj: can't read text object: [" << header << "]";

      try{
        o.text.resize(0);
        ret = read_text(ss, o.text);
      }
      catch (Err e){
        throw Err() << "FigObj: can't read text: " << e.str()
                    << ": [" << header << "]";
      }
      o.push_back(iPoint(x1,y1));
      break;
    case FIG_ARC:
      ss >> o.sub_type >> o.line_style >> o.thickness >> o.pen_color >> o.fill_color >> o.depth
         >> o.pen_style >> o.area_fill >> o.style_val >> o.cap_style
         >> o.direction >> o.forward_arrow >> o.backward_arrow
         >> o.center_x >> o.center_y >> x1 >> y1 >> x2 >> y2 >> x3 >> y3
         >> std::ws;
      if (ss.fail() || !ss.eof())
         throw Err() << "FigObj: can't read arc object: [" << header << "]";
      o.push_back(iPoint(x1,y1));
      o.push_back(iPoint(x2,y2));
      o.push_back(iPoint(x3,y3));
      break;
    case FIG_COMPOUND:
      if (!ss.eof()) ss >> x1 >> y1 >> x2 >> y2 >> std::ws;
      else x1=x2=y1=y2=0;
      if (ss.fail() || !ss.eof())
        throw Err() << "FigObj: can't read compound object: [" << header << "]";
      o.push_back(iPoint(x1,y1));
      o.push_back(iPoint(x2,y2));
      break;
    case FIG_END_COMPOUND:
      if (ss.fail() || !ss.eof())
        throw Err() << "FigObj: can't read compound end object: [" << header << "]";
      break;
    case FIG_COLOR_DEF:
      throw Err() << "FigObj: color definitions must come before other objects: [" << header << "]";
    default:
      throw Err() << "FigObj: unknown object type: [" << header << "]";
  }
  return ret;
}


/******************************************************************/
void read_fig(std::istream & s, Fig & w, const Opt & ropts){
  string enc = ropts.get("fig_enc", default_enc);
  IConv cnv(enc.c_str(), "UTF-8");

  if (ropts.get("fig_header", 1)) {
    std::string l;
    std::getline(s, l);
    if (l.size()<8 || l.substr(0,8) != "#FIG 3.2")
      throw Err() << "Fig: non-supported format";

    std::getline(s, w.orientation);
    if (w.orientation!="Landscape" && w.orientation != "Portrait"){
      std::cerr << "Fig: unknown orientation setting: " << w.orientation << "\n";
      w.orientation = "Landscape";
    }
    std::getline(s, w.justification);
    if (w.justification!="Center" && w.justification != "Flush Left"){
      std::cerr << "Fig: unknown justification setting: " << w.justification << "\n";
      w.justification = "Center";
    }
    std::getline(s, w.units);
    if (w.units!="Metric" && w.units != "Inches"){
      std::cerr << "Fig: unknown units setting: " << w.units << "\n";
      w.units = "Metric";
    }
    std::getline(s, w.papersize);
    if (w.papersize!="Letter" && w.papersize != "Legal" && w.papersize != "Ledger" &&
        w.papersize != "Tabloid" && w.papersize != "A" && w.papersize != "B" &&
        w.papersize != "C" && w.papersize != "D" && w.papersize != "E" &&
        w.papersize != "A4" && w.papersize != "A3" && w.papersize != "A2" &&
        w.papersize != "A1" && w.papersize != "A0" && w.papersize != "B5"){
      std::cerr << "Fig: unknown papersize setting: " << w.papersize << "\n";
      w.papersize = "A4";
    }
    read_line(s, w.magnification);

    std::getline(s, w.multiple_page);
    if (w.multiple_page!="Single" && w.multiple_page != "Multiple"){
      std::cerr << "Fig: unknown multiple_page setting: " << w.multiple_page << "\n";
      w.multiple_page = "Single";
    }
    read_line(s, w.transparent_color);
    if (w.transparent_color<-3){
      std::cerr << "Fig: wrong transparent_color setting: " << w.transparent_color << "\n";
      w.transparent_color = -2;
    }

    // Read comments and options. Same as for FigObj,
    // using same method.
    l = read_comments(s, w.comment, cnv);

    // read resolution line
    std::istringstream s1(l);
    s1 >> w.resolution >> w.coord_system >> ws;
    if (s1.fail() || !s1.eof())
      throw Err() << "Fig: can't read fig file header";
  }

  // Read custom colors
  map<int,int> custom_colors;
  while (s.peek()=='0'){
    std::string l;
    std::getline(s, l);
    int type, ckey, cval;
    char c;
    std::istringstream s1(l);
    s1 >> type >> ckey >> c >> hex >> cval >> ws;
    if (s1.fail() || !s1.eof() || c != '#')
      throw Err() << "Fig: bad color object: [" << l << "]";
    if (Fig::colors.find(ckey) != Fig::colors.end())
      throw Err() << "Fig: redifinition of a predefined color: [" << l << "]";
    custom_colors.insert(make_pair(ckey, cval));
  }

  // Read objects
  while (1){
    if (s.eof()) break;
    FigObj o;
    std::string header = read_comments(s, o.comment, cnv);
    // parse object header
    int hret = read_figobj_header(o, header);
    // read extra text lines if needed
    if (o.type == FIG_TXT && hret){
      if (read_text(s, o.text))
        throw Err() << "FigObj: unfinished text";
    }

    // read arrow parameters if needed
    if (o.type == FIG_POLYLINE || o.type == FIG_SPLINE || o.type == FIG_ARC){
      if (o.forward_arrow) {
        std::string arr;
        getline(s, arr);
        std::istringstream s1(arr);
        s1 >> o.farrow_type >> o.farrow_style >> o.farrow_thickness
           >> o.farrow_width >> o.farrow_height >> std::ws;
        if (s1.fail() || !s1.eof())
          throw Err() << "FigObj: can't read arrow parameters: [" << arr << "]";
      }
      if (o.backward_arrow) {
        std::string arr;
        getline(s, arr);
        std::istringstream s1(arr);
        s1 >> o.barrow_type >> o.barrow_style >> o.barrow_thickness
           >> o.barrow_width >> o.barrow_height >> std::ws;
        if (s1.fail() || !s1.eof())
          throw Err() << "FigObj: can't read arrow parameters: [" << arr << "]";
      }
    }

    // read image
    if (o.type == FIG_POLYLINE && o.sub_type == 5) {
      s >> o.image_orient >> std::ws;
      getline(s, o.image_file);
      if (s.fail())
        throw Err() << "FigObj: can't image parameters";
      // xfig writes <empty> for empty filenames
      if (o.image_file == "<empty>") o.image_file = "";
    }

    // read coordinates
    if (o.type == FIG_POLYLINE || o.type == FIG_SPLINE) {
      o.resize(hret);
      for (int i=0; i<o.size(); i++){
        s >> o[i].x >> o[i].y >> std::ws;
        if (s.fail()) throw Err() << "FigObj: can't read line coordinates";
      }
    }
    // read spline factors
    if (o.type == FIG_SPLINE) {
      o.f.resize(hret);
      for (int i=0; i<o.f.size(); i++){
        s >> o.f[i] >> std::ws;
        if (s.fail()) throw Err() << "FigObj: can't read spline coordinates";
      }
    }

    s >> ws; // empty lines after the object
    if (s.fail()) throw Err() << "Fig: can't read fig file";

    // convert text encoding
    o.text = cnv(o.text);

    // convert colors to rgb
    o.pen_color  = color_fig2rgb(o.pen_color,  custom_colors);
    o.fill_color = color_fig2rgb(o.fill_color, custom_colors);

    w.push_back(o);
  }

  // convert colors
  // convert encoding
}


/******************************************************************/
// Writing
/******************************************************************/


/// write comments
void
write_comments(std::ostream & s, const vector<string> & comment, const IConv & cnv){
  int n;
  for (n=0; n<comment.size(); n++){
    if (n>99) {cerr << "fig comment contains > 100 lines! Cutting...\n"; break;}
    string str = comment[n];
    if (str.size()>1022){
      cerr << "fig comment line is > 1022 chars! Cutting...\n";
      str.resize(1022);
    }
    s << "# " << cnv(str) << "\n";
  }
}


// write arrow information for an object (spline, polyline, arc)
void
write_arrows(std::ostream & s, const FigObj & o){
  if (o.forward_arrow)
    s << "\t" << setprecision(2)
      << o.farrow_type << " "
      << o.farrow_style << " "
      << o.farrow_thickness << " "
      << o.farrow_width << " "
      << o.farrow_height << "\n";
  if (o.backward_arrow)
    s << "\t" << setprecision(2)
      << o.barrow_type << " "
      << o.barrow_style << " "
      << o.barrow_thickness << " "
      << o.barrow_width << " "
      << o.barrow_height << "\n";
}

// Write object coordinates (spline, polyline).
// Use addpt=1 to add first point at the end (only for closed polylines),
// 0 in other cases.
void
write_coords(std::ostream & s, const FigObj & o, const int add_pt=0) {
  int n = o.size();
  for (int i=0; i<n+add_pt; i++)
    s << ((i%6==0) ? "\t":" ")
      << o[i%n].x << " " << o[i%n].y
      << ((i%6==5 || i==n+add_pt-1) ? "\n":"");

  // write spline factors
  if (o.type == FIG_SPLINE){
    int n = o.f.size();
    s << setprecision(3);
    for (int i=0; i<n; i++)
      s << ((i%6==0) ? "\t":" ")
        << o.f[i]
        << ((i%6==5 || i==n-1) ? "\n":"");
  }
}

/******************************************************************/
void
write_fig(ostream & s, const Fig & w, const Opt & wopts){
  string enc = wopts.get("fig_enc", default_enc);
  IConv cnv("UTF-8", enc.c_str());

  // Writing header:
  if (wopts.get("fig_header", 1)) {
    s << "#FIG 3.2\n"
        << w.orientation << "\n" << w.justification << "\n"
        << w.units << "\n" << w.papersize << "\n"
        << setprecision(2) << fixed << w.magnification << "\n"
        << setprecision(3) << w.multiple_page << "\n"
        << w.transparent_color << "\n";

    // Write comments:
    write_comments(s, w.comment, cnv);
    s << w.resolution << " " << w.coord_system << "\n";
  }


  // Looking for custom colors in objects:
  map<int,int> custom_colors;
  for (Fig::const_iterator i=w.begin(); i!=w.end(); i++){
    color_rgbadd(i->pen_color,  custom_colors);
    color_rgbadd(i->fill_color, custom_colors);
  }
  // writing custom colors
  for (map<int,int>::const_iterator i = custom_colors.begin();
    i != custom_colors.end(); i++){
    s << "0 " << i->first << " #"
      << setbase(16) << setw(6) << setfill('0')
      << i->second << setbase(10) << "\n";
  }

  // Writing objects
  for (Fig::const_iterator i=w.begin(); i!=w.end(); i++){
    write_comments(s, i->comment, cnv);


    int pen_color  = color_rgb2fig(i->pen_color,  custom_colors);
    int fill_color = color_rgb2fig(i->fill_color, custom_colors);

    s << i->type << " " << std::setprecision(3) << std::fixed;
    int add_pt;
    switch (i->type) {
      case FIG_ELLIPSE:
        if (i->size()!=1) throw Err() << "FigObj: ellipse should have 1 coordinate point";
        s << i->sub_type <<" " << i->line_style << " " << i->thickness << " "
          << pen_color << " " << fill_color << " " << i->depth << " "
          << i->pen_style << " " << i->area_fill << " " << i->style_val << " "
          << i->direction << " " << std::setprecision(4) << i->angle << " "
          << (*i)[0].x << " " << (*i)[0].y << " "
          << i->radius_x << " " << i->radius_y << " " << i->start_x << " "
          << i->start_y << " " << i->end_x << " " << i->end_y << "\n";
        if (s.fail()) throw Err() << "FigObj: can't write ellipse object";
        break;
      case FIG_POLYLINE:
        if (i->size()<1) throw Err() << "FigObj: line should have at least 1 coordinate point";

        // If line if closed the last point should be same as first one.
        // Let's add the missing point if needed:
        add_pt = (i->is_closed() && (*i)[i->size()-1] != (*i)[0]) ? 1:0;

        s << i->sub_type << " " << i->line_style << " " << i->thickness << " "
          << pen_color << " " << fill_color << " " << i->depth << " "
          << i->pen_style << " " << i->area_fill << " " << i->style_val << " "
          << i->join_style << " " << i->cap_style << " " << i->radius << " "
          << i->forward_arrow << " " << i->backward_arrow << " " << i->size() + add_pt << "\n";
        write_arrows(s, *i);
        if (i->sub_type==5){ // image
          s << "\t" << i->image_orient << " "
            << (i->image_file == ""? "<empty>":i->image_file) << "\n";
        }
        write_coords(s, *i, add_pt);
        if (s.fail()) throw Err() << "FigObj: can't write line object";
        break;
      case FIG_SPLINE:
        if (i->is_closed()  && i->size()<3) throw Err() << "FigObj: closed spline with < 3 points";
        if (!i->is_closed() && i->size()<2) throw Err() << "FigObj: spline with < 2 points";
        if (i->size()!=i->f.size()) throw Err() << "FigObj: different amount of x,y and f values in a spline";
        s << i->sub_type << " " << i->line_style << " " << i->thickness << " "
          << pen_color << " " << fill_color << " " << i->depth << " "
          << i->pen_style << " " << i->area_fill << " " << i->style_val << " "
          << i->cap_style << " " << i->forward_arrow << " " << i->backward_arrow << " "
          << i->size() << "\n";
        write_arrows(s, *i);
        write_coords(s, *i);
        if (s.fail()) throw Err() << "FigObj: can't write spline object";
        break;
      case FIG_TXT:
        // TODO: convert charset, convert chars > 127
        // TODO: convert text dimensions?
        if (i->size()!=1) throw Err() << "FigObj: text should have 1 coordinate point";
        s << i->sub_type << " " << pen_color << " " << i->depth << " "
          << i->pen_style << " " << i->font << " " << i->font_size << " "
          << std::setprecision(4) << i->angle << " " << i->font_flags << " "
          << i->height << " " << i->length << " " << (*i)[0].x << " " << (*i)[0].y << " ";
        s << cnv(i->text) << "\\001\n";
        if (s.fail()) throw Err() << "FigObj: can't write text object";
        break;
      case FIG_ARC:
        if (i->size()!=3) throw Err() << "FigObj: arc should have 3 coordinate point";
        // TODO: calculate center!
        s << i->sub_type << " " << i->line_style << " " << i->thickness << " "
          << pen_color << " " << fill_color << " " << i->depth << " "
          << i->pen_style << " " << i->area_fill << " " << i->style_val << " "
          << i->cap_style << " " << i->direction << " "
          << i->forward_arrow << " " << i->backward_arrow << " "
          << i->center_x << " " << i->center_y << " "
          << (*i)[0].x << " " << (*i)[0].y << " "
          << (*i)[1].x << " " << (*i)[1].y << " "
          << (*i)[2].x << " " << (*i)[2].y << "\n";
        write_arrows(s, *i);
        if (s.fail()) throw Err() << "FigObj: can't write arc object";
        break;
      case FIG_COMPOUND:
        if (i->size()!=2) throw Err() << "FigObj: compound should have 2 coordinate point";
        s << (*i)[0].x << " " << (*i)[0].y << " "
          << (*i)[1].x << " " << (*i)[1].y << "\n";
        if (s.fail()) throw Err() << "FigObj: can't write compound object";
        break;
      case FIG_END_COMPOUND:
        if (s.fail()) throw Err() << "FigObj: can't write compound end object";
        break;
      default:
        throw Err() << "FigObj: unknown object type: " << i->type;
    }
  }
}

