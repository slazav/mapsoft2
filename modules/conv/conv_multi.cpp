#include "conv_multi.h"
#include "conv_aff.h"

/// Try to substitude all conversions by a single ConvAff.
//  Algorythm:
//  - make NxN points in the src_box, convert src->dst
//  - build ConvAff using these points
//  - convert points back to src coordinates
//  - measure error (mean square distance, in src coordinates)
//  - if error < E substitude all conversions with the new one
//  - return true if substitution was none, false otherwise.
bool
ConvMulti::simplify(const dRect & src_box, int N, double E) {
  if (size() == 0) return false;

  if (N<2 || N>1000) throw Err() <<
    "ConvMulti::simplify: wrong number of points: " << N;
  dLine pts_src;
  for (int i=0; i<N; ++i) for (int j=0; j<N; ++j){
    pts_src.push_back(src_box.tlc() +
      dPoint(src_box.w*i/(N-1), src_box.h*j/(N-1)));
  }
  int NP = pts_src.size();
  dLine pts_dst(pts_src);
  frw(pts_dst);
  std::map<dPoint, dPoint> ptmap;
  for (int i=0; i<NP; ++i)
    ptmap.emplace(pts_src[i], pts_dst[i]);

  std::shared_ptr<ConvAff2D> new_cnv(new ConvAff2D(ptmap));
  new_cnv->bck(pts_dst); // to src coordinates

  // if error in the conversion is small, use this conversion
  if (new_cnv->get_src_err() < E){
    cnvs.clear();
    dirs.clear();
    cnvs.push_back(new_cnv);
    dirs.push_back(true);
    return true;
  }
  return false;
}
