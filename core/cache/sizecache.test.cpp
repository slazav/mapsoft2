///\cond HIDDEN (do not show this in Doxyden)

#include <iostream>
#include "cache/sizecache.h"

using namespace std;

// Int class integer value and size() method.
// Size is equal to the value.
class Int {
public:
    Int(const int& i) : value_(i) { }
    Int(const Int& o) : value_(o.value_) { }
    bool operator== (const Int & i) const {return i.value_==value_;}
    int get() const { return value_; }
    int size() const { return value_; }
private:
    int value_;
};

// operators << and >> for Int
std::ostream& operator<<(std::ostream& s, const Int& i) { s << i.get(); return s; }
std::istream& operator>>(std::istream& s, Int& i) { int v; s >> v; i=Int(v); return s; }

int main() {
  // create a cache with size 250
  SizeCache<int, Int> cache(250);

  assert(cache.count()==0);
  assert(cache.size_total()==250);
  assert(cache.size_used()==0);

  // put elements: i->i^2, i=0..9
  // (only last 4 will be in the cache)
  for (int i = 0; i < 10; ++i) cache.add(i, Int(i*i));

  for (int i =  0; i <  6; ++i) assert(cache.contains(i) == false);
  for (int i =  6; i < 10; ++i) assert(cache.contains(i) == true);
  for (int i =  6; i < 10; ++i) assert(cache.get(i) == Int(i*i));
  for (int i = 10; i < 15; ++i) assert(cache.contains(i) == false);

  assert(cache.count()==4);
  assert(cache.size_total()==250);
  assert(cache.size_used()==230); // 6^2+7^2+8^2+9^2

  // remove elements 6 and 9 using iterators
  SizeCache<int, Int>::iterator it = cache.begin();
  while (it != cache.end()) {
    if (it->first % 3 == 0) it = cache.erase(it);
    else ++it;
  }
  assert(cache.contains(6) == false);
  assert(cache.contains(9) == false);

  // remove element 7 using the key
  cache.erase(7);
  assert(cache.contains(7) == false);

  assert(cache.count()==1);
  assert(cache.size_total()==250);
  assert(cache.size_used()==64);

  // clear the cache
  cache.clear();
  for (int i =  0; i < 15; ++i) assert(cache.contains(i) == false);

  assert(cache.count()==0);
  assert(cache.size_total()==250);
  assert(cache.size_used()==0);

}

///\endcond
