#include "image_colors.h"
#include <map>
#include <cassert>

// Reshape color bits to have more sugnificant ones first:
uint32_t
image_reduce_color_enc(const uint32_t c){
  uint32_t ret=0;
  for (int i=7; i>=0; i--){ // 8 bits
    for (int j=3; j>=0; j--){ // 4 bytes
      ret <<= 1;
      ret |= (c >> (j*8+i)) & 1;
    }
  }
  return ret;
}

// Inverse transformation:
uint32_t
image_reduce_color_dec(const uint32_t c){
  uint32_t ret=0;
  for (int j=3; j>=0; j--){ // 4 bytes
    for (int i=7; i>=0; i--){ // 8 bits
      ret <<= 1;
      ret |= (c >> (i*4+j)) & 1;
    }
  }
  return ret;
}

// distance between two colors
double
color_dist(const uint32_t c1, const uint32_t c2){
  return sqrt(
    pow( (int)((c1>>24)&0xFF)-(int)((c2>>24)&0xFF), 2) +
    pow( (int)((c1>>16)&0xFF)-(int)((c2>>16)&0xFF), 2) +
    pow( (int)((c1>> 8)&0xFF)-(int)((c2>>8)&0xFF), 2) +
    pow( (int)(c1&0xFF) - (int)(c2&0xFF), 2));
}

// remove transparency (for scaled colors)
uint32_t color_rem_transp(const uint32_t c, const bool gifmode){
  int a = (c>>24)&0xFF;
  if (a==0) return gifmode? 0 : c|0xFFFFFFFF;
  if (a==255) return c;

  int r = (c>>16)&0xFF;
  int g = (c>> 8)&0xFF;
  int b = c&0xFF;
  if (a>0 && a<0xFF){
    r = (r*256)/a;
    g = (g*256)/a;
    b = (b*256)/a;
    if (r>0xFF) r=0xFF;
    if (g>0xFF) g=0xFF;
    if (b>0xFF) b=0xFF;
  }
  return (0xFF<<24)+(r<<16)+(g<<8)+b;
}


// Create color palette
void
image_color_mkpal(const Image & img, uint32_t *colors, int clen, int mode){
  // palette length should be 1..256
  if (clen < 1 || clen > 256)
    throw Err() << "image_color_reduce: palette length is out of range";

  if (img.bpp()!=32) throw Err() <<
    "image_color_mkpal: only 32-bpp images are supported";

  // Step 1: build full color map
  std::map<uint32_t, uint64_t> cmap; // color -> number of points
  for (int y=0; y<img.height(); ++y){
    for (int x=0; x<img.width(); ++x){
      uint32_t c = img.get<uint32_t>(x,y);
      if (mode>0) c = color_rem_transp(c, mode>1);
      c = image_reduce_color_enc(c);
      if (cmap.count(c) == 0) cmap[c] = 1;
      else ++cmap[c];
    }
  }

  // Split the colormap into 256 ranges with equal number of points
  // Find "center of mass" of each region
  // Fill color palette with encoded colors
  uint64_t total = img.width()*img.height();
  uint64_t rsize = total/clen;
  while (rsize*clen < total) ++rsize;

  uint64_t sum1=0, sum2=0, sum12=0;
  int ci=0;
  for (int i=0; i<clen; i++) colors[i]=0;
  for (auto const & cm:cmap){
    sum1 += cm.first;
    sum2 += cm.second;
    sum12 += cm.second*cm.first;

    if (sum2>=rsize){
      assert(ci<clen);
      colors[ci++] = sum12/sum2;
      sum1=sum2=sum12=0;
    }
  }
  if (sum2>0){
    assert(ci<clen);
    colors[ci++] = sum12/sum2;
  }

  // decode color palette
  for (int i=0; i<clen; i++)
    colors[i]=image_reduce_color_dec(colors[i]);
}

// Reduce number of colors
Image
image_color_reduce(const Image & img, uint32_t *colors, int clen, int mode){

  // we return 8bpp image, palette length should be 1..256
  if (clen < 1 || clen > 256)
    throw Err() << "image_color_reduce: palette length is out of range";

  if (img.bpp()!=32) throw Err() <<
    "image_color_reduce: only 32-bpp images are supported";

  // encode color palette
  for (int i=0; i<clen; i++) colors[i]=image_reduce_color_enc(colors[i]);

  // Construct the new image
  Image img1(img.width(), img.height(), 8);
  for (int y=0; y<img.height(); ++y){
    for (int x=0; x<img.width(); ++x){
      // get encoded color
      uint32_t c = img.get<uint32_t>(x,y);
      if (mode>0) c = color_rem_transp(c, mode>1);
      c = image_reduce_color_enc(c);

      // find nearest palette value
      uint32_t d0=0xFFFFFFFF, i0=0;
      for (int i=0; i<clen; i++){
        // note type overfulls
        uint32_t d = c>colors[i] ? c-colors[i] : colors[i]-c;
        if (d < d0) {d0=d; i0=i;}
      }
      img1.set<uint8_t>(x,y,i0);
    }
  }

  // decode color palette
  for (int i=0; i<clen; i++) colors[i]=image_reduce_color_dec(colors[i]);

  return img1;
}

/***********************************************************/

int
image_classify_alpha(const Image & img){
  int ret=0;
  if (img.bpp()!=32) throw Err() <<
    "image_classify: only 32-bpp images are supported";

  for (int y=0; y<img.height(); y++){
    for (int x=0; x<img.width(); x++){
      uint32_t c = img.get<uint32_t>(x, y);
      int a = (c >> 24) & 0xFF;

      if (a>0 && a<255) {ret=2; break;}
      if (a==0) ret=1;
    }
  }
  return ret;
}

int
image_classify_color(const Image & img, uint32_t *colors, int clen){
  int ret=0;
  if (img.bpp()!=32) throw Err() <<
    "image_classify: only 32-bpp images are supported";

  int nc=0; // number of colors
  for (int i=0; i<clen; ++i) colors[i]=0;

  for (int y=0; y<img.height(); ++y){
    for (int x=0; x<img.width(); ++x){
      uint32_t c = img.get<uint32_t>(x, y);
      int r = (c >> 16) & 0xFF;
      int g = (c >> 8) & 0xFF;
      int b = c & 0xFF;

      if (r!=g || r!=b) ret=1;

      if (nc<=clen){
        bool found=false;
        for (int i=0; i<nc; i++)
          if (c==colors[i]){ found=true; break;}
        if (!found){
          if (nc<clen) colors[nc] = c;
          nc++;
        }
      }

    }
  }
  if (nc>clen) ret+=1;
  return ret;
}

