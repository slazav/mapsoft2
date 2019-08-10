#include "cairo_wrapper.h"
#include "geom/line.h"
#include "err/err.h"

void
CairoExtra::mkpath_smline(const dMultiLine & o, bool close, double curve_l){
  for (dMultiLine::const_iterator l=o.begin(); l!=o.end(); l++)
    mkpath_smline(*l, close, curve_l);
}

void
CairoExtra::mkpath_smline(const dLine & o, bool close, double curve_l){
  if (o.size()<1) return;
  dPoint old;
  bool first = true;
  for (dLine::const_iterator p=o.begin(); p!=o.end(); p++){
    if (p==o.begin()){
      move_to(*p);
      old=*p;
      continue;
    }
    if (curve_l==0){
      line_to(*p);
    }
    else {
      dPoint p1,p2;
      if (len2d(*p - old) > 2*curve_l){
        p1 = old + norm2d(*p - old)*curve_l;
        p2 = *p - norm2d(*p - old)*curve_l;
      }
      else {
        p1=p2=(*p+old)/2.0;
      }
      if (!first){
        curve_to(old, old, p1);
      }
      else {
        first=false;
      }
      line_to(p2);
    }
    old=*p;
  }
  if (curve_l!=0)  line_to(old);
  if (close) close_path();
}

void
CairoExtra::render_text_fig(const char *text, dPoint pos, double ang,
       int color, int fig_font, double font_size, double dpi, int hdir, int vdir){
  Cairo::Context::save();
  set_color(color);

  std::string       face;
  Cairo::FontSlant  slant;
  Cairo::FontWeight weight;
  switch(fig_font){
    case 0:
      face="times";
      slant=Cairo::FONT_SLANT_NORMAL;
      weight=Cairo::FONT_WEIGHT_NORMAL;
      break;
    case 1:
      face="times";
      slant=Cairo::FONT_SLANT_ITALIC;
      weight=Cairo::FONT_WEIGHT_NORMAL;
      break;
    case 2:
      face="times";
      slant=Cairo::FONT_SLANT_NORMAL;
      weight=Cairo::FONT_WEIGHT_BOLD;
      break;
    case 3:
      face="times";
      slant=Cairo::FONT_SLANT_ITALIC;
      weight=Cairo::FONT_WEIGHT_BOLD;
      break;
    case 16:
      face="sans";
      slant=Cairo::FONT_SLANT_NORMAL;
      weight=Cairo::FONT_WEIGHT_NORMAL;
      break;
    case 17:
      face="sans";
      slant=Cairo::FONT_SLANT_OBLIQUE;
      weight=Cairo::FONT_WEIGHT_NORMAL;
      break;
    case 18:
      face="sans";
      slant=Cairo::FONT_SLANT_NORMAL;
      weight=Cairo::FONT_WEIGHT_BOLD;
      break;
    case 19:
      face="sans";
      slant=Cairo::FONT_SLANT_OBLIQUE;
      weight=Cairo::FONT_WEIGHT_BOLD;
      break;
    default:
      std::cerr << "warning: unsupported fig font: " << fig_font << "\n";
      face="sans";
      slant=Cairo::FONT_SLANT_NORMAL;
      weight=Cairo::FONT_WEIGHT_NORMAL;
  }

  if (face=="times") font_size/=0.85;
  Cairo::Context::set_font_size(font_size*dpi/89.0);
  Cairo::Context::set_font_face(
    Cairo::ToyFontFace::create(face, slant, weight));

  dRect ext = get_text_extents(text);
  move_to(pos);
  Cairo::Context::rotate(ang);
  if (hdir == 1) Cairo::Context::rel_move_to(-ext.w/2, 0.0);
  if (hdir == 2) Cairo::Context::rel_move_to(-ext.w, 0.0);
  if (vdir == 1) Cairo::Context::rel_move_to(0.0, ext.h/2);
  if (vdir == 2) Cairo::Context::rel_move_to(0.0, ext.h);
  Cairo::Context::reset_clip();
  Cairo::Context::show_text(text);
  Cairo::Context::restore();
}

