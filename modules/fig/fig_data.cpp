#include <map>
#include <string>
#include <iomanip>

#include "iconv/iconv.h"
#include "err/err.h"

#include "fig_data.h"

using namespace std;

/*************************************************/
/// static data

// fig units, 1.05/1200in (not 1/1200in, I don't know why...)
const double Fig::cm2fig = 1200.0 / 1.05 / 2.54;
const double Fig::fig2cm = 1.0 / cm2fig;
const char *default_enc = "KOI8-R";

#define O_COLOR_DEF 0
#define O_ELLIPSE   1
#define O_POLYLINE  2
#define O_SPLINE    3
#define O_TXT       4
#define O_ARC       5
#define O_COMPOUND  6
#define O_END_COMPOUND -O_COMPOUND


// fig colors
typedef pair<int,int> p_ii;
const p_ii colors_a[] = {
  p_ii(-1, 0x000000), // default
  p_ii(0,  0x000000), // black
  p_ii(1,  0xff0000), // blue
  p_ii(2,  0x00ff00), // green
  p_ii(3,  0xffff00), // cyan
  p_ii(4,  0x0000ff), // red
  p_ii(5,  0xff00ff), // magenta
  p_ii(6,  0x00ffff), // yellow
  p_ii(7,  0xffffff), // white
  p_ii(8,  0x900000), // blue4
  p_ii(9,  0xb00000), // blue3
  p_ii(10, 0xd00000), // blue2
  p_ii(11, 0xffce87), // ltblue
  p_ii(12, 0x009000), // green4
  p_ii(13, 0x00b000), // green3
  p_ii(14, 0x00d000), // green2
  p_ii(15, 0x909000), // cyan4
  p_ii(16, 0xb0b000), // cyan3
  p_ii(17, 0xd0d000), // cyan2
  p_ii(18, 0x000090), // red4
  p_ii(19, 0x0000b0), // red3
  p_ii(20, 0x0000d0), // red2
  p_ii(21, 0x900090), // magenta4
  p_ii(22, 0xb000b0), // magenta3
  p_ii(23, 0xd000d0), // magenta2
  p_ii(24, 0x003080), // brown4
  p_ii(25, 0x0040a0), // brown3
  p_ii(26, 0x0060c0), // brown2
  p_ii(27, 0x8080ff), // pink4
  p_ii(28, 0xa0a0ff), // pink3
  p_ii(29, 0xc0c0ff), // pink2
  p_ii(30, 0xe0e0ff), // pink
  p_ii(31, 0x00d7ff)  // gold
};
const map<int,int> Fig::colors(&colors_a[0],
    &colors_a[sizeof(colors_a)/sizeof(p_ii)]);

// fig postscript fonts
typedef pair<int,string> p_is;
const p_is psfonts_a[] = {
  p_is(-1, "Default"),
  p_is( 0, "Times-Roman"),
  p_is( 1, "Times-Italic"),
  p_is( 2, "Times-Bold"),
  p_is( 3, "Times-BoldItalic"),
  p_is( 4, "AvantGarde-Book"),
  p_is( 5, "AvantGarde-BookOblique"),
  p_is( 6, "AvantGarde-Demi"),
  p_is( 7, "AvantGarde-DemiOblique"),
  p_is( 8, "Bookman-Light"),
  p_is( 9, "Bookman-LightItalic"),
  p_is(10, "Bookman-Demi"),
  p_is(11, "Bookman-DemiItalic"),
  p_is(12, "Courier"),
  p_is(13, "Courier-Oblique"),
  p_is(14, "Courier-Bold"),
  p_is(15, "Courier-BoldOblique"),
  p_is(16, "Helvetica"),
  p_is(17, "Helvetica-Oblique"),
  p_is(18, "Helvetica-Bold"),
  p_is(19, "Helvetica-BoldOblique"),
  p_is(20, "Helvetica-Narrow"),
  p_is(21, "Helvetica-Narrow-Oblique"),
  p_is(22, "Helvetica-Narrow-Bold"),
  p_is(23, "Helvetica-Narrow-BoldOblique"),
  p_is(24, "NewCenturySchlbk-Roman"),
  p_is(25, "NewCenturySchlbk-Italic"),
  p_is(26, "NewCenturySchlbk-Bold"),
  p_is(27, "NewCenturySchlbk-BoldItalic"),
  p_is(28, "Palatino-Roman"),
  p_is(29, "Palatino-Italic"),
  p_is(30, "Palatino-Bold"),
  p_is(31, "Palatino-BoldItalic"),
  p_is(32, "Symbol"),
  p_is(33, "ZapfChancery-MediumItalic"),
  p_is(34, "ZapfDingbats"),
};
const map<int,string> Fig::psfonts(&psfonts_a[0],
    &psfonts_a[sizeof(psfonts_a)/sizeof(p_is)]);

