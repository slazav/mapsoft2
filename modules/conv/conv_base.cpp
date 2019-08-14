#include "conv_base.h"

dLine
ConvBase::frw_acc(const dLine & l, double acc) const {
  dLine ret;
  if (l.size()==0) return ret;
  dPoint P1 = l[0], P1a =P1;
  frw(P1a); ret.push_back(P1a); // add first point
  dPoint P2, P2a;

  // for all line segments:
  for (int i=1; i<l.size(); i++){
    // start with a whole segment
    P1 = l[i-1];
    P2 = l[i];
    if (P2==P1) continue;
    do {
      // convert first and last point
      P1a = P1; frw(P1a);
      P2a = P2; frw(P2a);
      // C1 - is a center of (P1-P2)
      // C2-C1 is a perpendicular to (P1-P2) with acc length
      dPoint C1 = (P1+P2)/2.0;
      dPoint C2x = C1 + dPoint(acc,0,0);
      dPoint C2y = C1 + dPoint(0,acc,0);
      dPoint C2z = C1 + dPoint(0,0,acc);

      dPoint C1a(C1); frw(C1a);
      frw(C2x); frw(C2y); frw(C2z);

      // accuracy in destination units:
      double acc_dst = std::max(std::max(dist(C1a,C2x),dist(C1a,C2y)),dist(C1a,C2z));

      if ((dist(C1a, (P1a+P2a)/2.) < acc_dst) ||
          (dist(P1,P2) < acc)){
        // go to the rest of line (P2-l[i])
        ret.push_back(P2a);
        P1 = P2;
        P2 = l[i];
      }
      else {
        // go to the first half (P1-C1) of current line
        P2 = C1;
      }
    } while (P1!=P2);
  }
  return ret;
}

dLine
ConvBase::bck_acc(const dLine & l, double acc) const {
  // Note that bck_acc and frw_acc are not symmetric
  // because accuracy is always calculated on the src side.

  dLine ret;
  if (l.size()==0) return ret;
  dPoint P1 = l[0], P1a =P1;
  bck(P1a); ret.push_back(P1a); // add first point
  dPoint P2, P2a;

  for (int i=1; i<l.size(); i++){
    // start with a whole segment
    P1 = l[i-1];
    P2 = l[i];
    if (P2==P1) continue;
    do {
      // convert first and last point
      P1a = P1; bck(P1a);
      P2a = P2; bck(P2a);
      // convert central point
      dPoint C1a = (P1+P2)/2.0;
      bck(C1a);

      if ((dist(C1a, (P1a+P2a)/2.0) < acc) ||
          (dist(P1,P2) < acc)){

        ret.push_back(P2a);
        P1 = P2;
        P2 = l[i];
      }
      else {
        // go to the first half of current line
        P2 = (P1+P2)/2.0;
      }
    } while (P1!=P2);
  }
  return ret;
}

dMultiLine
ConvBase::frw_acc(const dMultiLine & ml, double acc) const{
  dMultiLine ret;
  for (auto const &l:ml) ret.push_back(frw_acc(l,acc));
  return ret;
}


dMultiLine
ConvBase::bck_acc(const dMultiLine & ml, double acc) const{
  dMultiLine ret;
  for (auto const &l:ml) ret.push_back(bck_acc(l,acc));
  return ret;
}


/*

double
ConvBase::ang_frw(dPoint p, double a, double dx) const{
  dPoint p1 = p + dPoint(dx*cos(a), dx*sin(a));
  dPoint p2 = p - dPoint(dx*cos(a), dx*sin(a));
 frw(p1); frw(p2);
  p1-=p2;
  return atan2(p1.y, p1.x);
}

double
ConvBase::ang_bck(dPoint p, double a, double dx) const{
  dPoint p1 = p + dPoint(dx*cos(a), dx*sin(a));
  dPoint p2 = p - dPoint(dx*cos(a), dx*sin(a));
  bck(p1); bck(p2);
  p1-=p2;
  return atan2(p1.y, p1.x);
}

double
ConvBase::angd_frw(dPoint p, double a, double dx) const{
  return 180.0/M_PI * ang_frw(p, M_PI/180.0*a, dx);
}

double
ConvBase::angd_bck(dPoint p, double a, double dx) const{
  return 180.0/M_PI * ang_bck(p, M_PI/180.0*a, dx);
}

dPoint
ConvBase::units_frw(dPoint p) const{
  dPoint p1 = p + dPoint(1,0);
  dPoint p2 = p + dPoint(0,1);
  frw(p), frw(p1), frw(p2);
  return dPoint(dist(p1,p), dist(p2,p));
}

dPoint
ConvBase::units_bck(dPoint p) const{
  dPoint p1 = p + dPoint(1,0);
  dPoint p2 = p + dPoint(0,1);
  bck(p), bck(p1), bck(p2);
  return dPoint(dist(p1,p), dist(p2,p));
}

int
ConvBase::image_frw(const iImage & src_img, iImage & dst_img,
                const iPoint & shift, const double scale) const{
  for (int y=0; y<dst_img.h; y++){
    for (int x=0; x<dst_img.w; x++){
      dPoint p(x,y); p+=shift; bck(p); p*=scale;
      int c = src_img.safe_get(int(p.x),int(p.y));
      dst_img.set_a(x,y,c);
    }
  }
}

int
ConvBase::image_bck(const iImage & src_img, iImage & dst_img,
                const iPoint & shift, const double scale) const{
  for (int y=0; y<dst_img.h; y++){
    for (int x=0; x<dst_img.w; x++){
      dPoint p(x,y); p+=shift; frw(p); p*=scale;
      int c = src_img.safe_get(int(p.x),int(p.y));
      dst_img.set_a(x,y,c);
    }
  }
}

*/
