#ifndef CONV_BASE_H
#define CONV_BASE_H

#include "geom/point.h"
#include "geom/line.h"
#include "geom/multiline.h"
#include "geom/rect.h"
//#include "image.h"

///\addtogroup libmapsoft
///@{

/// Trivial point transformation. Children can
/// redefine frw_pt() and bck_pt() methods.
/// Also sc_src and sc_dst parameters should be used (or
/// rescale_src/rescale_dst redifined).
struct ConvBase{

  /// constructor - trivial transformation
  ConvBase(double sc=1.0): sc_dst(1.0), sc_src(1.0){}

  // forward point conversion (can be redefined)
  virtual void frw_pt(dPoint & p) const {p.x*=sc_src*sc_dst; p.y*=sc_src*sc_dst;}

  // backward point conversion (can be redefined)
  virtual void bck_pt(dPoint & p) const {p.x/=sc_src*sc_dst; p.y/=sc_src*sc_dst;}


  /// Forward point transformation.
  virtual void frw(dPoint & p) const {frw_pt(p);}

  /// Backward point transformation.
  virtual void bck(dPoint & p) const {bck_pt(p);}

  /// Convert a Line, point to point.
  virtual void frw(dLine & l) const {
    for (dLine::iterator i=l.begin(); i!=l.end(); i++) frw(*i); }

  /// Convert a Line, point to point.
  virtual void bck(dLine & l) const {
    for (dLine::iterator i=l.begin(); i!=l.end(); i++) bck(*i); }

  /// Convert a MultiLine, point to point.
  virtual void frw(dMultiLine & l) const {
    for (dMultiLine::iterator i=l.begin(); i!=l.end(); i++) frw(*i); }

  /// Convert a MultiLine, point to point.
  virtual void bck(dMultiLine & l) const {
    for (dMultiLine::iterator i=l.begin(); i!=l.end(); i++) bck(*i); }


  /// Convert a line. Each segment can be divided to provide
  /// accuracy <acc> in source units.
  /// If acc<=0 then point-to-point conversion is used.
  virtual dLine frw_acc(const dLine & l, double acc = 0.5) const;

  /// Convert a line. Each segment can be divided to provide
  /// accuracy <acc> in source units.
  /// If acc<=0 then point-to-point conversion is used.
  /// Note that bck_acc and frw_acc are not symmetric
  /// because accuracy is always calculated on the src side.
  virtual dLine bck_acc(const dLine & l, double acc = 0.5) const;

  /// Convert a MultiLine. Each segment of each line
  /// can be divided to provide accuracy <acc> in source units.
  virtual dMultiLine frw_acc(const dMultiLine & l, double acc = 0.5) const;

  /// Convert a MultiLine. Each segment of each line can be
  /// divided to provide accuracy <acc> in source units.
  // Note that bck_acc and frw_acc are not symmetric
  // because accuracy is always calculated on the src side.
  virtual dMultiLine bck_acc(const dMultiLine & l, double acc = 0.5) const;

  /// Convert a rectagle and return bounding box of resulting figure.
  /// Accuracy <acc> is measured in x-y plane in source units.
  virtual dRect frw_acc(const dRect & R, double acc = 0.5) const {
    return frw_acc(rect_to_line(R), acc).bbox(); }

  /// Convert a rectagle and return bounding box of resulting figure.
  /// Accuracy <acc> is measured in x-y plane in source units (and
  /// thus bck_acc and frw_acc are not symmetric).
  virtual dRect bck_acc(const dRect & R, double acc = 0.5) const {
    return bck_acc(rect_to_line(R), acc).bbox(); }

  /// Convert angle (radians, ccw from y=const) at point p.
  virtual double frw_ang(dPoint p, double a, double dx) const;
  virtual double bck_ang(dPoint p, double a, double dx) const;

  /// Convert angle (degrees, ccw from y=const) at point p.
  virtual double frw_angd(dPoint p, double a, double dx) const;
  virtual double bck_angd(dPoint p, double a, double dx) const;

  /// Linear scales, destination units per source units in x and y direction.
  /// box is given in source coordinates.
  dPoint scales(const dRect & box) const;

/*

  /// Convert units
  virtual dPoint units_frw(dPoint p) const;
  virtual dPoint units_bck(dPoint p) const;

  /// Fill dst_image from src_image
  virtual int image_frw(const iImage & src_img, iImage & dst_img,
                        const iPoint & shift = iPoint(0,0),
                        const double scale = 1.0) const;
  virtual int image_bck(const iImage & src_img, iImage & dst_img,
                        const iPoint & shift = iPoint(0,0),
                        const double scale = 1.0) const;
*/

  /// change sc_src parameter
  /// Childs can use this parameter in frw/bck or redefine rescale_src()
  virtual void rescale_src(const double s) { sc_src*=s; }

  /// change sc_dst parameter
  /// Childs can use this parameter in frw/bck or redefine rescale_dst()
  virtual void rescale_dst(const double s) { sc_dst*=s; }

protected:
  double sc_src, sc_dst;
};

///@}
#endif
