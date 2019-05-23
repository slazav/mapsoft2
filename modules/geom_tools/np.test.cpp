#include <cassert>
#include <fstream>
#include <iostream>
#include "fig/fig.h"
#include "line_utils.h"

using namespace std;

int
main(int argc, char **argv){
  try {

  if (argc!=3) throw Err() << "usage: np.test <in_file.fig> <out_file.fig>";
  const char *in_file  = argv[1];
  const char *out_file = argv[2];

  Fig F;
  ifstream in(in_file);
  read_fig(in, F);

  int w = int(10.0*Fig::cm2fig);
  iRect cutter(0,0,w,w);

  // push all fig objects into single multiline
  iMultiLine ml;
  Fig::const_iterator f;
  for (f=F.begin(); f!=F.end(); f++) ml.push_back(*f);

  FigObj o;
  read_figobj_header(o, "2 1 0 1 4 7 20 -1 -1 0.000 1 0 0 0 0 0");
  o.pen_color = 0xFF0000;
  o.fill_color = 0xFFFFFF;

  // use points on greed 10x10 with 1cm step
  for (int i=0; i<10; i++){
    for (int j=0; j<10; j++){
      iPoint p(i,j); p*=Fig::cm2fig;
      dPoint dp(p);

      o.clear();
      o.push_back(p);

      dPoint v;
      nearest_pt(ml, v, p, Fig::cm2fig);

      o.push_back(p);
      F.push_back(o);
    }
  }

  ofstream out(out_file);
  write_fig(out,F);

  return 0;
  }
  catch (Err e){
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}


