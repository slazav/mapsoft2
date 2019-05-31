## Geohash

Based on https://github.com/lyokato/libgeohash

* check if Geohash contains only valid characters
```c++
bool GEOHASH_verify(const std::string & hash);
```

* Encode a point, return hash.
```c++
std::string GEOHASH_encode(const dPoint & p, unsigned int hash_length);
```

* Encode a rectangle, return longest hash (but not longer then maxlen)
which covers the rectangle. Can return an ampty string.
```c++
std::string GEOHASH_encode(const dRect & r, unsigned int maxlen);
```

* Encode a rectangle, return up to 4 longest adjacent hashes (not
longer then maxlen) which covers the rectangle. Should be
efficient (give longer hashas) for rectangles which cross hash bundaries.
```c++
std::set<std::string> GEOHASH_encode4(const dRect & r, unsigned int maxlen);
```

* Decode a hash, return coordinate range. Range is empty if hash is not valid.
```c++
dRect GEOHASH_decode(const std::string & hash);
```

* Find adjacent hash. Direction starts from north and goes clockwise
(0:N, 1:NE, 2:E, 3:SE, 4:S, etc.) return empty string on errors.
```c++
std::string GEOHASH_adjacent(const std::string & hash, int dir);
```
