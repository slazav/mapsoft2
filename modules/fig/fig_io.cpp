#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>

#include "fig_io.h"
#include "iconv/iconv.h"

using namespace std;
string default_enc("KOI8-R");

/****************************************************************/
FigObj
read_figobj_header(const std::string & header){
  FigObj ret;
  std::istringstream ss(header);
  ss >> type >> ws;
  if (ss.fail())
    throw Err() << "FigObj: can't read object: [" << header << "]";

  int npts, n;
  int x1,x2,x3,y1,y2,y3;
  switch (type) {
    case O_ELLIPSE:
      ss >> o.sub_type >> o.line_style >> o.thickness
         >> o.pen_color >> o.fill_color >> o.depth
         >> o.pen_style >> o.area_fill >> o.style_val
         >> o.direction >> o.angle
         >> x1 >> y1 >> o.radius_x >> o.radius_y
         >> o.start_x >> o.start_y >> o.end_x >> o.end_y
         >> std::ws;
      if (ss.fail() || !ss.eof())
        throw Err() << "FigObj: can't read ellipse object: [" << header << "]";
      o.push_back(iPoint(x1,y1));
      break;
    case O_POLYLINE:
      ss >> o.sub_type >> o.line_style >> o.thickness
         >> o.pen_color >> o.fill_color >> o.depth
         >> o.pen_style >> o.area_fill >> o.style_val
         >> o.join_style >> o.cap_style >> o.radius
         >> o.forward_arrow >> o.backward_arrow >> npts
         >> std::ws;
      if (ss.fail() || !ss.eof())
        throw Err() << "FigObj: can't read line object: [" << header << "]";
      o.resize(npts);
      break;
    case O_SPLINE:
      ss >> o.sub_type >> o.line_style >> o.thickness
         >> o.pen_color >> fill_color >> depth
         >> o.pen_style >> o.area_fill >> o.style_val >> o.cap_style
         >> o.forward_arrow >> o.backward_arrow >> npts
         >> std::ws;
      if (ss.fail() || !ss.eof())
        throw Err() << "FigObj: can't read spline object: [" << header << "]";
      o.resize(npts);
      o.f.resize(npts);
      break;
    case O_TXT:
      ss >> o.sub_type >> o.pen_color >> o.depth >> o.pen_style
         >> o.font >> o.font_size >> o.angle >> o.font_flags
         >> o.height >> o.length >> x1 >> y1;
      ss.get(); // read space;
      if (ss.fail())
        throw Err() << "FigObj: can't read text object: [" << header << "]";

      try{
        o.multiline = read_text(ss, text);
      }
      catch (Err e){
        throw Err() << "FigObj: can't read text: " << e.str()
                    << ": [" << header << "]";
      }
      o.push_back(iPoint(x1,y1));
      break;
    case O_ARC:
      ss >> o.sub_type >> o.line_style >> o.thickness
         >> o.pen_color >> o.fill_color >> o.depth
         >> o.pen_style >> o.area_fill >> o.style_val
         >> o.cap_style >> o.direction
         >> o.forward_arrow >> o.backward_arrow
         >> o.center_x >> o.center_y >> x1 >> y1 >> x2 >> y2 >> x3 >> y3
         >> std::ws;
      if (ss.fail() || !ss.eof())
         throw Err() << "FigObj: can't read arc object: [" << header << "]";
      o.push_back(iPoint(x1,y1));
      o.push_back(iPoint(x2,y2));
      o.push_back(iPoint(x3,y3));
      break;
    case O_COMPOUND:
      if (!ss.eof()) ss >> x1 >> y1 >> x2 >> y2 >> std::ws;
      else x1=x2=y1=y2=0;
      if (ss.fail() || !ss.eof())
        throw Err() << "FigObj: can't read compound object: [" << header << "]";
      o.push_back(iPoint(x1,y1));
      o.push_back(iPoint(x2,y2));
      break;
    case O_END_COMPOUND:
      if (ss.fail() || !ss.eof())
        throw Err() << "FigObj: can't read compound end object: [" << header << "]";
      break;
    case O_COLOR_DEF:
      throw Err() << "FigObj: color definitions must come before other objects: [" << header <
    default:
      throw Err() << "FigObj: unknown object type: [" << header << "]";
  }
  return o;
}
/****************************************************************/


