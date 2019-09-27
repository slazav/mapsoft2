#include "cairo/cairo_wrapper.h"
#include "filename/filename.h"
#include "geo_mkref/geo_mkref.h"
#include "geo_data/geo_io.h"
#include "geo_data/conv_geo.h"
#include "viewer/gobj_multi.h"
#include "gobj_trk.h"
#include "gobj_wpts.h"
#include "gobj_maps.h"
#include "draw_pulk_grid.h"
#include "write_geoimg.h"


void
ms2opt_add_geoimg(GetOptSet & opts){
  ms2opt_add_mkref(opts);
  ms2opt_add_drawwpt(opts);
  ms2opt_add_drawtrk(opts);
  ms2opt_add_drawmap(opts);
//  ms2opt_add_drawgrd(opts);
  ms2opt_add_image(opts);
  ms2opt_add_ozimap_o(opts);

  int m = MS2OPT_IMAGE;
  opts.remove("img_in_fmt");
  opts.remove("img_out_fmt");
  opts.add("out_fmt",  1,0,   m, "Image output format: pdf, ps, svg, png, jpeg, tiff, gif.");
  opts.add("bgcolor",  1,0,   m, "Image background color (default 0xFFFFFFFF).");
  opts.add("map",      1,'m', m, "Write map file in OziExprorer format.");
}

void
write_geoimg(const std::string & fname, GeoData & data, const Opt & opts){

  std::string fmt;
  if      (file_ext_check(fname, ".pdf"))  fmt="pdf";
  else if (file_ext_check(fname, ".ps"))   fmt="ps";
  else if (file_ext_check(fname, ".svg"))  fmt="svg";
  else if (file_ext_check(fname, ".png"))  fmt="png";
  else if (file_ext_check(fname, ".jpg"))  fmt="jpeg";
  else if (file_ext_check(fname, ".jpeg")) fmt="jpeg";
  else if (file_ext_check(fname, ".tif"))  fmt="tiff";
  else if (file_ext_check(fname, ".tiff")) fmt="tiff";
  else if (file_ext_check(fname, ".gif"))  fmt="gif";

  if (opts.get("out_fmt","") != "") fmt = opts.get("out_fmt", "");

  // make reference and conversion map -> WGS84
  GeoMap map = geo_mkref(data, opts);
  std::shared_ptr<ConvMap> cnv(new ConvMap(map));

  // find image dimensions
  dRect box = dRect(dPoint(), (dPoint)map.image_size);
  if (box.is_zsize()) box = map.border.bbox();
  if (box.is_zsize()) throw Err() << "write_img: can't get map dimensions";

  // setup cairo context
  CairoWrapper cr;
  Image img;
  int w=box.brc().x, h=box.brc().y;
  if      (fmt == "pdf") cr.set_surface_pdf(fname.c_str(), w,h);
  else if (fmt == "ps")  cr.set_surface_ps(fname.c_str(), w,h);
  else if (fmt == "svg") cr.set_surface_svg(fname.c_str(), w,h);
  else if (fmt == "png" || fmt=="jpeg" || fmt=="tiff" || fmt=="gif"){
    img = Image(w,h,IMAGE_32ARGB);
    img.fill32(0);
    cr.set_surface_img(img);
  }
  else
    throw Err() << "write_geoimg: unknown output format";

  // fill the image with bgcolor
  cr->set_color_a(opts.get<int>("bgcolor", 0xFFFFFFFF));
  cr->paint();

  // construct GObjMulti with all the objects we want to draw:
  GObjMulti obj;
  obj.set_opt(std::shared_ptr<Opt>(new Opt(opts)));
  obj.set_cnv(cnv);

  for (auto & m:data.maps)
    obj.add(3, std::shared_ptr<GObjMaps>(new GObjMaps(m)));

  for (auto & t:data.trks)
    obj.add(2, std::shared_ptr<GObjTrk>(new GObjTrk(t)));

  for (auto & w:data.wpts)
    obj.add(1, std::shared_ptr<GObjWpts>(new GObjWpts(w)));

  // draw tracks and waypoints
  obj.draw(cr, box);

// // draw grid
//  dPoint origin(0,0);
//  if (opts.get("grid", 0))
//    draw_pulk_grid(cr, origin, cnv, opts);

  // write raster formats
  if (fmt == "png" || fmt=="jpeg" || fmt=="tiff" || fmt=="gif"){
    Opt o(opts);
    o.put("img_out_fmt", fmt);
    image_save(img, fname, o);
  }

  // write map file
  if (opts.exists("map")){
    map.image = fname;
    write_ozi_map(opts.get("map","").c_str(), map, opts);
  }
}

