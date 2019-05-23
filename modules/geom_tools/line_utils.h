#ifndef LINE_UTILS_H
#define LINE_UTILS_H

#include "geom/line.h"
#include "geom/multiline.h"
#include <cassert>

///\addtogroup lib2d
///@{
///\defgroup line_utils
///@{

/*
 Find the minimum distance between a line and a point <pt>.

 If the distance is less then <maxdist> then the distance is returned,
 the nearest point in the line is returned in <pt> and direction in <vec>.

 If the distance is larger then <maxdist> then <maxdist> is returned and
 <pt> and <vec> are not modified.
*/
template<typename T>
double nearest_pt(const Line<T> & line, dPoint & vec, Point<T> & pt, double maxdist){

  Point<T> pm = pt;

  for (int j=1; j<line.size(); j++){
    Point<T> p1(line[j-1]);
    Point<T> p2(line[j]);

    double  ll = dist(p1,p2);
    if (ll==0) continue;
    dPoint v = dPoint(p2-p1)/ll;

    double ls = dist(pt,p1);
    double le = dist(pt,p2);

    if (ls<maxdist){ maxdist=ls; pm=p1; vec=v; }
    if (le<maxdist){ maxdist=le; pm=p2; vec=v; }

    double prl = pscal(dPoint(pt-p1), v);

    if ((prl>=0)&&(prl<=ll)) { // point is inside a segment
      Point<T> pc = p1 + Point<T>(v * prl);
      double lc=dist(pt,pc);
      if (lc<maxdist) { maxdist=lc; pm=pc; vec=v; }
    }
  }
  pt=pm;
  return maxdist;
}

// same for MultiLine
template<typename T>
double nearest_pt(const MultiLine<T> & lines, dPoint & vec, Point<T> & pt, double maxdist){
  dPoint pm=pt;
  typename MultiLine<T>::const_iterator i;
  for (i = lines.begin(); i != lines.end(); i++){
    Point<T> p = pt;
    maxdist = nearest_pt(*i, vec, p, maxdist);
    if ( p != pt) { pm = p;}
  }
  pt=pm;
  return maxdist;
}

/// Found bounding convex polygon for points.
/// Resulting polygon start from point with minimal x,
/// it is always clockwise-oriented, its last point =
/// first point
template <typename T>
Line<T> convex_border(const Line<T> & points){
  Line<T> ret;
  if (points.size()==0) return ret;
  typename Line<T>::const_iterator p, p0, p1;

  // find point with minimal x
  p0=points.begin();
  for (p=points.begin(); p!=points.end(); p++){
    if (p0->x > p->x) p0=p;
  }

  dPoint v0(0,1);

  do {
    ret.push_back(*p0);
    p1=p0;
    p1++; if (p1==points.end()) p1=points.begin();
    if (p0==p1) break;

    // find point with maximal v*norm(p-p0)
    double cmax=-1;
    for (p=points.begin(); p!=points.end(); p++){
      if (*p == *p0) continue;
      dPoint v = norm(*p - *p0);
      double c = pscal(v0, v);
      if (cmax < c) { cmax=c; p1=p;}
    }
    v0=norm(*p1 - *p0);
    p0=p1;
    assert (ret.size() <= points.size());
  } while (*p1!=*ret.begin());
  return ret;
}

// test_pairs helper function for margin_classifier().
// For each pair of points (p1,p2) from oriented contour l1
// find the shortest distance from the line p1-p2 to points in l2.
// Then find the pair with maximum value (but less then maxdist).
// Distance is negative inside l1 and positive outside it.
// The points and distance are returned in maxdist, p1,p2.
template <typename T>
void
test_pairs(const Line<T> & l1, const Line<T> & l2,
           double & maxdist, Point<T> & p1, Point<T> & p2){
  typename Line<T>::const_iterator i,j;
  for (i=l1.begin(); i!=l1.end(); i++){
    j=i+1; if (j==l1.end()) j=l1.begin();
    if (*i==*j) continue;
    double mindist=INFINITY;
    typename Line<T>::const_iterator k;
    for (k=l2.begin(); k!=l2.end(); k++){
      dPoint v1(*j-*i);
      dPoint v2(*k-*i);
      double K = pscal(v1,v2)/pscal(v1,v1);
      double d = len(K*v1-v2);
      if ((v1.x*v2.y-v2.x*v1.y)<0) d*=-1;
      if (d<mindist) mindist=d;
    }
    if (maxdist < mindist){
      maxdist = mindist;
      p1=*i; p2=*j;
    }
  }
}


/*
 Margin Classifier --
 Find line which separates two point sets L1 and L2 with maximal margin.
 Returns margin value (can by < 0).
 p0 is set to line origin, and t is set to line direction.
*/
template <typename T>
double
margin_classifier(const Line<T> & L1, const Line<T> & L2,
  dPoint & p0, dPoint & t){

  // find borders
  Line<T> l1=convex_border(L1);
  Line<T> l2=convex_border(L2);

  // Support vectors (http://en.wikipedia.org/wiki/Support_vector_machine)
  // are located on a convex boundary of a point set. At least in one set
  // there are two of them and they are neighbours in the boundary.

  // For each pair we apply test_pairs() function.
  // Function convex_border() gives clockwise-oriented boundaries,
  // distances are positive outside them.

  double maxdist=-INFINITY;
  Point<T> p1,p2;
  test_pairs(l1,l2,maxdist,p1,p2);
  test_pairs(l2,l1,maxdist,p1,p2);

  // We have found two support vectors: p1 and p2 in one of the boundaries and
  // the distance between sets (positive or negative).

  // Now move the line p1-p2 by maxdist/2 на maxdist/2.

  t = norm(p2-p1);
  dPoint n(-t.y, t.x);
  p0 = dPoint(p1) + n*maxdist/2.0;

  return maxdist;
}


#endif
