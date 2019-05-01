#include "rainbow.h"
#include <cmath>

int
get_rainbow(double val, const std::vector<rainbow_data> & RD,
             int low_c, int high_c){
  int r=0, g=0, b=0;
  int N = RD.size();
  if (N<1) return 0;
  int dir = RD[N-1].v-RD[0].v > 0 ? 1:-1;

  if ((low_c  > 0) && (val < (dir?RD[0].v:RD[N-1].v))) return low_c;
  if ((high_c > 0) && (val > (dir?RD[N-1].v:RD[0].v))) return high_c;

  for (int i=0; i<N; i++){

    r  = (RD[i].c >> 16) & 0xFF;
    g  = (RD[i].c >> 8) & 0xFF;
    b  = RD[i].c & 0xFF;

    int ip = (i==0)? i : i-1;
    if (dir*(val - RD[ip].v) <= 0) break;

    if (dir*(val - RD[i].v) <= 0){
      int rp = (RD[ip].c >> 16) & 0xFF;
      int gp = (RD[ip].c >> 8) & 0xFF;
      int bp = RD[ip].c & 0xFF;

      r =  rp + rint((r - rp) * (val - RD[ip].v) / (RD[i].v-RD[ip].v));
      g =  gp + rint((g - gp) * (val - RD[ip].v) / (RD[i].v-RD[ip].v));
      b =  bp + rint((b - bp) * (val - RD[ip].v) / (RD[i].v-RD[ip].v));

      break;
    }
  }
  return (r << 16) + (g << 8) + b;
}

int
color_shade(int c, double k){
  unsigned char r=(c>>16)&0xff, g=(c>>8)&0xff,  b=c&0xff;
  r=rint(r*k); g=rint(g*k); b=rint(b*k);
  return (r << 16) + (g << 8) + b;
}

simple_rainbow::simple_rainbow(double min, double max, rainbow_type type){
  switch (type){
    case RAINBOW_NORMAL:
    RD=std::vector<rainbow_data>(6);
    RD[0].c = 0xff0000;
    RD[1].c = 0xffff00;
    RD[2].c = 0x00ff00;
    RD[3].c = 0x00ffff;
    RD[4].c = 0x0000ff;
    RD[5].c = 0xff00ff;
    break;
    case RAINBOW_BURNING:
    RD=std::vector<rainbow_data>(6);
    RD[0].c = 0xffffff;
    RD[1].c = 0xffff00;
    RD[2].c = 0xff0000;
    RD[3].c = 0xff00ff;
    RD[4].c = 0x0000ff;
    RD[5].c = 0x000040;
    break;
    case RAINBOW_BURNING1:
    RD=std::vector<rainbow_data>(4);
    RD[0].c = 0x000000;
    RD[1].c = 0xff0000;
    RD[2].c = 0xffff00;
    RD[3].c = 0xffffff;
    break;
  }
  set_range(min,max);
}

simple_rainbow::simple_rainbow(double min, double max, int cmin, int cmax){
  RD=std::vector<rainbow_data>(2);
  RD[0].c = cmin; RD[0].v = min;
  RD[1].c = cmax; RD[1].v = max;
}

void
simple_rainbow::set_range(double min, double max){
  for (int i=0; i<RD.size(); i++)
    RD[i].v = min + (max-min)/double(RD.size()-1)*i;
}

double
simple_rainbow::get_min() const{
  return RD[0].v;
}

double
simple_rainbow::get_max() const{
  return RD[RD.size()-1].v;
}


int
simple_rainbow::get(double val, int low_c, int high_c) const{
  return get_rainbow(val, RD, low_c, high_c);
}

const rainbow_data *
simple_rainbow::get_data() const {
  return RD.data();
}

int
simple_rainbow::get_size() const{
  return RD.size();
}


