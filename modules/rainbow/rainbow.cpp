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
  update_data();
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
  lc=0;
  hc=0;
  for (int i=0; i<N; i++){
    vv[i] = RD[i].v;
    cc[i] = RD[i].c;
    rr[i]  = (cc[i] >> 16) & 0xFF;
    gg[i]  = (cc[i] >> 8) & 0xFF;
    bb[i]  = cc[i] & 0xFF;
  }
  if (N>0) {
    dir = (vv[N-1]-vv[0]>0) ? 1:-1;
    lv = dir>0 ? vv[0] : vv[N-1];
    hv = dir>0 ? vv[N-1] : vv[0];
    lc = dir>0 ? cc[0] : cc[N-1];
    hc = dir>0 ? cc[N-1] : cc[0];
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
}

void
Rainbow::set_range(double min, double max){
  for (int i=0; i<RD.size(); i++)
    RD[i].v = min + (max-min)/double(RD.size()-1)*i;
}

/********************************************************************/

void
Rainbow::set_limits(int low_c, int high_c){
  lc = low_c; hc = high_c;
  if (lc==-1) lc = dir>0 ? cc[0] : cc[N-1];
  if (hc==-1) hc = dir>0 ? cc[N-1] : cc[0];
}

int
Rainbow::get(double val) const{
  if (N<1) return 0;
  if (val < lv) return lc;
  if (val > hv) return hc;

  int i = 1;
  while (i<N-1 && (dir*(val - vv[i]) > 0)) i++;

  double k = (val - vv[i-1]) / (vv[i]-vv[i-1]);
  int r =  rr[i-1] + rint((rr[i] - rr[i-1])*k);
  int g =  gg[i-1] + rint((gg[i] - gg[i-1])*k);
  int b =  bb[i-1] + rint((bb[i] - bb[i-1])*k);
  return (r << 16) + (g << 8) + b;
}

