#include <set>
#include <map>
#include <string>

#include "err/err.h"
#include "geohash.h"
#include "storage.h"

// Max hash length. 12 gives 0.1m accuracy
#define HASHLEN 12

void
GeoHashStorage::put(const int id, const dRect & range){
  std::set<std::string> hashes =
    GEOHASH_encode4(GEOHASH_convert_box(range, bbox), HASHLEN);
  for (auto const & h:hashes) storage.insert(std::make_pair(h, id));
}

std::set<int>
GeoHashStorage::get(const dRect & range){
  std::set<std::string> hashes =
    GEOHASH_encode4(GEOHASH_convert_box(range, bbox), HASHLEN);
  std::set<int> ret;
  std::set<std::string> done;
  for (auto const & h:hashes) {
    for (int i=0; i<=h.size(); i++) {
      std::string hh = h.substr(0,i);
      if (done.count(hh)) continue; // do not repeat queries with same hash
      bool exact = i < h.size();  // for full hashes look also for smaller regions.
      done.insert(hh);
      // std::cerr << "GET [" << hh << "] " << (i<h.size()) << "\n";
      std::set<int> r = get_hash(hh, i<h.size());
      ret.insert(r.begin(), r.end());
    }
  }
  return ret;
}

std::set<int>
GeoHashStorage::get_hash(const std::string & hash0, bool exact){

  std::set<int> ret;
  std::multimap<std::string, int>::const_iterator i;
  for (i = storage.lower_bound(hash0); i != storage.end(); i++) {
    std::string hash = i->first;
    if (exact && hash!=hash0) break;

    if (hash.size()>=hash0.size() &&
        hash.compare(0,hash0.size(), hash0)==0){
      ret.insert(i->second);
      continue;
    }
    break;
  }
  return ret;
}