void
CairoExtra::render_text_fc(const char *text, dPoint pos, double ang,
       int color, const char *fc_patt, double font_size, int hdir, int vdir){
  Cairo::Context::save();
  set_color(color);
  Cairo::Context::set_font_size(font_size);

  // For work with patterns see:
  // https://www.freedesktop.org/software/fontconfig/fontconfig-devel/x103.html#AEN242
  // For font properties see:
  // https://www.freedesktop.org/software/fontconfig/fontconfig-devel/x19.html
  // https://www.freedesktop.org/software/fontconfig/fontconfig-user.html
  // https://wiki.archlinux.org/index.php/Font_configuration
  FcPattern *patt = FcNameParse((const FcChar8 *)fc_patt);
  set_font_face( Cairo::FtFontFace::create(patt));
  FcPatternDestroy(patt);

  dRect ext = get_text_extents(text);
  move_to(pos);
  Cairo::Context::rotate(ang);
  if (hdir == 1) Cairo::Context::rel_move_to(-ext.w/2, 0.0);
  if (hdir == 2) Cairo::Context::rel_move_to(-ext.w, 0.0);
  if (vdir == 1) Cairo::Context::rel_move_to(0.0, ext.h/2);
  if (vdir == 2) Cairo::Context::rel_move_to(0.0, ext.h);
  Cairo::Context::reset_clip();
  Cairo::Context::show_text(text);
  Cairo::Context::restore();
}


void
CairoExtra::render_border(const iRect & range, const dLine & brd, const int bgcolor){
  // make border path
  mkpath(brd);
  if (bgcolor!=0){
    // draw border
    set_source_rgb(0,0,0);
    set_line_width(2);
    stroke_preserve();
  }

  // erase everything outside border
  mkpath(rect_to_line(expand(range,1)));
  set_fill_rule(Cairo::FILL_RULE_EVEN_ODD);
  if (bgcolor==0)  set_operator(Cairo::OPERATOR_CLEAR);
  else  set_color(bgcolor);
  fill();
}

Cairo::RefPtr<Cairo::SurfacePattern>
CairoExtra::img2patt(const Image & img, double sc){
  try{
    const Cairo::Format format = Cairo::FORMAT_ARGB32;
    if (img.bpp()!=32)
      throw Err() << "CairoWrapper: only 32-bpp images are supported";
    if (Cairo::ImageSurface::format_stride_for_width(format, img.width())
        != img.width()*4)
      throw Err() << "CairoWrapper: non-compatable data";

    Cairo::RefPtr<Cairo::ImageSurface> patt_surf =
      Cairo::ImageSurface::create((unsigned char*)img.data(),
        format, img.width(), img.height(), img.width()*4);
    Cairo::RefPtr<Cairo::SurfacePattern> patt =
      Cairo::SurfacePattern::create(patt_surf);
    Cairo::Matrix M=Cairo::identity_matrix();
    M.translate(patt_surf->get_width()/2.0, patt_surf->get_height()/2.0);
    M.scale(sc,sc);
    patt->set_matrix(M);
    return patt;
  }
  catch (Cairo::logic_error err){
    throw Err() << err.what();
  }
}

/// Cairo Wrapper functions

void
CairoWrapper::set_surface_img(int w, int h){
  image=Image();
  Cairo::Format format = Cairo::FORMAT_ARGB32;
  // check if surface raw data compatable with iImage
  if (image.bpp()!=32)
    throw Err() << "CairoWrapper: only 32-bpp images are supported";
  if (Cairo::ImageSurface::format_stride_for_width(format, w)!= w*4)
    throw Err() << "CairoWrapper: non-compatable data";

  // create surface
  surface = Cairo::ImageSurface::create(format, w, h);

  Cairo::RefPtr<CairoExtra>::operator=
    (cast_static(Cairo::Context::create(surface)));
}

void
CairoWrapper::set_surface_img(const Image & img){
  image=img; // increase refcount of image
  Cairo::Format format = Cairo::FORMAT_ARGB32;
  // check if surface raw data compatable with Image
  if (img.bpp()!=32)
    throw Err() << "CairoWrapper: only 32-bpp images are supported";
  if (Cairo::ImageSurface::format_stride_for_width(format, img.width())
      != img.width()*4)
    throw Err() << "CairoWrapper: non-compatable data";

  surface = Cairo::ImageSurface::create((unsigned char*)img.data(),
      format, img.width(), img.height(), img.width()*4);
  Cairo::RefPtr<CairoExtra>::operator=
    (cast_static(Cairo::Context::create(surface)));
}

void
CairoWrapper::set_surface_ps(const char *fname, int w, int h){
  surface = Cairo::PsSurface::create(fname, w, h);
  Cairo::RefPtr<CairoExtra>::operator=
    (cast_static(Cairo::Context::create(surface)));
}

void
CairoWrapper::set_surface_pdf(const char *fname, int w, int h){
  surface = Cairo::PdfSurface::create(fname, w, h);
  Cairo::RefPtr<CairoExtra>::operator=
    (cast_static(Cairo::Context::create(surface)));
}

void
CairoWrapper::set_surface_svg(const char *fname, int w, int h){
  surface = Cairo::SvgSurface::create(fname, w, h);
  Cairo::RefPtr<CairoExtra>::operator=
    (cast_static(Cairo::Context::create(surface)));
}


