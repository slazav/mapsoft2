///\cond HIDDEN (do not show this in Doxyden)

#include <iostream>
#include <cassert>
#include "err/assert_err.h"
#include "cache.h"

using namespace std;

int main() {
  try {

    // create a cache with size 5
    Cache<int, int> cache(5);
    assert_eq(cache.size_total(), 5);
    assert_eq(cache.size_used(), 0);

    // put elements: i->i^2, i=0..9
    // (only last five will be in the cache)
    for (int i = 0; i < 10; ++i) cache.add(i, i*i);

    for (int i =  0; i <  5; ++i) assert_eq(cache.contains(i), false);
    for (int i =  5; i < 10; ++i) assert_eq(cache.contains(i), true);
    for (int i =  5; i < 10; ++i) assert_eq(cache.get(i), i*i);
    for (int i = 10; i < 15; ++i) assert_eq(cache.contains(i), false);
    assert_eq(cache.size_total(), 5);
    assert_eq(cache.size_used(), 5);

    // remove elements 6 and 9 using iterators
    Cache<int, int>::iterator it = cache.begin();
    while (it != cache.end()) {
      if (it->first % 3 == 0) it = cache.erase(it);
      else ++it;
    }
    assert_eq(cache.contains(6), false);
    assert_eq(cache.contains(9), false);
    assert_eq(cache.size_total(), 5);
    assert_eq(cache.size_used(), 3);

    // remove element 5 using the key
    cache.erase(5);
    assert_eq(cache.contains(5), false);
    assert_eq(cache.size_used(), 2);

    // clear the cache
    cache.clear();
    assert_eq(cache.size_used(), 0);
    for (int i =  0; i < 15; ++i) assert_eq(cache.contains(i), false);

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
}

///\endcond
