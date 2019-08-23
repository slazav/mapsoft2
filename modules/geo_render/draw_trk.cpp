#include <vector>
#include <cmath>

#include "draw_trk.h"
#include "geom/line.h"
#include "rainbow/rainbow.h"

using namespace std;

void
draw_trk(CairoWrapper & cr, const iPoint & origin,
         const ConvBase & cnv, const GeoTrk & trk,
         const Opt & opt){

  cr->save();
  cr->cap_round();

  double w  = trk.opts.get<double>("thickness", 1);
  bool   closed = trk.opts.get<double>("closed", false);
  int    color  = trk.opts.get<int>("color", 0xFF0000);

  int arr_w = w * 2.0;
  int dot_w = w * 0.5;
  int arr_dist = w * 10; // minimal segment with arrow

  string trk_draw_mode = opt.get<string>("trk_draw_mode", "normal");

  if (trk_draw_mode == "normal"){
    color            = opt.get("trk_draw_color", color);
    bool draw_dots   = opt.get("trk_draw_dots", 0);
    bool draw_arrows = opt.get("trk_draw_arrows", 0);
    cr->set_color(color);
    cr->set_line_width(w);

    if (trk.size()==1){ // draw 1pt tracks even in non-draw_dots mode
      GeoTpt p1(trk[0]);
      cnv.bck(p1);  p1-=origin;
      cr->move_to(p1);
      cr->circle(p1, dot_w);
    }

    for (int i=0; i<trk.size(); i++){
      GeoTpt p1,p2;
      p2 = trk[i];
      if (i==0){
        if (closed) p1=trk[trk.size()-1];
        else continue;
      }
      else p1=trk[i-1];

      if (p2.start) continue;

      cnv.bck(p1);  p1-=origin;
      cnv.bck(p2);  p2-=origin;

      cr->move_to(p1);
      cr->line_to(p2);

      if (draw_dots){
        if ((i==1) || p1.start) cr->circle(p1, dot_w);
        cr->circle(p2, dot_w);
      }

      if (draw_arrows && (len2d(p1-p2) > arr_dist)){
        dPoint p0=(p1+p2)/2;
        dPoint dp = norm2d(p1-p2) * arr_w;
        dPoint p3 = p0 + dp + dPoint(dp.y, -dp.x) * 0.5;
        dPoint p4 = p0 + dp - dPoint(dp.y, -dp.x) * 0.5;
        cr->move_to(p0);
        cr->line_to(p3);
        cr->line_to(p4);
        cr->line_to(p0);
      }

    }
    cr->stroke();
  }

  if (trk_draw_mode == "speed"){
    cr->set_line_width(2*w);
    Rainbow RB(opt.get<double>("trk_draw_min", 0),
               opt.get<double>("trk_draw_max", 10),
               opt.get<string>("trk_draw_grad", "BCGYRM").c_str());


    if (trk.size()==1){ // draw 1pt tracks even in non-draw_dots mode
      GeoTpt p1(trk[0]);
      cnv.bck(p1);  p1-=origin;
      cr->set_color(0);
      cr->move_to(p1);
      cr->circle(p1, dot_w);
    }

    for (int i=0; i<trk.size(); i++){
      GeoTpt p1,p2;
      p2 = trk[i];
      if (i==0){
        if (closed) p1=trk[trk.size()-1];
        else continue;
      }
      else p1=trk[i-1];

      if (p2.start) continue;

      double cc = cos((p1.y + p2.y)/2.0/180.0*M_PI);
      double dx = (p2.x - p1.x)*cc;
      double dy = (p2.y - p1.y);
      double d = sqrt(dx*dx + dy*dy) * 6380e3 * M_PI/180;

      cnv.bck(p1);  p1-=origin;
      cnv.bck(p2);  p2-=origin;

      double t = p2.t - p1.t; // t in ms
      if (t>0 && t<3600*24) cr->set_color(RB.get(d/t * 3600));
      else cr->set_color(0);

      cr->move_to(p1);
      cr->line_to(p2);
      cr->stroke();
    }
  }

  if (trk_draw_mode == "height"){
    cr->set_line_width(2*w);
    Rainbow RB(opt.get<double>("trk_draw_min", -200),
               opt.get<double>("trk_draw_max", 8000),
               opt.get<string>("trk_draw_grad", "BCGYRM").c_str());

    if (trk.size()==1){ // draw 1pt tracks even in non-draw_dots mode
      GeoTpt p1(trk[0]);
      cnv.bck(p1);  p1-=origin;
      cr->set_color(0);
      cr->move_to(p1);
      cr->circle(p1, dot_w);
    }

    for (int i=0; i<trk.size(); i++){
      GeoTpt p1,p2;
      p2 = trk[i];
      if (i==0){
        if (closed) p1=trk[trk.size()-1];
        else continue;
      }
      else p1=trk[i-1];

      if (p2.start) continue;

      cnv.bck(p1);  p1-=origin;
      cnv.bck(p2);  p2-=origin;

      if (!p1.have_alt() || !p2.have_alt()){
        cr->set_color(0);
        cr->move_to(p1);
        cr->line_to(p2);
        cr->stroke();
      }
      else {
        int n = ceil(len2d(p2-p1)/2/w);
        dPoint dp = (p2-p1)/n;

        for(int j=0; j< n; j++){
          cr->move_to(p1 + 1.0*j*dp);
          cr->line_to(p1 + 1.0*(j+1)*dp);
          cr->set_color(RB.get( p1.z + double(j*(p2.z - p1.z))/n ));
          cr->stroke();
        }
      }
    }
  }
  cr->restore();
}

