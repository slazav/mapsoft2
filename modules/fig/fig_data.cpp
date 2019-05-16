#include "fig_data.h"
#include <map>
#include <string>

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


void FigObj::set_points(const dLine & v){
  clear();
  for (int i=0;i<v.size();i++)
    push_back(iPoint(lround(v[i].x), lround(v[i].y)));
}

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