// fig tex fonts
const p_is texfonts_a[] = {
  p_is( 0, "Default"),
  p_is( 1, "Roman"),
  p_is( 2, "Bold"),
  p_is( 3, "Italic"),
  p_is( 4, "Sans Serif"),
  p_is( 5, "Typewriter")
};
const map<int,string> Fig::texfonts(&texfonts_a[0],
    &texfonts_a[sizeof(texfonts_a)/sizeof(p_is)]);

/*************************************************/

FigObj::FigObj(){
  type=0; sub_type=0; line_style=0; thickness=1; pen_color=0; fill_color=7; depth=50;
  pen_style=0; area_fill=-1; style_val=0.0; join_style=0; cap_style=0; radius=0;
  direction=1; angle=0.0; forward_arrow=0; backward_arrow=0; center_x=0; center_y=0;
  radius_x=0; radius_y=0; start_x=0; start_y=0; end_x=0; end_y=0; font=0; font_size=12;
  font_flags=0; image_orient=0;
  farrow_type=0; barrow_type=0;
  farrow_style=0; barrow_style=0;
  farrow_thickness=1; barrow_thickness=1;
  farrow_width=60; barrow_width=60;
  farrow_height=30; barrow_height=30;
  height=0; length=0;
  multiline=false;
}

/*************************************************/
bool
FigObj::read_text(std::istream & ss){
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

/*************************************************/
// read object comments and options, return first non-comment line
// (object header)
std::string
FigObj::read_comments(std::istream & s){
  std::string ret;
  while (getline(s, ret)){
    if (s.fail()) throw Err() << "FigObj: can't read object";
    if (ret.size()<1) continue; // skip empty lines
    if (ret[0] != '#') break; // start reading the object ret
    if (ret.size()<2 || ret[1] != ' ')
      throw Err() << "FigObj: broken comment: [" << ret << "]\n";
    // read option
    if (ret.size()>2 && ret[2] == '\\'){
      int p1 = 3;
      int p2 = ret.find('=');
      if (p2<p1) opts.put(ret.substr(p1,-1), 1);
      else opts.put(ret.substr(p1,p2-p1), ret.substr(p2+1,-1));
    }
    else {
      // read comment
      comment.push_back(ret.substr(2, -1));
    }
  }
  return ret;
}

/*************************************************/
void
FigObj::read_header(const std::string & header){
  std::istringstream ss(header);
  ss >> type >> ws;
  if (ss.fail())
    throw Err() << "FigObj: can't read object: [" << header << "]";

  int npts, n;
  int x1,x2,x3,y1,y2,y3;
  switch (type) {
    case O_ELLIPSE:
      ss >> sub_type >> line_style >> thickness >> pen_color >> fill_color >> depth
         >> pen_style >> area_fill >> style_val >> direction >> angle
         >> x1 >> y1 >> radius_x >> radius_y >> start_x >> start_y >> end_x >> end_y
         >> std::ws;
      if (ss.fail() || !ss.eof())
        throw Err() << "FigObj: can't read ellipse object: [" << header << "]";
      push_back(iPoint(x1,y1));
      break;
    case O_POLYLINE:
      ss >> sub_type >> line_style >> thickness >> pen_color >> fill_color >> depth
         >> pen_style >> area_fill >> style_val >> join_style >> cap_style >> radius
         >> forward_arrow >> backward_arrow >> npts
         >> std::ws;
      if (ss.fail() || !ss.eof())
        throw Err() << "FigObj: can't read line object: [" << header << "]";
      resize(npts);
      break;
    case O_SPLINE:
      ss >> sub_type >> line_style >> thickness >> pen_color >> fill_color >> depth
         >> pen_style >> area_fill >> style_val >> cap_style
         >> forward_arrow >> backward_arrow >> npts
         >> std::ws;
      if (ss.fail() || !ss.eof())
        throw Err() << "FigObj: can't read spline object: [" << header << "]";
      resize(npts);
      f.resize(npts);
      break;
    case O_TXT:
      ss >> sub_type >> pen_color >> depth >> pen_style
         >> font >> font_size >> angle >> font_flags
         >> height >> length >> x1 >> y1;
      ss.get(); // read space;
      if (ss.fail())
        throw Err() << "FigObj: can't read text object: [" << header << "]";

      try{
        text.resize(0);
        multiline = read_text(ss);
      }
      catch (Err e){
        throw Err() << "FigObj: can't read text: " << e.str()
                    << ": [" << header << "]";
      }
      push_back(iPoint(x1,y1));
      break;
    case O_ARC:
      ss >> sub_type >> line_style >> thickness >> pen_color >> fill_color >> depth
         >> pen_style >> area_fill >> style_val >> cap_style
         >> direction >> forward_arrow >> backward_arrow
         >> center_x >> center_y >> x1 >> y1 >> x2 >> y2 >> x3 >> y3
         >> std::ws;
      if (ss.fail() || !ss.eof())
         throw Err() << "FigObj: can't read arc object: [" << header << "]";
      push_back(iPoint(x1,y1));
      push_back(iPoint(x2,y2));
      push_back(iPoint(x3,y3));
      break;
    case O_COMPOUND:
      if (!ss.eof()) ss >> x1 >> y1 >> x2 >> y2 >> std::ws;
      else x1=x2=y1=y2=0;
      if (ss.fail() || !ss.eof())
        throw Err() << "FigObj: can't read compound object: [" << header << "]";
      push_back(iPoint(x1,y1));
      push_back(iPoint(x2,y2));
      break;
    case O_END_COMPOUND:
      if (ss.fail() || !ss.eof())
        throw Err() << "FigObj: can't read compound end object: [" << header << "]";
      break;
    case O_COLOR_DEF:
      throw Err() << "FigObj: color definitions must come before other objects: [" << header << "]";
    default:
      throw Err() << "FigObj: unknown object type: [" << header << "]";
  }
}

/*************************************************/
void
FigObj::read(std::istream & s){

  *this = FigObj(); // reset defaults

  std::string header = read_comments(s);

  // parse object header
  read_header(header);

  // read extra text lines if needed
  if (type == O_TXT && multiline){
    if (read_text(s))
      throw Err() << "FigObj: unfinished text";
  }

  // read arrow parameters if needed
  if (type == O_POLYLINE || type == O_SPLINE || type == O_ARC){
    if (forward_arrow) {
      std::string arr;
      getline(s, arr);
      std::istringstream s1(arr);
      s1 >> farrow_type >> farrow_style >> farrow_thickness
         >> farrow_width >> farrow_height >> std::ws;
      if (s1.fail() || !s1.eof())
        throw Err() << "FigObj: can't read arrow parameters: [" << arr << "]";
    }
    if (backward_arrow) {
      std::string arr;
      getline(s, arr);
      std::istringstream s1(arr);
      s1 >> barrow_type >> barrow_style >> barrow_thickness
         >> barrow_width >> barrow_height >> std::ws;
      if (s1.fail() || !s1.eof())
        throw Err() << "FigObj: can't read arrow parameters: [" << arr << "]";
    }
  }

  // read image
  if (type == O_POLYLINE && sub_type == 5) {
    s >> image_orient >> std::ws;
    getline(s, image_file);
    if (s.fail())
      throw Err() << "FigObj: can't image parameters";
    // xfig writes <empty> for empty filenames
    if (image_file == "<empty>") image_file = "";
  }

  // read coordinates
  if (type == O_POLYLINE || type == O_SPLINE) {
    for (int i=0; i<size(); i++){
      s >> (*this)[i].x >> (*this)[i].y >> std::ws;
      if (s.fail()) throw Err() << "FigObj: can't read line coordinates";
    }
  }

  // read spline factors
  if (type == O_SPLINE) {
    for (int i=0; i<f.size(); i++){
      s >> f[i] >> std::ws;
      if (s.fail()) throw Err() << "FigObj: can't read spline coordinates";
    }
  }
}

void
FigObj::write_comments(std::ostream & s) const{
  int n;
  // comments:
  for (n=0; n<comment.size(); n++){
    if (n>99) {cerr << "fig comment contains > 100 lines! Cutting...\n"; break;}
//    string str = cnv.from_utf8(comment[n]);
    string str = comment[n];
    if (str.size()>1022){
      cerr << "fig comment line is > 1022 chars! Cutting...\n";
      str.resize(1022);
    }
    s << "# " << str << "\n";
  }
  // options:
  for (Opt::const_iterator o=opts.begin(); o!=opts.end();o++){
    if (n++>99) {cerr << "fig comment contains > 100 lines! Cutting...\n"; break;}
//    string str = cnv.from_utf8(o->first) + "=" + cnv.from_utf8(o->second);
    string str = o->first + "=" + o->second;
    if (str.size()>1022){
      cerr << "fig comment line is > 1022 chars! Cutting...\n";
      str.resize(1022);
    }
    s << "# \\" << str << "\n";
  }
}

void
FigObj::write_arrows(std::ostream & s) const{
  if (forward_arrow)
    s << "\t" << setprecision(2)
      << farrow_type << " "
      << farrow_style << " "
      << farrow_thickness << " "
      << farrow_width << " "
      << farrow_height << "\n";
  if (backward_arrow)
    s << "\t" << setprecision(2)
      << barrow_type << " "
      << barrow_style << " "
      << barrow_thickness << " "
      << barrow_width << " "
      << barrow_height << "\n";
}
void
FigObj::write_coords(std::ostream & s, const int add_pt) const{
  int n = size();
  for (int i=0; i<n+add_pt; i++)
    s << ((i%6==0) ? "\t":" ")
      << (*this)[i%n].x << " " << (*this)[i%n].y
      << ((i%6==5 || i==n+add_pt-1) ? "\n":"");
}
void
FigObj::write_factors(std::ostream & s) const{
  int n = f.size();
  s << setprecision(3);
  for (int i=0; i<n; i++)
    s << ((i%6==0) ? "\t":" ")
      << f[i]
      << ((i%6==5 || i==n-1) ? "\n":"");
}

void
FigObj::write(std::ostream & s) const{
  write_comments(s);
  s << type << " " << std::setprecision(3) << std::fixed;
  int add_pt;
  switch (type) {
    case O_ELLIPSE:
      if (size()!=1) throw Err() << "FigObj: ellipse should have 1 coordinate point";
      s << sub_type <<" " << line_style << " " << thickness << " "
        << pen_color << " " << fill_color << " " << depth << " "
        << pen_style << " " << area_fill << " " << style_val << " "
        << direction << " " << std::setprecision(4) << angle << " "
        << (*this)[0].x << " " << (*this)[0].y << " "
        << radius_x << " " << radius_y << " " << start_x << " " 
        << start_y << " " << end_x << " " << end_y << "\n";
      if (s.fail()) throw Err() << "FigObj: can't write ellipse object";
      break;
    case O_POLYLINE:
      if (size()<1) throw Err() << "FigObj: line should have at least 1 coordinate point";

      // If line if closed the last point should be same as first one.
      // Let's add the missing point if needed:
      add_pt = (is_closed() && (*this)[size()-1] != (*this)[0]) ? 1:0;

      s << sub_type << " " << line_style << " " << thickness << " "
        << pen_color << " " << fill_color << " " << depth << " "
        << pen_style << " " << area_fill << " " << style_val << " "
        << join_style << " " << cap_style << " " << radius << " "
        << forward_arrow << " " << backward_arrow << " " << size() + add_pt << "\n";
      write_arrows(s);
      if (sub_type==5){ // image
        s << "\t" << image_orient << " "
          << (image_file == ""? "<empty>":image_file) << "\n";
      }
      write_coords(s, add_pt);
      if (s.fail()) throw Err() << "FigObj: can't write line object";
      break;
    case O_SPLINE:
      if (is_closed()  && size()<3) throw Err() << "FigObj: closed spline with < 3 points";
      if (!is_closed() && size()<2) throw Err() << "FigObj: spline with < 2 points";
      if (size()!=f.size()) throw Err() << "FigObj: different amount of x,y and f values in a spline";
      s << sub_type << " " << line_style << " " << thickness << " "
        << pen_color << " " << fill_color << " " << depth << " "
        << pen_style << " " << area_fill << " " << style_val << " "
        << cap_style << " " << forward_arrow << " " << backward_arrow << " "
        << size() << "\n";
      write_arrows(s);
      write_coords(s);
      write_factors(s);
      if (s.fail()) throw Err() << "FigObj: can't write spline object";
      break;
    case O_TXT:
      // TODO: convert charset, convert chars > 127
      // TODO: convert text dimensions?
      if (size()!=1) throw Err() << "FigObj: text should have 1 coordinate point";
      s << sub_type << " " << pen_color << " " << depth << " "
        << pen_style << " " << font << " " << font_size << " "
        << std::setprecision(4) << angle << " " << font_flags << " "
        << height << " " << length << " " << (*this)[0].x << " " << (*this)[0].y << " ";
      s << text << "\\001\n";
      if (s.fail()) throw Err() << "FigObj: can't write text object";
      break;
    case O_ARC:
      if (size()!=3) throw Err() << "FigObj: arc should have 3 coordinate point";
      // TODO: calculate center!
      s << sub_type << " " << line_style << " " << thickness << " "
        << pen_color << " " << fill_color << " " << depth << " "
        << pen_style << " " << area_fill << " " << style_val << " "
        << cap_style << " " << direction << " "
        << forward_arrow << " " << backward_arrow << " "
        << center_x << " " << center_y << " "
        << (*this)[0].x << " " << (*this)[0].y << " "
        << (*this)[1].x << " " << (*this)[1].y << " "
        << (*this)[2].x << " " << (*this)[2].y << "\n";
      write_arrows(s);
      if (s.fail()) throw Err() << "FigObj: can't write arc object";
      break;
    case O_COMPOUND:
      if (size()!=2) throw Err() << "FigObj: compound should have 2 coordinate point";
      s << (*this)[0].x << " " << (*this)[0].y << " "
        << (*this)[1].x << " " << (*this)[1].y << "\n";
      if (s.fail()) throw Err() << "FigObj: can't write compound object";
      break;
    case O_END_COMPOUND:
      if (s.fail()) throw Err() << "FigObj: can't write compound end object";
      break;
    default:
      throw Err() << "FigObj: unknown object type: " << type;
  }
}


/*************************************************/
bool
FigObj::operator== (const FigObj & o) const{
  if (type       != o.type)       return false;
  if (sub_type   != o.sub_type)   return false;
  if (line_style != o.line_style) return false;
  if (thickness  != o.thickness)  return false;
  if (pen_color  != o.pen_color)  return false;
  if (fill_color != o.fill_color) return false;
  if (depth      != o.depth)      return false;
  if (pen_style  != o.pen_style)  return false;
  if (area_fill  != o.area_fill)  return false;
  if (style_val  != o.style_val)  return false;
  if (join_style != o.join_style) return false;
  if (cap_style  != o.cap_style)  return false;
  if (radius     != o.radius)     return false;
  if (direction  != o.direction)  return false;
  if (angle      != o.angle)      return false;
  if (forward_arrow  != o.forward_arrow)  return false;
  if (backward_arrow != o.backward_arrow) return false;
  if (center_x != o.center_x) return false;
  if (center_y != o.center_y) return false;
  if (radius_x != o.radius_x) return false;
  if (radius_y != o.radius_y) return false;
  if (start_x  != o.start_x)  return false;
  if (start_y  != o.start_y)  return false;
  if (end_x    != o.end_x)    return false;
  if (end_y    != o.end_y)    return false;
  if (font       != o.font)       return false;
  if (font_size  != o.font_size)  return false;
  if (font_flags != o.font_flags) return false;
  if (height != o.height) return false;
  if (length != o.length) return false;
  if (forward_arrow) {
    if (farrow_type      != o.farrow_type) return false;
    if (farrow_style     != o.farrow_style) return false;
    if (farrow_thickness != o.farrow_thickness) return false;
    if (farrow_width     != o.farrow_width) return false;
    if (farrow_height    != o.farrow_height) return false;
  }
  if (backward_arrow) {
    if (barrow_type      != o.barrow_type) return false;
    if (barrow_style     != o.barrow_style) return false;
    if (barrow_thickness != o.barrow_thickness) return false;
    if (barrow_width     != o.barrow_width) return false;
    if (barrow_height    != o.barrow_height) return false;
  }
  if (image_file   != o.image_file) return false;
  if (image_orient != o.image_orient) return false;
  if (text    != o.text)    return false;
  if (comment != o.comment) return false;
  if (opts    != o.opts)    return false;
  if (iLine::operator!=(o)) return false;
  if (f!=o.f) return false;
  return true;
}

bool
FigObj::operator< (const FigObj & o) const{
  if (type != o.type) return (type < o.type);
  if (sub_type != o.sub_type) return (sub_type < o.sub_type);
  if (line_style != o.line_style) return (line_style < o.line_style);
  if (thickness != o.thickness) return (thickness < o.thickness);
  if (pen_color != o.pen_color) return (pen_color < o.pen_color);
  if (fill_color != o.fill_color) return (fill_color < o.fill_color);
  if (depth != o.depth) return (depth < o.depth);
  if (pen_style != o.pen_style) return (pen_style < o.pen_style);
  if (area_fill != o.area_fill) return (area_fill < o.area_fill);
  if (style_val != o.style_val) return (style_val < o.style_val);
  if (join_style != o.join_style) return (join_style < o.join_style);
  if (cap_style != o.cap_style) return (cap_style < o.cap_style);
  if (radius != o.radius) return (radius < o.radius);
  if (direction != o.direction) return (direction < o.direction);
  if (angle != o.angle) return (angle < o.angle);
  if (forward_arrow != o.forward_arrow) return (forward_arrow < o.forward_arrow);
  if (backward_arrow != o.backward_arrow) return (backward_arrow < o.backward_arrow);
  if (center_x != o.center_x) return (center_x < o.center_x);
  if (center_y != o.center_y) return (center_y < o.center_y);
  if (radius_x != o.radius_x) return (radius_x < o.radius_x);
  if (radius_y != o.radius_y) return (radius_y < o.radius_y);
  if (start_x != o.start_x) return (start_x < o.start_x);
  if (start_y != o.start_y) return (start_y < o.start_y);
  if (end_x != o.end_x) return (end_x < o.end_x);
  if (end_y != o.end_y) return (end_y < o.end_y);
  if (font != o.font) return (font < o.font);
  if (font_size != o.font_size) return (font_size < o.font_size);
  if (font_flags != o.font_flags) return (font_flags < o.font_flags);
  if (height != o.height) return (height < o.height);
  if (length != o.length) return (length < o.length);
  if (forward_arrow) {
    if (farrow_type != o.farrow_type) return (farrow_type < o.farrow_type);
    if (farrow_style != o.farrow_style) return (farrow_style < o.farrow_style);
    if (farrow_thickness != o.farrow_thickness) return (farrow_thickness < o.farrow_thickness);
    if (farrow_width != o.farrow_width) return (farrow_width < o.farrow_width);
    if (farrow_height != o.farrow_height) return (farrow_height < o.farrow_height);
  }
  if (backward_arrow) {
    if (barrow_type != o.barrow_type) return (barrow_type < o.barrow_type);
    if (barrow_style != o.barrow_style) return (barrow_style < o.barrow_style);
    if (barrow_thickness != o.barrow_thickness) return (barrow_thickness < o.barrow_thickness);
    if (barrow_width != o.barrow_width) return (barrow_width < o.barrow_width);
    if (barrow_height != o.barrow_height) return (barrow_height < o.barrow_height);
  }
  if (image_file != o.image_file) return (image_file < o.image_file);
  if (image_orient != o.image_orient) return (image_orient < o.image_orient);
  if (text != o.text) return (text < o.text);
  if (comment != o.comment) return (comment < o.comment);
  if (opts != o.opts) return (opts < o.opts); // ?!
  if (f != o.f) return (f < o.f);
  return iLine::operator<(o);
}

void FigObj::set_points(const dLine & v){
  clear();
  for (int i=0;i<v.size();i++)
    push_back(iPoint(lround(v[i].x), lround(v[i].y)));
}


/*************************************************/

void FigObj::set_points(const iLine & v){
  clear();
  insert(end(), v.begin(), v.end());
}

void Fig::remove_empty_comp(){
  int removed;
  do{
    removed=0;
    iterator o=begin();
    while (o!=end()){
      if (o->type==6){
        iterator on=o; on++;
        if ((on!=end()) && (on->type==-6)){
          o=erase(o);
          o=erase(o);
          removed++;
          continue;
        }
      }
      o++;
    }
  }
  while(removed>0);
}

template <typename T>
void read_line(std::istream & s, T val){
  std::string l;
  std::getline(s,l);
  std::istringstream s1(l);
  s1 >> val >> std::ws;
  if (s1.fail() || !s1.eof())
    throw Err() << "Fig: can't read fig file";
}

void
Fig::read(std::istream & s, const Opt & ropts){
  std::string l;
  std::getline(s,l);
  if (l.size()<8 || l.substr(0,8) != "#FIG 3.2")
    throw Err() << "Fig: non-supported format";

  read_line(s, orientation);
  read_line(s, justification);
  read_line(s, units);
  read_line(s, papersize);
  read_line(s, magnification);
  read_line(s, multiple_page);
  read_line(s, transparent_color);

  // Read comments and options. Same as for FigObj,
  // using same method.
  FigObj tmp;
  tmp.read_comments(s);
  comment = tmp.comment;
  opts    = tmp.opts;

  // Read custom colors

  // Read objects
  while (1){
    FigObj o;
    o.read(s);
    if (s.eof()) break;
    if (s.fail()) throw Err() << "Fig: can't read fig file";
    push_back(o);
  }

  // convert colors
  // convert encoding
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

void
Fig::write(std::ostream & s, const Opt & wopts) const{
  string enc = opts.get("fig_out_enc", std::string(default_enc));
  IConv cnv(enc.c_str(), "UTF-8");

  // Writing header:
  s << "#FIG 3.2\n"
      << orientation << "\n" << justification << "\n"
      << units << "\n" << papersize << "\n"
      << setprecision(2) << fixed << magnification << "\n"
      << setprecision(3) << multiple_page << "\n"
      << transparent_color << "\n";

  // Write comments and options. Same as for FigObj,
  // using same method.
  FigObj tmp;
  tmp.comment=comment; tmp.opts=opts;
  tmp.write_comments(s);

  s << resolution << " " << coord_system << "\n";

  // Looking for custom colors:
  map<int,int> custom_colors;
  for (const_iterator i=begin(); i!=end(); i++){
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

  // writing objects
  for (const_iterator i=begin(); i!=end(); i++) i->write(s);
}
