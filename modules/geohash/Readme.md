## geohash.h

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

* Sometimes it is needed to encode arbitrary type of coordinates,
not lat-lon. This function does a linear transformation of
coordinates in such a way that `bbox -> dRect(-180,-90,360,90)`.
If bor or bbox is empty then original box is returned without modification.
```c++
dRect GEOHASH_convert_box(const dRect & box, const dRect & bbox);
```

## GeoHashStorage -- spatial indexing storage (see storage.h)

Methods:

* Add object with id and range.
``` c++
virtual void put(const int id, const dRect & range);
```

* Get id of objects which may be found in the range.
``` c++
std::set<int> get(const dRect & range);
```

* Delete an object with id and range.
If the record does not exist do nothing.
``` c++
void del(const int id, const dRect & range);
```

* set bbox for coordinate transformation.
``` c++
void set_bbox(const dRect & bbox_);
```