bool read_fig(const char* filename, Fig & data, const Opt & opts = Opt()){
}


/****************************************************************/
/****************************************************************/

int f_arr, b_arr;
bool no_f_arr(){return f_arr==0;}
bool no_b_arr(){return b_arr==0;}
int npoints, npoints_f;
bool point_counter(){npoints--; return npoints>=0;}
bool point_f_counter(){npoints_f--; return npoints_f>=0;}
int sub_type;
bool no_picture(){return sub_type!=5;}

// convert fig colors to/from rgb
int
color_fig2rgb(const int c, const map<int,int> & custom){
  map<int,int>::const_iterator i;
  i=colors.find(c);
  if (i!=colors.end()) return i->second;
  i=custom.find(c);
  if (i!=custom.end()) return i->second;
  cerr << "Unknown fig color: " << c << "\n";
  return c;
}
int
color_rgb2fig(const int c, const map<int,int> & custom){
  map<int,int>::const_iterator i;
  for (i=colors.begin(); i!=colors.end(); i++){
    if (i->first == -1 ) continue; // avoid -1 value ("default color")
    if (i->second == c) return i->first;
  }
  for (i=custom.begin(); i!=custom.end(); i++){
    if (i->second == c) return i->first;
  }
  cerr << "Unknown fig color: " << c << "\n";
  return -1;
}
// Add new rgb color to custom colormap if needed. Return true if color was added
bool
color_rgbadd(const int c, map<int,int> & custom){
  int maxc=0;
  map<int,int>::const_iterator i;
  for (i=colors.begin(); i!=colors.end(); i++){
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

// function for reading objects from file
bool read(const char* filename, fig_world & world, const Options & opts){

  fig_world ret;

  fig_object o, o0;
  iPoint p;
  vector<string> comment;
  map<int,int> custom_colors;

  int color_num=0;

  string key;
  Options tmp_opts;

  rule_t ch = anychar_p - eol_p;

  rule_t comm = eps_p[clear_a(comment)][clear_a(tmp_opts)] >>
           *( (str_p("# \\") >> (*(ch-'='))[assign_a(key)] >> '=' >>
                (*ch)[insert_at_a(tmp_opts,key)] >> eol_p) |
              (str_p("# \\") >> (*(ch-'='))[assign_a(key)] >>
                eol_p)[insert_at_a(tmp_opts,key,"1")] |
              (str_p("# ") >> (*ch)[push_back_a(comment)] >> eol_p) );

  rule_t header = str_p("#FIG 3.2") >> *ch >> eol_p >>
	(*ch)[assign_a(ret.orientation)] >> eol_p >>       // orientation ("Landscape" or "Portrait")
	(*ch)[assign_a(ret.justification)] >> eol_p >>     // justification ("Center" or "Flush Left")
	(*ch)[assign_a(ret.units)] >> eol_p >>             // units ("Metric" or "Inches")
	(*ch)[assign_a(ret.papersize)] >> eol_p >>         // papersize ("A4", etc.)
	ureal_p[assign_a(ret.magnification)] >> eol_p >>   // magnification (export and print magnification, %)
	(*ch)[assign_a(ret.multiple_page)] >> eol_p >>     // multiple-page ("Single" or "Multiple" pages)
	int_p[assign_a(ret.transparent_color)] >> eol_p >> // transparent color
	comm[assign_a(ret.comment, comment)][assign_a(ret.opts, tmp_opts)] >>  // comments and options
	uint_p[assign_a(ret.resolution)] >> +space_p >>    // resolution
                uint_p[assign_a(ret.coord_system)] >> eol_p;       // coord_system

  /****************************************************************/
  rule_t r_sub_type       = +blank_p >> uint_p[assign_a(o.sub_type)][assign_a(sub_type)];
  rule_t r_line_style     = +blank_p >> int_p[assign_a(o.line_style)];
  rule_t r_thickness      = +blank_p >> int_p[assign_a(o.thickness)];
  rule_t r_pen_color      = +blank_p >> int_p[assign_a(o.pen_color)];
  rule_t r_fill_color     = +blank_p >> int_p[assign_a(o.fill_color)];
  rule_t r_depth          = +blank_p >> uint_p[assign_a(o.depth)];
  rule_t r_pen_style      = +blank_p >> int_p[assign_a(o.pen_style)];
  rule_t r_area_fill      = +blank_p >> int_p[assign_a(o.area_fill)];
  rule_t r_style_val      = +blank_p >> real_p[assign_a(o.style_val)];
  rule_t r_cap_style      = +blank_p >> int_p[assign_a(o.cap_style)];
  rule_t r_join_style     = +blank_p >> int_p[assign_a(o.join_style)];
  rule_t r_direction      = +blank_p >> int_p[assign_a(o.direction)];
  rule_t r_radius         = +blank_p >> int_p[assign_a(o.radius)];
  rule_t r_arrows         = +blank_p >> uint_p[assign_a(f_arr)][assign_a(o.forward_arrow)] >> 
                          +blank_p >> uint_p[assign_a(b_arr)][assign_a(o.backward_arrow)];
  rule_t r_arrows_p = (eps_p(&no_f_arr) | (
                     +space_p >> int_p[assign_a(o.farrow_type)]  
                  >> +blank_p >> int_p[assign_a(o.farrow_style)]   
                  >> +blank_p >> real_p[assign_a(o.farrow_thickness)]
                  >> +blank_p >> real_p[assign_a(o.farrow_width)]
                  >> +blank_p >> real_p[assign_a(o.farrow_height)]
                )) >> (eps_p(&no_b_arr) | (
                     +space_p >> int_p[assign_a(o.barrow_type)]    
                  >> +blank_p >> int_p[assign_a(o.barrow_style)]   
                  >> +blank_p >> real_p[assign_a(o.barrow_thickness)]
                  >> +blank_p >> real_p[assign_a(o.barrow_width)]
                  >> +blank_p >> real_p[assign_a(o.barrow_height)]
                ));
  rule_t r_angle          = +blank_p >> real_p[assign_a(o.angle)];
  rule_t r_center_xy      = +blank_p >> real_p[assign_a(o.center_x)] >>
                            +blank_p >> real_p[assign_a(o.center_y)];
  rule_t r_radius_xy      = +blank_p >> int_p[assign_a(o.radius_x)] >>
                            +blank_p >> int_p[assign_a(o.radius_y)];
  rule_t r_start_xy       = +blank_p >> int_p[assign_a(o.start_x)] >> 
                            +blank_p >> int_p[assign_a(o.start_y)];
  rule_t r_end_xy         = +blank_p >> int_p[assign_a(o.end_x)] >> 
                            +blank_p >> int_p[assign_a(o.end_y)];
  rule_t r_npoints        = +blank_p >> int_p[assign_a(npoints)][assign_a(npoints_f)];
  rule_t r_push_xy        = +space_p >> int_p[assign_a(p.x)] >>
                            +space_p >> int_p[assign_a(p.y)][push_back_a(o,p)];
  rule_t r_image          = (eps_p(&no_picture) | (
                            +space_p >> uint_p[assign_a(o.image_orient)] >> // orientation = normal (0) or flipped (1)
                            +blank_p >> (+ch)[assign_a(o.image_file)]));
  rule_t r_points         = *( eps_p(&point_counter) >> r_push_xy );
  rule_t r_fpoints        = *( eps_p(&point_f_counter) >> +space_p >> real_p[push_back_a(o.f)] );

  /*******************************************/
  rule_t c0_color = ch_p('0')
	>> +blank_p >> uint_p[assign_a(color_num)]
	>> +blank_p >> '#' >> hex_p[insert_at_a(custom_colors,color_num)]
	>> eol_p;
  /*******************************************/
  const int type5=5;
  rule_t c5_arc   = ch_p('5')[assign_a(o.type,type5)]  // Arc
	>> r_sub_type >> r_line_style >> r_thickness
	>> r_pen_color >> r_fill_color >> r_depth
	>> r_pen_style >> r_area_fill >> r_style_val
        >> r_cap_style >> r_direction >> r_arrows >> r_center_xy
        >> r_push_xy >> r_push_xy >> r_push_xy
	>> r_arrows_p >> eol_p;
  /*******************************************/
  const int type6=6;
  rule_t c6_compound_start = ch_p('6')[assign_a(o.type,type6)] // Compound
        >> r_push_xy >> r_push_xy >> eol_p;
  /*******************************************/
  const int typem6=-6;
  rule_t c6_compound_end = str_p("-6")[assign_a(o.type,typem6)] >> eol_p;
  /*******************************************/
  const int type1=1;
  rule_t c1_ellipse = ch_p('1')[assign_a(o.type,type1)] // Ellipse
	>> r_sub_type >> r_line_style >> r_thickness
	>> r_pen_color >> r_fill_color >> r_depth
        >> r_pen_style >> r_area_fill >> r_style_val
        >> r_direction /*(always 1)*/ >> r_angle /*(radians, the angle of the x-axis)*/
	>> r_center_xy >> r_radius_xy 
        >> r_start_xy >> r_end_xy >> eol_p;
  /*******************************************/
  const int type2=2;
  rule_t c2_polyline = ch_p('2')[assign_a(o.type,type2)] // Polyline
	>> r_sub_type >> r_line_style >> r_thickness
	>> r_pen_color >> r_fill_color >> r_depth
	>> r_pen_style >> r_area_fill >> r_style_val 
        >> r_join_style >> r_cap_style >> r_radius
	>> r_arrows >> r_npoints >> r_arrows_p
	>> r_image >> r_points >> eol_p;
  /*******************************************/
  const int type3=3;
  rule_t c3_spline = ch_p('3')[assign_a(o.type,type3)] // Spline
	>> r_sub_type >> r_line_style >> r_thickness
	>> r_pen_color >> r_fill_color
	>> r_depth >> r_pen_style >> r_area_fill
	>> r_style_val >> r_cap_style
	>> r_arrows >> r_npoints >> r_arrows_p
        >> r_points >> r_fpoints >> eol_p;
  /*******************************************/
  const int type4=4;
  rule_t c4_text = ch_p('4')[assign_a(o.type,type4)] // Text
	>> r_sub_type >> r_pen_color >> r_depth
	>> r_pen_style // (not used)
	>> +blank_p >> int_p[assign_a(o.font)]
	>> +blank_p >> real_p[assign_a(o.font_size)]
	>> r_angle
	>> +blank_p >> int_p[assign_a(o.font_flags)]
	>> +blank_p >> real_p[assign_a(o.height)]
	>> +blank_p >> real_p[assign_a(o.length)]
        >> r_push_xy
	>> blank_p  >> (*(~eps_p("\\001") >> anychar_p))[assign_a(o.text)] 
        >> str_p("\\001") >> eol_p;
  /*******************************************/

  rule_t main_rule = header >>
       *( eps_p[assign_a(o,o0)] >>
          comm[assign_a(o.comment, comment)][assign_a(o.opts,tmp_opts)] >>
        ( c0_color | (c1_ellipse | c2_polyline | c3_spline | c4_text | c5_arc |
        c6_compound_start | c6_compound_end) [push_back_a(ret,o)]) );

  if (!parse_file("fig::read", filename, main_rule)) return false;

  // convert \??? chars
  for (fig::fig_world::iterator i=ret.begin(); i!=ret.end(); i++){
    string t;
    for (int n=0;n<i->text.size();n++){
      if ((i->text[n] == '\\')&&(n+3<i->text.size())){
        t+=char((i->text[n+1]-'0')*64 + (i->text[n+2]-'0')*8 + (i->text[n+3]-'0'));
        n+=3;
      }
      else t+=i->text[n];
    }
    i->text=t;
  }

  //convert comments, options and text to UTF-8
  string enc = opts.get("fig_in_enc", default_enc);
  IConv cnv(enc);

  for (fig_world::iterator i=ret.begin(); i!=ret.end(); i++){
    i->text = cnv.to_utf8(i->text);
    for (vector<string>::iterator
          c = i->comment.begin(); c != i->comment.end(); c++){
      *c = cnv.to_utf8(*c);
    }
    tmp_opts.clear();
    for (Options::iterator o=i->opts.begin();o!=i->opts.end();o++)
      tmp_opts.put(cnv.to_utf8(o->first), cnv.to_utf8(o->second));
    i->opts.swap(tmp_opts);
  }
  for (vector<string>::iterator
      c = ret.comment.begin(); c != ret.comment.end(); c++){
    *c = cnv.to_utf8(*c);
  }
  tmp_opts.clear();
  for (Options::iterator o=ret.opts.begin();o!=ret.opts.end();o++)
    tmp_opts.put(cnv.to_utf8(o->first), cnv.to_utf8(o->second));
  ret.opts.swap(tmp_opts);

  // convert colors to rgb
  for (fig::fig_world::iterator i=ret.begin(); i!=ret.end(); i++){
    i->pen_color  = color_fig2rgb(i->pen_color,  custom_colors);
    i->fill_color = color_fig2rgb(i->fill_color, custom_colors);
  }

  // merge world and ret
  fig::fig_world tmp=world;
  world=ret;
  world.insert(world.begin(), tmp.begin(), tmp.end());
  for (Options::iterator o=tmp.opts.begin();o!=tmp.opts.end();o++)
    world.opts.put(o->first, o->second);

  return true;
}

/***********************************************************/
bool write(ostream & out, const fig_world & world, const Options & opts){

  string enc = opts.get("fig_out_enc", default_enc);
  IConv cnv(enc);

  int n;
  // writing header
  out << "#FIG 3.2\n" 
      << world.orientation << "\n"
      << world.justification << "\n"
      << world.units << "\n"
      << world.papersize << "\n"
      << setprecision(2) << fixed 
      << world.magnification << "\n"
      << setprecision(3) 
      << world.multiple_page << "\n"
      << world.transparent_color << "\n";

  // writing comments
  for (0;n<world.comment.size();n++){
    if (n>99) {cerr << "fig comment contains > 100 lines! Cutting...\n"; break;}
    string s = cnv.from_utf8(world.comment[n]);
    if (s.size()>1022){cerr << "fig comment line is > 1022 chars! Cutting...\n"; s.resize(1022);}
    out << "# " << s << "\n";
  }
  // writing options (as comments)
  for (Options::const_iterator o=world.opts.begin();o!=world.opts.end();o++){
    if (n++>99) {cerr << "fig comment contains > 100 lines! Cutting...\n"; break;}
      string s = cnv.from_utf8(o->first) + "=" + cnv.from_utf8(o->second);
    if (s.size()>1022){cerr << "fig comment line is > 1022 chars! Cutting...\n"; s.resize(1022);}
    out << "# \\" << s << "\n";
  }

  out << world.resolution << " " 
      << world.coord_system << "\n";


  // looking for custom colors
  map<int,int> custom_colors;
  for (fig::fig_world::const_iterator i=world.begin(); i!=world.end(); i++){
    color_rgbadd(i->pen_color,  custom_colors);
    color_rgbadd(i->fill_color, custom_colors);
  }
  // writing custom colors
  for (map<int,int>::const_iterator i  = custom_colors.begin();
       i != custom_colors.end(); i++){
      out << "0 " << i->first << " #" 
          << setbase(16) << setw(6) << setfill('0')
          << i->second << setbase(10) << "\n";
  }

  // writing objects
  for (fig_world::const_iterator
       i  = world.begin(); 
       i != world.end(); i++){

    // We don't support color objects!
    // color object with comment or options breaks all!
    if (i->type==0) continue;

    for (n=0;n<i->comment.size();n++){
      if (n>99) {cerr << "fig comment contains > 100 lines! Cutting...\n"; break;}
      string s = cnv.from_utf8(i->comment[n]);
      if (s.size()>1022){cerr << "fig comment line is > 1022 chars! Cutting...\n"; s.resize(1022);}
      out << "# " << s << "\n";
    }
    for (Options::const_iterator o=i->opts.begin();o!=i->opts.end();o++){
      if (n++>99) {cerr << "fig comment contains > 100 lines! Cutting...\n"; break;}
      string s = cnv.from_utf8(o->first) + "=" + cnv.from_utf8(o->second);
      if (s.size()>1022){cerr << "fig comment line is > 1022 chars! Cutting...\n"; s.resize(1022);}
      out << "# \\" << s << "\n";
    }

    int nn = i->size();
    int nn1=nn;
    vector<float> f = i->f;

    int pen_color  = color_rgb2fig(i->pen_color,  custom_colors);
    int fill_color = color_rgb2fig(i->fill_color, custom_colors);

    switch (i->type){
    case 0: // Color
      break;
    case 1: // Ellipse
      out << "1 "
	<< i->sub_type   << " "
        << i->line_style << " "
        << i->thickness  << " "
	<< pen_color     << " "
        << fill_color    << " "
        << i->depth      << " "
	<< i->pen_style  << " " 
        << i->area_fill  << " " 
        << i->style_val  << " "
	<< i->direction  << " "
        << i->angle      << " "
        << int(i->center_x) << " " 
        << int(i->center_y) << " "  
        << i->radius_x   << " " 
        << i->radius_y   << " " 
	<< i->start_x    << " "
	<< i->start_y    << " "
        << i->end_x      << " "
        << i->end_y      << "\n";
        break;
    case 2: // Polyline

      // в замкнутых многоугольниках последняя точка должна совпадать с первой
      if ((i->sub_type > 1) && (nn>0) && ((*i)[nn-1]!=(*i)[0])){
        nn1=nn+1;
      }

      out << "2 "
        << i->sub_type   << " "
        << i->line_style << " "
        << i->thickness  << " "
        << pen_color     << " " 
        << fill_color    << " "
        << i->depth      << " "
        << i->pen_style  << " "
        << i->area_fill  << " "
        << i->style_val  << " "
        << i->join_style << " "
        << i->cap_style  << " "
        << i->radius     << " "
        << i->forward_arrow  << " "
        << i->backward_arrow << " "
        << nn1;
      if (i->forward_arrow) out << "\n\t"
        << i->farrow_type << " "
        << i->farrow_style << " "
        << i->farrow_thickness << " "
        << i->farrow_width << " "
        << i->farrow_height;
      if (i->backward_arrow) out << "\n\t"
        << i->barrow_type << " "
        << i->barrow_style << " "
        << i->barrow_thickness << " "
        << i->barrow_width << " "
        << i->barrow_height;
      if (i->sub_type==5) out << "\n\t"
        << i->image_orient << " "
        << i->image_file;
      for (n=0; n<nn; n++)
        out << ((n%6==0) ? "\n\t":" ")
            << (*i)[n].x << " " << (*i)[n].y;
      if (nn1>nn)  out << " " << (*i)[0].x << " " << (*i)[0].y;
      out << "\n";
      break;
    case 3: // Spline
      if (nn!=f.size()){
        f.resize(nn, 1);
        cerr << "fig::write (spline): different amount of x,y and f values\n";
      }
      if ((i->sub_type%2==1) && (nn<3)){ 
        cerr << "fig::write (spline): closed spline with <3 points\n";
        break;
      }
      if ((i->sub_type%2==0) && (nn<2)){ 
        cerr << "fig::write (spline): spline with <2 points\n";
        break;
      }
      out << "3 "
        << i->sub_type   << " "
        << i->line_style << " "
        << i->thickness  << " "
        << pen_color     << " " 
        << fill_color    << " "
        << i->depth      << " "
        << i->pen_style  << " "
        << i->area_fill  << " "
        << i->style_val  << " " 
        << i->cap_style  << " "
        << i->forward_arrow  << " "
        << i->backward_arrow << " "
        << nn;
        if (i->forward_arrow) out << "\n\t"
           << i->farrow_type << " "
           << i->farrow_style << " "
           << i->farrow_thickness << " "
           << i->farrow_width << " "
           << i->farrow_height;
        if (i->backward_arrow) out << "\n\t"
           << i->barrow_type << " "
           << i->barrow_style << " "
           << i->barrow_thickness << " "
           << i->barrow_width << " "
           << i->barrow_height;
        for (n=0; n<nn; n++)
          out << ((n%6==0) ? "\n\t":" ")
              << (*i)[n].x << " " << (*i)[n].y;
        for (n=0; n<nn; n++)
          out << ((n%6==0) ? "\n\t":" ")
              << f[n];
        out << "\n";
        break;
    case 4: // Text
      // сделать пересчет размеров (height length)!
      if (nn<1){
        cerr << "fig::write (text): can't get x and y values\n";
        break;
      }
      out << "4 "
        << i->sub_type   << " "
        << pen_color     << " " 
        << i->depth      << " "
        << i->pen_style  << " "
        << i->font       << " "
        << i->font_size  << " "
        << i->angle      << " "
        << i->font_flags << " "
        << i->height     << " "
        << i->length     << " "
        << (*i)[0].x     << " "
        << (*i)[0].y     << " "
        << cnv.from_utf8(i->text) << "\\001\n";
        break;
    case 5: // Arc
      if (nn<3){
        cerr << "fig::write (arc): can't get x and y values\n";
        break;
      }
      out << "5 "
        << i->sub_type   << " "
        << i->line_style << " "
        << i->thickness  << " "
        << pen_color     << " "
        << fill_color    << " "
        << i->depth      << " "
        << i->pen_style  << " "
        << i->area_fill  << " " 
        << i->style_val  << " "
        << i->cap_style  << " "
        << i->direction  << " "
        << i->forward_arrow  << " "
        << i->backward_arrow << " "
        << i->center_x   << " "
        << i->center_y   << " "
        << (*i)[0].x     << " "
        << (*i)[0].y     << " "
        << (*i)[1].x     << " "
        << (*i)[1].y     << " "
        << (*i)[2].x     << " "
        << (*i)[2].y;
        if (i->forward_arrow) out << "\n\t"
           << i->farrow_type << " "
           << i->farrow_style << " "
           << i->farrow_thickness << " "
           << i->farrow_width << " "
           << i->farrow_height;
        if (i->backward_arrow) out << "\n\t"
           << i->barrow_type << " "
           << i->barrow_style << " "
           << i->barrow_thickness << " "
           << i->barrow_width << " "
           << i->barrow_height;
        out << "\n";
      break;
    case 6: // Compound begin
      // сделать пересчет размеров!
      int x1,y1,x2,y2;
      if (nn<2){
        cerr << "fig::write (compound): can't get x and y values\n";
        x1=y1=x2=y2=0;
      } else {
        x1 = (*i)[0].x;
        y1 = (*i)[0].y;
        x2 = (*i)[1].x;
        y2 = (*i)[1].y;
      }
      out << "6 " 
          << x1 << " " 
          << y1 << " "
          << x2 << " " 
          << y2 << "\n";
      break;
    case -6: // Compound end
      out << "-6\n"; 
      break;
    default:
      cerr << "fig::write: bad object type!\n";
      break;
    }
  }
  return true;
}

bool write(const string & file, const fig_world & world, const Options & opts){
  ofstream out(file.c_str());
  if (!out) {
    cerr << "Error: can't open fig file \"" << file << "\"\n";
    return false;
  }
  if (!write(out, world, opts)) {
    cerr << "Error: can't write to fig file \"" << file << "\"\n";
    return false;
  }
  return true;
}

} //namespace
