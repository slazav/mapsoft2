#ifndef POLY_TOOLS_H
#define POLY_TOOLS_H

#include "multiline.h"
#include <algorithm>

/* Extra polygon-related functions */

/// Class for checking if a point is inside a polygon.
/// Line is always treated as closed.
template <typename T>
class PolyTester{
  std::vector<Point<T> > sb,se; // segment beginning, ending
  std::vector<double> ss; // segment slope
  bool horiz; // test direction
  bool borders; // include borders
public:

  // Constructor, build the tester class for a given polygon.
  // Parameters:
  //  - L -- polygon (represented by Line object)
  //  - horiz -- set test direction
  //  - borders -- include borders?
  PolyTester(const Line<T> & L,
             const bool horiz_ = true,
             const bool borders_=true
            ): horiz(horiz_), borders(borders_){

    // Collect line sides info: start and end points, slopes
    int pts = L.size();
    for (int i = 0; i < pts; i++){
      Point<T> b(L[i%pts].x, L[i%pts].y),
               e(L[(i+1)%pts].x, L[(i+1)%pts].y);
      if (!horiz){ // swap x and y
        b.y = L[i%pts].x; b.x=L[i%pts].y;
        e.y = L[(i+1)%pts].x;
        e.x = L[(i+1)%pts].y;
      }
      if (b.y==e.y) continue; // skip horizontal segments

      // side slope
      double s = double(e.x-b.x)/double(e.y-b.y);
      sb.push_back(b);
      se.push_back(e);
      ss.push_back(s);
    }
  }

  /// get sorted coordinates of the polygon crossings with
  /// y=const (or x=const if horiz=false) line
  std::vector<double> get_cr(T y){
    std::vector<double> cr;
    for (int k = 0; k < sb.size(); k++){
      if ((sb[k].y > y)&&(se[k].y > y)) continue; // segment is above the row
      if ((sb[k].y < y)&&(se[k].y < y)) continue; // segment is below the row

      // if two segments are crossing the row in the same point
      // it could be 0, 1, or 2 actual crossings.
      if (sb[k].y == y){
        int kp = k>0? k-1:sb.size()-1;
        if ((sb[kp].y < y && se[k].y > y) ||
            (sb[kp].y > y && se[k].y < y))
          cr.push_back(sb[k].x);
        else if (borders){
          cr.push_back(sb[k].x);
          cr.push_back(se[kp].x);
        }
        continue;
      }

      if (se[k].y == y){
        continue;
      }

      // segment is crossing the row
      cr.push_back((ss[k] * double(y - sb[k].y)) + sb[k].x);


    }
    sort(cr.begin(), cr.end());
    return cr;
  }

  /// Is coord x inside the polygon at cr line
  bool test_cr(const std::vector<double> & cr, T x) const{
    // number of crossings on the ray (x,y) - (inf,y)
    auto i = lower_bound(cr.begin(), cr.end(), x);
    if (i == cr.end()) return false;
    if (*i==x) return borders;
    int k=0;
    while (i!=cr.end()) { i++; k++; }
    return k%2==1;
  }

};
typedef PolyTester<double> dPolyTester;
typedef PolyTester<int>    iPolyTester;

/**********************************************************/

/// Check if one-segment polygon L covers point P.
template <typename T>
bool
point_in_polygon(const Point<T> & P, const Line<T> & L, const bool borders = true){
  PolyTester<T> lt(L, true, borders);
  auto cr = lt.get_cr(P.y);
  return lt.test_cr(cr, P.x);
}

/// Check if one-segment polygon l covers (maybe partially) rectangle r.
template <typename T>
bool
rect_in_polygon(const Rect<T> & R, const Line<T> & L, const bool borders = true){

  if (!R) return false;

  std::vector<double> cr;
  PolyTester<T> lth(L, true, borders), ltv(L,false, borders);

  // check if there is any crossing at any rectangle side
  cr = lth.get_cr(R.y);
  for (auto const x0:cr){
    if (R.x < x0 && x0 <R.x+R.w ) return true;
    if (borders && (R.x==x0 || x0==R.x+R.w)) return true;
  }
  cr = lth.get_cr(R.y+R.h);
  for (auto const x0:cr){
    if (R.x < x0 && x0 <R.x+R.w ) return true;
    if (borders && (R.x==x0 || x0==R.x+R.w)) return true;
  }

  cr = ltv.get_cr(R.x);
  for (auto const y0:cr){
    if (R.y < y0 && y0 <R.y+R.h ) return true;
    if (borders && (R.y==y0 || y0==R.y+R.h)) return true;
  }
  cr = ltv.get_cr(R.x+R.w);
  for (auto const y0:cr){
    if (R.y < y0 && y0 <R.y+R.h ) return true;
    if (borders && (R.y==y0 || y0==R.y+R.h)) return true;
  }

  // one rectangle corner is inside polygon
  if (ltv.test_cr(cr, R.y)) return true;

  // one of polygon points is inside the rectangle
  if (L.size() && borders && R.contains_u(L[0])) return true;
  if (L.size() && !borders && R.contains_l(L[0])) return true;
  return false;
}

// Join a multi-segment polygon into a single-segment one
// using shortest cuts.
template<typename T>
Line<T> join_polygons(const MultiLine<T> & L){

  Line<T> ret;
  if (L.size()==0) return ret;

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
      if ( !i2->size() || !point_in_polygon((*i2)[0], *i1, false)){
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

/// Read a figure from the string and get its bounding box.
/// The figure can be Point, Line/Multiline, Rect
template <typename T>
MultiLine<T> figure_line(const::std::string &str) {
  MultiLine<T> ret;
  if (str=="") return ret;

  // try point
  try {
    Line<T> l;
    l.push_back(Point<T>(str));
    ret.push_back(l);
    return ret;
  }
  catch (Err e){}

  // try Rect
  try {
    ret.push_back(rect_to_line(Rect<T>(str), true));
    return ret;
  }
  catch (Err e){}

  // try Line/Multiline
  try {
    MultiLine<T> ml(str);
    return ml;
  }
  catch (Err e){}
  throw Err() << "can't read figure: " << str;
}

/// Read a figure from the string and get its bounding box.
/// The figure can be Point, Line/Multiline, Rect
template <typename T>
Rect<T> figure_bbox(const::std::string &str) {
  return figure_line<T>(str).bbox();
}


#endif
