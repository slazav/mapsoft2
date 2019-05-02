#include "rainbow.h"
#include <cmath>
#include <cstring>

int
color_shade(int c, double k){
  unsigned char r=(c>>16)&0xff, g=(c>>8)&0xff,  b=c&0xff;
  r=rint(r*k); g=rint(g*k); b=rint(b*k);
  return (r << 16) + (g << 8) + b;
}

/********************************************************************/
Rainbow::Rainbow(const std::vector<rainbow_data> & RD_): RD(RD_){
  update_data();
}

Rainbow::Rainbow(double min, double max, const char *colors){
  set_color_string(min, max, colors);
}

Rainbow::Rainbow(double min, double max, rainbow_type type){
  switch (type){
    case RAINBOW_NORMAL:   set_color_string(min, max, "BCGYRM"); break;
    case RAINBOW_BURNING:  set_color_string(min, max, "WYRMBb"); break;
    case RAINBOW_BURNING1: set_color_string(min, max, "KRYW"); break;
  }
}

Rainbow::Rainbow(double min, double max, int cmin, int cmax){
  RD=std::vector<rainbow_data>(2);
  RD[0].c = cmin; RD[0].v = min;
  RD[1].c = cmax; RD[1].v = max;
  update_data();
}

/********************************************************************/

void
Rainbow::update_data(){
  N = RD.size();
  vv.resize(N);
  cc.resize(N);
  rr.resize(N);
  gg.resize(N);
  bb.resize(N);
  for (int i=0; i<N; i++){
    vv[i] = RD[i].v;
    cc[i] = RD[i].c;
    rr[i]  = (cc[i] >> 16) & 0xFF;
    gg[i]  = (cc[i] >> 8) & 0xFF;
    bb[i]  = cc[i] & 0xFF;
  }
  if (N>0) {
    dir = (vv[N-1]-vv[0]>0) ? 1:-1;
  }
}

void
Rainbow::set_color_string(double min, double max, const char *colors){
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
  update_data();
}

void
Rainbow::set_range(double min, double max){
  for (int i=0; i<RD.size(); i++)
    RD[i].v = min + (max-min)/double(RD.size()-1)*i;
}

/********************************************************************/

double
Rainbow::get_min() const{
  return RD[0].v;
}

double
Rainbow::get_max() const{
  return RD[RD.size()-1].v;
}

int
Rainbow::get_size() const{
  return RD.size();
}


int
Rainbow::get(double val, int low_c, int high_c) const{
  if (N<1) return 0;

  if ((low_c  > 0) && (val < (dir>0?vv[0]:vv[N-1]))) return low_c;
  if ((high_c > 0) && (val > (dir>0?vv[N-1]:vv[0]))) return high_c;

  if (dir*(val - vv[0])<=0) return cc[0];

  int i = 1;
  while (i<N && (dir*(val - vv[i]) > 0)) i++;
  if (i==N) return cc[N-1];

  double k = (val - vv[i-1]) / (vv[i]-vv[i-1]);
  int r =  rr[i-1] + rint((rr[i] - rr[i-1])*k);
  int g =  gg[i-1] + rint((gg[i] - gg[i-1])*k);
  int b =  bb[i-1] + rint((bb[i] - bb[i-1])*k);
  return (r << 16) + (g << 8) + b;
}

