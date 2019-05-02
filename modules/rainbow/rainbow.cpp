#include "rainbow.h"
#include <cmath>
#include <cstring>

#include <iostream>

int
color_shade(int c, double k){
  unsigned char r=(c>>16)&0xff, g=(c>>8)&0xff,  b=c&0xff;
  r=rint(r*k); g=rint(g*k); b=rint(b*k);
  return (r << 16) + (g << 8) + b;
}

/********************************************************************/
simple_rainbow::simple_rainbow(double min, double max, rainbow_type type){
  switch (type){
    case RAINBOW_NORMAL:   set_color_string(min, max, "BCGYRM"); break;
    case RAINBOW_BURNING:  set_color_string(min, max, "WYRMBb"); break;
    case RAINBOW_BURNING1: set_color_string(min, max, "KRYW"); break;
  }
}

simple_rainbow::simple_rainbow(double min, double max, int cmin, int cmax){
  RD=std::vector<rainbow_data>(2);
  RD[0].c = cmin; RD[0].v = min;
  RD[1].c = cmax; RD[1].v = max;
}

simple_rainbow::simple_rainbow(double min, double max, const char *colors){
  set_color_string(min, max, colors);}

simple_rainbow::simple_rainbow(const std::vector<rainbow_data> & RD_):RD(RD_){
}

/********************************************************************/

void
simple_rainbow::set_color_string(double min, double max, const char *colors){
  RD.resize(0);
  for (int i=0; i<strlen(colors); i++) {
    switch (colors[i]){
      case 'R': RD.push_back({0, 0xff0000}); break;
      case 'G': RD.push_back({0, 0x00ff00}); break;
      case 'B': RD.push_back({0, 0x0000ff}); break;
      case 'C': RD.push_back({0, 0x00ffff}); break;
      case 'M': RD.push_back({0, 0xff00ff}); break;
      case 'Y': RD.push_back({0, 0xffff00}); break;
      case 'W': RD.push_back({0, 0xffffff}); break;
      case 'K': RD.push_back({0, 0x000000}); break;
      case 'r': RD.push_back({0, 0x400000}); break;
      case 'g': RD.push_back({0, 0x004000}); break;
      case 'b': RD.push_back({0, 0x000040}); break;
      case 'c': RD.push_back({0, 0x004040}); break;
      case 'm': RD.push_back({0, 0x400040}); break;
      case 'y': RD.push_back({0, 0x404000}); break;
      case 'w': RD.push_back({0, 0x404040}); break;
    }
  }
  set_range(min, max);
}

void
simple_rainbow::set_range(double min, double max){
  for (int i=0; i<RD.size(); i++)
    RD[i].v = min + (max-min)/double(RD.size()-1)*i;
}

/********************************************************************/

double
simple_rainbow::get_min() const{
  return RD[0].v;
}

double
simple_rainbow::get_max() const{
  return RD[RD.size()-1].v;
}

int
simple_rainbow::get_size() const{
  return RD.size();
}


int
simple_rainbow::get(double val, int low_c, int high_c) const{
  int N = RD.size();
  if (N<1) return 0;

  int dir = RD[N-1].v-RD[0].v > 0 ? 1:-1;

  if ((low_c  > 0) && (val < (dir?RD[0].v:RD[N-1].v))) return low_c;
  if ((high_c > 0) && (val > (dir?RD[N-1].v:RD[0].v))) return high_c;

  if (dir*(val - RD[0].v)<=0) return RD[0].c;

  int i = 1;
  while (i<N && (dir*(val - RD[i].v) > 0)) i++;
  if (i==N) return RD[N-1].c;

//std::cerr << "> " << i << "/" << N << " " << val << " " << RD[i].v << "\n";

  int r  = (RD[i].c >> 16) & 0xFF;
  int g  = (RD[i].c >> 8) & 0xFF;
  int b  = RD[i].c & 0xFF;

  int rp = (RD[i-1].c >> 16) & 0xFF;
  int gp = (RD[i-1].c >> 8) & 0xFF;
  int bp = RD[i-1].c & 0xFF;

  double k = (val - RD[i-1].v) / (RD[i].v-RD[i-1].v);
  r =  rp + rint((r - rp)*k);
  g =  gp + rint((g - gp)*k);
  b =  bp + rint((b - bp)*k);

  return (r << 16) + (g << 8) + b;
}

