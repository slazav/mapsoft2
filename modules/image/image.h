#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

#include <memory>
#include <err/err.h>

/*
An image: 2d array of arbitrary data
*/



/*********************************************************************/
// base class
class Image {
  private:
    std::shared_ptr<void> data_;
    size_t w,h; // width, height
    size_t b;   // bits per data point

  public:

    size_t width() const  {return w;}
    size_t height() const {return h;}
    size_t bpp() const    {return b;}
    void *data() const    {return data_.get();}

    // constructor -- create an empty image
    Image(): w(0), h(0), b(0) {}

    // constructor -- create WxH non-initialized image
    Image(const size_t W, const size_t H, const size_t bpp):
          w(W), h(H), b(bpp) {

      if (w<=0 || h<=0)
      throw Err() << "non-positive image dimension: " << w << "x" << h;

      if ((b != 1 && b%8) || b<=0)
        throw Err() << "unsuported bpp for image: " << b;

      try{
        data_ = std::shared_ptr<void>(new uint8_t[w*h*bpp/8]);
      }
      catch (const std::bad_alloc & e) {
        throw Err() << "can't allocate memory for "
                    << *this << ": " << e.what();
      }
    }

    // constructor -- create WxH initialized with v
    template <typename T>
    Image(const size_t W, const size_t H, const size_t bpp, const T val):
          Image(W,H,bpp) { fill_int<T>(val); }

    bool is_empty() const {return w==0 || h==0 || b==0;}

    // Fast set -- set value without any checks.
    // Check before that
    //  - image.bpp() == sizeof(T)*8
    //  - x>=0 && x<image.width() 
    //  - y>=0 && x<image.height() 
    // For 1-bpp images use set_int() instead.
    template <typename T>
    void set(const size_t x, const size_t y, const T v) { ((T*)data_.get())[w*y+x] = v; }

    // Fast fill -- fill the whole image without any checks.
    // Same checks as for `set` are required.
    // For 1-bpp images use get_int() instead.
    template <typename T>
    void fill(const T v) { for (int i=0; i<w*h; i++) ((T*)data_.get())[i] = v; }

    // Fast get -- get value without any checks.
    // Same checks as for `set` are required.
    // For 1-bpp images use get_int() instead.
    template <typename T>
    T get(const size_t x, const size_t y) const { return ((T*)data_.get())[w*y+x]; }

    // Smart set: convert integer value to correct size (1,8,16,32, or 64 bits)
    // and write to an image point.
    template <typename T>
    void set_int(const size_t x, const size_t y, const T val) {
      switch (b){
        case 1: {
          size_t b = (w*y+x)/8;
          size_t o = (w*y+x)%8;
          uint8_t old = ((uint8_t*)data_.get())[b];
          ((uint8_t*)data_.get())[b] = val? old|(1<<o) : old&~(1<<o);
          break;
          }
        case 8:  set(x,y, (uint8_t)val); break;
        case 16: set(x,y, (uint16_t)val); break;
        case 32: set(x,y, (uint32_t)val); break;
        case 64: set(x,y, (uint64_t)val); break;
        default:
          throw Err() << "unsuported image bpp in set_int: " << b;
      }
    }

    // Smart fill: convert integer value to correct size (1,8,16,32, or 64 bits)
    // and fill the image.
    template <typename T>
    void fill_int(const T val) {
      switch (b){
        case 1:
          for (int i=0; i<w*h/8; i++) ((uint8_t*)data_.get())[i] = val ? 0xFF:0x00;
          break;
        case 8:  fill((uint8_t)val); break;
        case 16: fill((uint16_t)val); break;
        case 32: fill((uint32_t)val); break;
        case 64: fill((uint64_t)val); break;
        default:
          throw Err() << "unsuported image bpp in set_int: " << b;
      }
    }

    // Smart get: get integer value according with bpp setting
    template <typename T>
    T get_int(const size_t x, const size_t y) const {
      switch (b){
        case 1: {
          size_t b = (w*y+x)/8;
          size_t o = (w*y+x)%8;
          uint8_t v = ((uint8_t*)data_.get())[b];
          return (v >> o) & 1;
          }
        case 8:  return get<uint8_t>(x,y);
        case 16: return get<uint16_t>(x,y);
        case 32: return get<uint32_t>(x,y);
        case 64: return get<uint64_t>(x,y);
        default:
          throw Err() << "unsuported image bpp in get_int: " << b;
      }
    }

};


std::ostream & operator<< (std::ostream & s, const Image & i);

#endif
