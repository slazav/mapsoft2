#include <map>
#include <string>
#include <iomanip>

#include "iconv/iconv.h"
#include "err/err.h"

#include "fig.h"

using namespace std;

/*************************************************/
/// static data

// fig units, 1.05/1200in (not 1/1200in, I don't know why...)
const double Fig::cm2fig = 1200.0 / 1.05 / 2.54;
const double Fig::fig2cm = 1.0 / cm2fig;

// fig colors
typedef pair<int,int> p_ii;
const p_ii colors_a[] = {
  p_ii(-1, 0x000000), // default
  p_ii(0,  0x000000), // black
  p_ii(1,  0x0000ff), // blue
  p_ii(2,  0x00ff00), // green
  p_ii(3,  0x00ffff), // cyan
  p_ii(4,  0xff0000), // red
  p_ii(5,  0xff00ff), // magenta
  p_ii(6,  0xffff00), // yellow
  p_ii(7,  0xffffff), // white
  p_ii(8,  0x000090), // blue4
  p_ii(9,  0x0000b0), // blue3
  p_ii(10, 0x0000d0), // blue2
  p_ii(11, 0x87ceff), // ltblue
  p_ii(12, 0x009000), // green4
  p_ii(13, 0x00b000), // green3
  p_ii(14, 0x00d000), // green2
  p_ii(15, 0x009090), // cyan4
  p_ii(16, 0x00b0b0), // cyan3
  p_ii(17, 0x00d0d0), // cyan2
  p_ii(18, 0x900000), // red4
  p_ii(19, 0xb00000), // red3
  p_ii(20, 0xd00000), // red2
  p_ii(21, 0x900090), // magenta4
  p_ii(22, 0xb000b0), // magenta3
  p_ii(23, 0xd000d0), // magenta2
  p_ii(24, 0x803000), // brown4
  p_ii(25, 0xa04000), // brown3
  p_ii(26, 0xc06000), // brown2
  p_ii(27, 0xff8080), // pink4
  p_ii(28, 0xffa0a0), // pink3
  p_ii(29, 0xffc0c0), // pink2
  p_ii(30, 0xffe0e0), // pink
  p_ii(31, 0xffd700)  // gold
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


