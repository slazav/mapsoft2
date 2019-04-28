#ifndef SIZECACHE_H
#define SIZECACHE_H

#include <map>
#include <vector>
#include <set>
#include <cassert>
#include <iostream>

///\addtogroup libmapsoft
///@{
///\defgroup SizeCache
///cache of objects with limited size
///@{

/** Cache of objects of type V, keyed by type K.

    Cache eviction policy is size-based LRU: eviction starts whenever
    total stored size exceeds threshold set at construction and
    removed least recently used elements.
    To enable that, V must have method size() returning the size of
    object.

    Compiler directives:
    - DEBUG_SCACHE      -- log additions/deletions
    - DEBUG_SCACHE_GET  -- log gets
*/

template <typename K, typename V>
class SizeCache {
public:
    typedef typename std::map<K, V>::iterator iterator;

    /// Constructor: create the cache with size n
    SizeCache (int size) : upper_limit(size), current_size(0) { }

    /// Copy constructor
    SizeCache (SizeCache const & other)
      : upper_limit(other.upper_limit),
        current_size(other.current_size),
        storage(other.storage),
        usage(other.usage) { }

    /// Swap the cache with another one
    void swap (SizeCache<K,V> & other) {
      std::swap(upper_limit, other.upper_limit);
      std::swap(current_size, other.current_size);
      storage.swap(other.storage);
      usage.swap(other.usage);
    }

    /// Assignment
    SizeCache<K,V> & operator= (SizeCache<K,V> const& other) {
      SizeCache<K,V> dummy (other);
      swap(dummy);
      return *this;
    }

    /// Return number of elements in the cache
    int count(){
      return storage.size();
    }

    /// Return cache size
    int size_total(){
      return upper_limit;
    }

    /// Return total size of all elements
    int size_used() {
        return current_size;
    }

    /// Add element to the cache
    int add (K const & key, V const & value) {
        if (contains(key)) {
            erase(key);
#ifdef DEBUG_CACHE
            std::cout << "cache: replace " << key << " ";
#endif
        } else {
#ifdef DEBUG_CACHE
            std::cout << "cache: add " << key << " ";
#endif
        }

        int size = value.size();
        while (storage.size() > 0 && current_size + size > upper_limit) {
            el_index lru = usage[usage.size() - 1];
            int s = lru->second.size();
#ifdef DEBUG_CACHE
            std::cout << "no free space:"
                      << " current_size=" << current_size
                      << " lru=" << lru.first
                      << " size=" << s
                      << std::endl;
#endif
            current_size -= s;
            usage.resize(usage.size() - 1);
            storage.erase(lru);
        }

        std::pair<el_index, bool> n = storage.insert(std::make_pair(key, value));
        current_size += size;
        use(n.first);

#ifdef DEBUG_CACHE
        std::cout << "cache usage:";
        for (int i = 0; i < usage.size(); ++i) {
          std::cout << " " << usage[i].first;
        }
        std::cout << std::endl;
#endif
        return 0;
    }

    /// Check whether the cache contains a key
    bool contains (K const & key) {
      return storage.count(key) > 0;
    }

    /// Get element from cache
    V & get (K const & key) {
      el_index ind = storage.find(key);
      assert(ind != storage.end());
#ifdef DEBUG_CACHE_GET
      std::cout << "cache get: " << key << std::endl;
#endif
      use(ind);
      return ind->second;
    }

    /// Remove an element from the cache
    void erase(K const & key){
      el_index ind = storage.find(key);
      if (ind == storage.end())  return;

      for (int k = 0; k < usage.size(); ++k) {
        if (usage[k] == ind) {
          usage.erase(usage.begin() + k);
          break;
        }
      }
      current_size -= ind->second.size();
      storage.erase(ind);
    }

    /** Clear the cache. */
    void clear() {
      storage.clear();
      usage.clear();
      current_size = 0;
    }

    /// Returns an iterator pointing to the first element in the cache.
    /// Iterator traverses all the cache elements
    /// in order of increasing keys. All iterators are valid until the
    /// first cache insert or delete (changing value for existing key
    /// does not invalidate iterators).
    iterator begin() {
      return storage.begin();
    }

    /// Returns an iterator pointing to the last element in the cache.
    /// Iterator traverses all the cache elements
    /// in order of increasing keys. All iterators are valid until the
    /// first cache insert or delete (changing value for existing key
    /// does not invalidate iterators).
    iterator end() {
      return storage.end();
    }

    /// Erase an element pointed to by the iterator
    iterator erase(iterator it) {
      for (int k = 0; k < usage.size(); ++k) {
        if (usage[k] == it) {
          usage.erase(usage.begin() + k);
          break;
        }
      }
      current_size -= it->second.size();
      iterator it2(it);
      ++it2;
      storage.erase(it);
      return it2;
    }

private:
    int upper_limit;
    int current_size;

    typedef typename std::map<K, V>::iterator el_index;
    std::map<K, V> storage;
    std::vector<el_index> usage;

    // index end is removed
    template <typename T>
    void push_vector (std::vector<T> & vec, int start, int end) {
#ifdef DEBUG_CACHE_GET
      std::cout << "cache push_vector: start=" << start << " end=" << end << " size=" << vec.size() << std::endl;
#endif
      for (int i = end; i > start; --i)
        vec[i] = vec[i-1];
    }

    void use (el_index ind) {
      int i;
      for (i = 0; i < usage.size() && usage[i] != ind; ++i);
      if (i == usage.size()) {
        usage.resize (usage.size()+1);
        push_vector (usage, 0, usage.size()-1);
        usage[0] = ind;
      } else {
        push_vector (usage, 0, i);
        usage[0] = ind;
      }
    }
};

/// Print cache elements.
///template <typename K, typename V>
///std::ostream & operator<< (std::ostream & s, SizeCache<K,V> & cache){
///  s << "Cache(\n";
///  for (typename SizeCache<K, V>::iterator it = cache.begin(); it != cache.end(); ++it) {
///    s << "  " << it->first << " => " << it->second << "\n";
///  }
///  s << ")";
///  return s;
///}

///@}
#endif
