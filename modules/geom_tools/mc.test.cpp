#include <string>
#include <fstream>
#include <iostream>
#include "fig/fig.h"
#include "line_utils.h"

using namespace std;

int
main(int argc, char **argv){
  try{

  if (argc!=3) throw Err() << "usage: mc.test <in_file.fig> <out_file.fig>";

  const char *in_file  = argv[1];
  const char *out_file = argv[2];

  Fig F;
  ifstream in(in_file);
  read_fig(in, F);

  iLine l1;
  iLine l2;
  Fig::const_iterator f=F.begin();
  if (f!=F.end()) l1=*f;
  f++;
  if (f!=F.end()) l2=*f;

  dPoint p0;
  dPoint t;
  double m=margin_classifier(l1,l2,p0,t);

  // test that dLine и iLine give same results
  dPoint p0d, td;
  double md=margin_classifier(dLine(l1),dLine(l2),p0d,td);
  if ((md!=m) || (td!=t) || (p0d.rint()!=p0.rint())){
    std::cerr << "different results for int and double lines!\n";
    std::cerr << md << " " << m << " " << td << " " << t << " " << p0d << " " <<p0 << "\n";
    return 1;
  }

  dPoint n(t.y, -t.x);
  iPoint tx(t*Fig::cm2fig*5);
  iPoint nx(n*m/2);

  FigObj o;
  read_figobj_header(o, "2 1 0 1 4 7 20 -1 -1 0.000 1 0 0 0 0 0");
  o.pen_color = 0xFF0000;
  o.fill_color = 0xFFFFFF;

  o.push_back(p0 + tx);
  o.push_back(p0 - tx);
  F.push_back(o); o.clear();

  o.push_back(p0 - nx + tx);
  o.push_back(p0 - nx - tx);
  F.push_back(o); o.clear();

  o.push_back(p0 + nx + tx);
  o.push_back(p0 + nx - tx);
  F.push_back(o); o.clear();

  ofstream out(out_file);
  write_fig(out,F);
  return 0;

  }
  catch (Err e){
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}


