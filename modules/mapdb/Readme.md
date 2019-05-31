## MapDB -- a simple local storage for vector maps with fast access and spatial indexing.

At the first step four basic classes are implemented:

* Shp -- interface to shp/shx files utilising shapelib.
http://shapelib.maptools.org/

This database can store point/multiline/polygon data with fast access by
integer ID. It looks convenient to store all types of objects as lines.

* Dbf -- interface to dbf files utilising shapelib.
http://shapelib.maptools.org/

This database is a part of shape format. It keeps a table of object
attributes (fixed-length strings or numbers). One can read/write
attribute number N for object number M. This database is good for keeping
short attributes, but not variable-size data (like information about multiple
labels for an object).

* DBSimple -- a simple Berkleydb-based database with integer key and
std::string value. I plan to use it for vector map attributes  instead of
Dbf (and maybe also for coordinates instead of Shp).

* GeoHashDB -- BerkleyDB-based spatial indexing database (analog of
GeoHashStorage from module/geohash).



