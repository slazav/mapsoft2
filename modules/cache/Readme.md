-----------------
## Cache class

`Cache<K,V>` is a cache of objects of type V, keyed by type K.
Eviction starts whenever number of elements exceeds some limit (cache
size) set at construction and removes least recently used elements.

- `Cache(n)` -- Constructor, create a cache with size n,
- `Cache(c)` -- Copy constructor,
- `c1.swap(c2)` -- swap cache c1 with c2,
- `c1=c2` -- assignment,
- `c.size_total()` -- cache size (maximum number of elements),
- `c.size_used()` -- size of used space (number of elements),
- `c.add(key, value)` -- add an element,
- `c.contains(key)` -- check if the cache contains a key,
- `c.get(key)` -- get an element (throw error if it does not exist),
- `c.erase(key)` -- erase an element,
- `c.clear()` -- clear the cache,
- Iterator support (const_iterator pointing to std::pair(K,V)):
  - `i=c.begin(), i=c.end(), c.erase(i)`,
  - `i++`, `i--`, `*i`.

-----------------
## SizeCache class

`SizeCache<K,V>` is a cache of objects of type V, keyed by type K. Cache
eviction policy is size-based LRU: eviction starts whenever total stored
size exceeds threshold set at construction and removed least recently
used elements. To enable that, V must have method size() returning the
size of object.

- `SizeCache(n)` -- Constructor, create a cache with size n,
- `SizeCache(c)` -- Copy constructor,
- `c1.swap(c2)` -- swap cache c1 with c2,
- `c1=c2` -- assignment,
- `c.count()` -- number of elements in the cache
- `c.size_total()` -- cache size,
- `c.size_used()` -- size of used space,
- `c.add(key, value)` -- add an element,
- `c.contains(key)` -- check if the cache contains a key,
- `c.get(key)` -- get an element (throw error if it does not exist),
- `c.erase(key)` -- erase an element,
- `c.clear()` -- clear the cache,
- Iterator support (iterator pointing to std::pair(K,V)):
  - `i=c.begin(), i=c.end(), c.erase(i)`,
  - `i++`, `i--`, `*i`.
