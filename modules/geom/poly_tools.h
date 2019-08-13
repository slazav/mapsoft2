#ifndef POLY_TOOLS_H
#define POLY_TOOLS_H

#include "multiline.h"
#include <algorithm>

/* Extra polygon-related functions */

/// Class for checking if a point is inside a polygon.
/// Line is always treated as closed.
template <typename T>
class PolyTester{
  std::vector<Point<T> > sb,se;
  std::vector<double> ss;
  bool horiz;
public:
  PolyTester(const Line<T> & l, bool horiz_ = true): horiz(horiz_){
    // collect line sides info: start and end points, slopes.
    int pts = l.size();
    for (int i = 0; i < pts; i++){
      Point<T> b(l[i%pts].x, l[i%pts].y),
               e(l[(i+1)%pts].x, l[(i+1)%pts].y);
      if (!horiz){ // swap x and y
        b.y = l[i%pts].x; b.x=l[i%pts].y;
        e.y = l[(i+1)%pts].x;
        e.x = l[(i+1)%pts].y;
      }
      if (b.y == e.y) continue; // no need for horisontal sides
      double s = double(e.x-b.x)/double(e.y-b.y); // side slope
      sb.push_back(b);
      se.push_back(e);
      ss.push_back(s);
    }
  }

  /// get sorted coordinates of the polygon crossings with
  /// y=const (or x=const if horiz=false) line
  std::vector<T> get_cr(T y){
    std::vector<T> cr;
    for (int k = 0; k < sb.size(); k++){
      if ((sb[k].y >  y)&&(se[k].y >  y)) continue; // side is above the row
      if ((sb[k].y <= y)&&(se[k].y <= y)) continue; // side is below the row
      cr.push_back((ss[k] * double(y - sb[k].y)) + sb[k].x);
    }
    sort(cr.begin(), cr.end());
    return cr;
  }

  /// Is coord x inside the polygon at cr line
  bool test_cr(const std::vector<T> & cr, T x) const{
    // number of crossings on the ray (x,y) - (inf,y)
    typename std::vector<T>::const_iterator i =
             lower_bound(cr.begin(), cr.end(), x);
    int k=0;
    while (i!=cr.end()) {i++; k++;}
    return k%2==1;
  }

};
typedef PolyTester<double> dPolyTester;
typedef PolyTester<int>    iPolyTester;


/// Check if one-segment polygon L covers point P.
template <typename T>
bool
point_in_polygon(const Point<T> & P, const Line<T> & L){
  PolyTester<T> lt(L);
  std::vector<T> cr = lt.get_cr(P.y);
  return lt.test_cr(cr, P.x);
}

/// Check if one-segment polygon l covers (maybe partially) rectangle r.
template <typename T>
bool
rect_in_polygon(const Rect<T> & R, const Line<T> & L){
  PolyTester<T> lth(L, true), ltv(L,false);
  // check if there is any crossing at any sides,
  // or one corner is inside polygon
  // or one of polygon points is inside the rectangle
  std::vector<T> cr;
  cr = lth.get_cr(R.y);
  for (int i=0; i<cr.size(); i++)
    if (R.x < cr[i] && cr[i] <=R.x+R.w ) return true;
  cr = lth.get_cr(R.y+R.h);
  for (int i=0; i<cr.size(); i++)
    if (R.x < cr[i] && cr[i] <=R.x+R.w ) return true;
  cr = ltv.get_cr(R.x);
  for (int i=0; i<cr.size(); i++)
    if (R.y < cr[i] && cr[i] <=R.y+R.h ) return true;
  cr = ltv.get_cr(R.x+R.w);
  for (int i=0; i<cr.size(); i++)
    if (R.y < cr[i] && cr[i] <=R.y+R.h ) return true;

  for (int i=0; i<cr.size(); i++){
    if (R.y < cr[i] ){
      if (i%2 == 1) return true;
      break;
    }
  }
  if (L.size() && R.contains(L[0])) return true;
  return false;
}

// Join a multi-segment polygon into a single-segment one
// using shortest cuts.
template<typename T>
Line<T> join_polygons(const MultiLine<T> & L){

  Line<T> ret;

  typename MultiLine<T>::const_iterator l = L.begin();
  ret = *l; l++;
  while (l!=L.end()){

    // Find place for the shortest cut between ret vertex
    // and vertex of the next segment

    double dst = INFINITY;

    typename Line<T>::const_iterator i1,i2; // vertex iterators
    typename Line<T>::const_iterator q1,q2; // result

    for (i1=ret.begin(); i1!=ret.end(); i1++){
      for (i2=l->begin(); i2!=l->end(); i2++){
        double d = dist(*i1, *i2);
        if (d < dst){ dst = d; q1=i1; q2=i2; }
      }
    }

    // insert new segment
    Line<T> tmp;
    tmp.push_back(*q1);
    tmp.insert(tmp.end(), q2, l->end());
    tmp.insert(tmp.end(), l->begin(), q2);
    tmp.push_back(*q2);
    ret.insert(q1, tmp.begin(), tmp.end());
    l++;
  }
  return ret;
}

// Remove holes in a multi-segment polygon
// using shortest cuts.
template<typename T>
void remove_holes(MultiLine<T> & L){
  typename MultiLine<T>::iterator i1,i2;
  for (i1=L.begin(); i1!=L.end(); i1++){
    i2=i1+1;
    while (i2!=L.end()){
      if ( !i2->size() || !point_in_polygon((*i2)[0], *i1)){
        i2++; continue; }
      // join i2 -> i1

      // Find place for the shortest cut between ret vertex
      // and vertex of the next segment
      double dst = 1e99;
      typename Line<T>::iterator p1,q1;
      typename Line<T>::iterator p2,q2;
        // p1,p2 -- пара вершин
        // q1,q2 -- искомое
      for (p1=i1->begin(); p1!=i1->end(); p1++){
        for (p2=i2->begin(); p2!=i2->end(); p2++){
          double d = dist(*p1, *p2);
          if (d < dst){ dst = d; q1=p1; q2=p2; }
        }
      }
      // insert new segment
      Line<T> tmp;
      tmp.push_back(*q1);
      tmp.insert(tmp.end(), q2, i2->end());
      tmp.insert(tmp.end(), i2->begin(), q2);
      tmp.push_back(*q2);
      (*i1).insert(q1, tmp.begin(), tmp.end());
      i2=L.erase(i2);
    }
  }
}

#endif
