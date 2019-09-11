#ifndef IMAGE_CACHE_H
#define IMAGE_CACHE_H

#include <string>
#include "cache/cache.h"
#include "geom/line.h"
#include "image_io.h"
#include "image.h"
#include "image_colors.h"

// class for image cache
class ImageCache {
  Cache<std::string, std::pair<int, Image> > cache;

public:
  // constructor: make cache with maxnum capacity.
  ImageCache(const int maxnum) : cache(maxnum){}

  // Load image with scale sc or use already loaded image.
  // Border is applied only once, after loading.
  Image get(const std::string & fn,
             const int sc = 1,
             const iLine & border = iLine()){

    if (cache.contains(fn)){
      auto ip = cache.get(fn);
      if (ip.first == sc) return ip.second;
    }

    Image img = image_load(fn, sc);
    if (border.size()) image_apply_border(img, border/sc, 0);
    cache.add(fn, std::make_pair(sc, img));
    return img;
  }

  // Delete image from the cache
  void del(const std::string & fn){
    if (cache.contains(fn)) cache.erase(fn);
  }


};

#endif
