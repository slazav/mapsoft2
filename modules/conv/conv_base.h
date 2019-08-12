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
  virtual void frw_pt(dPoint & p) const {p*=sc_src*sc_dst;}

  // backward point conversion (can be redefined)
  virtual void bck_pt(dPoint & p) const {p/=sc_src*sc_dst;}


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
  virtual dLine frw_acc(const dLine & l, double acc) const;

  /// Convert a line. Each segment can be divided to provide
  /// accuracy <acc> in source units.
  // Note that bck_acc and frw_acc are not symmetric
  // because accuracy is always calculated on the src side.
  virtual dLine bck_acc(const dLine & l, double acc) const;

  /// Convert a rectagle and return bounding box of resulting figure.
  /// Accuracy <acc> is measured in x-y plane in source units.
  virtual dRect frw_acc(const dRect & R, double acc) const {
    return frw_acc(rect_to_line(R), acc).bbox(); }

  /// Convert a rectagle and return bounding box of resulting figure.
  /// Accuracy <acc> is measured in x-y plane in source units (and
  /// thus bck_acc and frw_acc are not symmetric).
  virtual dRect bck_acc(const dRect & R, double acc) const {
    return bck_acc(rect_to_line(R), acc).bbox(); }

/*
  /// Convert angle from y=const line at point p.
  /// Radians.
  virtual double ang_frw(dPoint p, double a, double dx) const;
  virtual double ang_bck(dPoint p, double a, double dx) const;
  /// Degrees.
  virtual double angd_frw(dPoint p, double a, double dx) const;
  virtual double angd_bck(dPoint p, double a, double dx) const;

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

  double sc_src, sc_dst;
};

///@}
#endif
