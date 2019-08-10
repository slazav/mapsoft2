#ifndef CAIRO_WRAPPER_H
#define CAIRO_WRAPPER_H

#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <cairomm/script.h>

#include <string>

#include "image/image.h"
#include "geom/point.h"
#include "geom/line.h"
#include "geom/multiline.h"
#include "geom/rect.h"

/// This class contains functions
/// we want to add to the Cairo::Context
struct CairoExtra : public Cairo::Context {
  void save_png(const char *fname){
    Cairo::Context::get_target()->write_to_png(fname); }

  void set_color_a(const int c){
    Cairo::Context::set_source_rgba(
      ((c&0xFF0000)>>16)/255.0,
      ((c&0xFF00)>>8)/255.0,
       (c&0xFF)/255.0,
      ((c&0xFF000000)>>24)/255.0
    );
  }
  void set_color(const int c){
    Cairo::Context::set_source_rgb(
      ((c&0xFF0000)>>16)/255.0,
      ((c&0xFF00)>>8)/255.0,
       (c&0xFF)/255.0
    );
  }

  // move_to/line_to functions for dPoint arguments

  void move_to(const dPoint & p){
    Cairo::Context::move_to(p.x, p.y); }

  void line_to(const dPoint & p){
    Cairo::Context::line_to(p.x, p.y); }

  void rel_move_to(const dPoint & p){
    Cairo::Context::rel_move_to(p.x, p.y); }

  void rel_line_to(const dPoint & p){
    Cairo::Context::rel_line_to(p.x, p.y); }

  void curve_to(const dPoint & p1, const dPoint & p2, const dPoint & p3){
    Cairo::Context::curve_to(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y); }

  void rel_curve_to(const dPoint & p1, const dPoint & p2, const dPoint & p3){
    Cairo::Context::rel_curve_to(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y); }

  void rectangle(const dRect &r){
    Cairo::Context::rectangle(r.x, r.y, r.w, r.h);
  }
  void circle(const dPoint &p, const double r){
    Cairo::Context::begin_new_sub_path();
    Cairo::Context::arc(p.x, p.y, r, 0, 2*M_PI);
  }
  dPoint get_current_point(){
    dPoint ret;
    Cairo::Context::get_current_point(ret.x, ret.y);
    return ret;
  }
  dRect get_text_extents(const std::string & utf8){
    dPoint p;
    Cairo::Context::get_current_point(p.x, p.y);
    Cairo::TextExtents extents;
    Cairo::Context::get_text_extents (utf8, extents);
    return dRect(p.x+extents.x_bearing, p.y+extents.y_bearing,
                 extents.width, extents.height);
  }

  // Make paths from dLine/dMultiline argument

  void mkpath(const dLine & line, bool close=true){
    if (line.size()==0) return;
    move_to(*line.begin());
    for (auto const & p:line) line_to(p);
    if (close) close_path();
  }
  void mkpath(const dMultiLine & mline, bool close=true){
    for (auto const & l:mline) mkpath(l, close);
  }

  void mkpath_points(const dLine & line){
    for (auto const & p:line){
      move_to(p);
      Cairo::Context::rel_line_to(0,0);
    }
  }
  void mkpath_points(const dMultiLine & mline){
    for (auto const & l:mline) mkpath_points(l);
  }

  // make path for smoothed line
  void mkpath_smline(const dMultiLine & o, bool close=true, double curve_l=0);
  void mkpath_smline(const dLine & o, bool close=true, double curve_l=0);

  // some short functions for cap/miter setting
  void cap_round()  { set_line_cap(Cairo::LINE_CAP_ROUND);  }
  void cap_butt()   { set_line_cap(Cairo::LINE_CAP_BUTT);   }
  void cap_square() { set_line_cap(Cairo::LINE_CAP_SQUARE); }
  void join_miter() { set_line_join(Cairo::LINE_JOIN_MITER); }
  void join_round() { set_line_join(Cairo::LINE_JOIN_ROUND); }

  // short functions for dash line settings
  void set_dash(std::vector<double> d){
    Cairo::Context::set_dash(d, 0);
  }
  void set_dash(double d1, double d2){
    std::vector<double> d;
    d.push_back(d1);
    d.push_back(d2);
    Cairo::Context::set_dash(d, 0);
  }
  void set_dash(double d1, double d2, double d3, double d4){
    std::vector<double> d;
    d.push_back(d1);
    d.push_back(d2);
    d.push_back(d3);
    d.push_back(d4);
    Cairo::Context::set_dash(d, 0);
  }

  // render text using fig font and font size
  void render_text_fig(const char *text, dPoint pos, double ang,
       int color, int fig_font, double font_size, double dpi, int hdir=0, int vdir=0);

  // For font properties see:
  // https://www.freedesktop.org/software/fontconfig/fontconfig-devel/x19.html
  // https://www.freedesktop.org/software/fontconfig/fontconfig-user.html
  // https://wiki.archlinux.org/index.php/Font_configuration
  // Examples of fc_patt:
  //   "Century Schoolbook L:Italic:semicondensed:rgba=none"
  void render_text_fc(const char *text, dPoint pos, double ang,
       int color, const char *fc_patt, double font_size, int hdir=0, int vdir=0);

  void render_border(const iRect & range, const dLine & brd, const int bgcolor);

  /// convert Image to a cairo pattern
  Cairo::RefPtr<Cairo::SurfacePattern> img2patt(const Image & I, double sc=1);

};

//////////////////////////////////////////////////////////////////
/*** CairoWrapper - we need this to create RefPtr<CairoExtra> ***/

struct CairoWrapper: Cairo::RefPtr<CairoExtra> {

private:
  // image is used only in set_surface_img(img)
  // for keeping data of the original image
  Image image;
  Cairo::RefPtr<Cairo::Surface> surface;

public:

  // Create surface and new cairo context
  // using internal image.
  // This should be done before any drawing.
  void set_surface_img(int w, int h);

  // Create surface and new cairo context
  // using external image.
  // This should be done before any drawing.
  void set_surface_img(const Image & img);

  // Create surface and new cairo context
  // using Postscript file.
  // This should be done before any drawing.
  void set_surface_ps(const char *fname, int w, int h);

  // Create surface and new cairo context
  // using PDF file.
  // This should be done before any drawing.
  void set_surface_pdf(const char *fname, int w, int h);

  // Create surface and new cairo context
  // using SVG file.
  // This should be done before any drawing.
  void set_surface_svg(const char *fname, int w, int h);

  // get the surface
  Cairo::RefPtr<Cairo::Surface> get_surface() { return surface; }
};
#endif
