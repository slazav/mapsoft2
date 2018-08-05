\mainpage Mapsoft libraries

Here only a short summery of library methods is collected. See
class references for more information or read code.

-----------------
## Err class

Err class is used in all mapsoft libraries for throwing
human-readable text exceptions (with an optional error code).

- Throwing an error:
```c
throw Err() << "some message";
```

- Catching and printing error:
```c
catch (Err E){ cerr << E.str() << "\n"; }
```

- Integer error code can be set as an optional constructor parameter
  and extracted by `code` method: `E.code()`. Default code is -1.
```c
try {throw Err(1); } catch (Err E) {int code=E.code();}
```

\ref Err "Class reference..."

-----------------
## Opt class

Opt class is a `map<string,string>` container with functions for
getting/putting values of arbitrary types. Data types should have `<<`,
`>>` operators and a constructor without arguments.

- Converting any object to std::string and back:
```c
str = str_to_type(v);
v = type_to_str<type>(str);
```
Here `v` is some object of type `type`.

- Creating Opt class, putting and extracting values:
```c
Opt o;
o.put<type>("key", v);
v = o.get<type>("key", def);
```
Here "key" is a string key which is used to access data.
If no value is set for this key, the `def` object is returned.

- Opt object can be converted to a string and back (and thus used inside Opt class).
  String representation is a JSON object with string fields.

\ref Opt "Class reference..."

-----------------
## Getop group
Mapsoft getopt wrapper.

parse_options() allows to build a command line interface with many
option groups:
```
program [<general_options>|<global_input_options>]\
        <input_file_1> [<input_options_1>]\
        <input_file_2> [<input_options_2>] ...\
        (--out|-o) <output_file> [<output_options>]
```
(example can be found in `getopt.test.cpp`)

Structure `ext_option` is used as an extention of a structure from
`getopt_long (3)`. It contains option description and group number which
is used for selecting a few sets of options from a single list.

`print_options()` can be used for printing option list (with
descriptions) in text or in pod format (pod then can be used for
generating man pages).

`parse_options_all()` can be used in simple tools to collect all options
and non-option arguments from a command line.

\ref Getopt "Group reference..."

-----------------
## Point class

`Point<T>` is a 3D point with coordinates of arbitrary numerical type T.

- Constructors:
  - `Point()` -- point with zero coordinates,
  - `Point(x,y,z=0)` -- point with coordinates x and y,
  - `Point(string)` -- read from std::string (see below).

- Typedefs:
  - `dPoint` is a `Point<double>`,
  - `iPoint` is a `Point<int>`.

- Arithmetic operations (`p` are points, `k` are numbers
  of same type as point coordinates):
  - `p+=p`, `p-=p`, `p+p`, `p-p`, `-p`,
  - `p*=k`, `p/=k`, `p*k`, `k*p`, `p/k`,
  - `p==p`, `p!=p`,
  - `p<p`, `p<=p`, `p>=p`, `p>p`

- Other operations:
  - `p1.swap(p2)` -- swap two points,
  - `dPoint(p)`, `iPoint(p)` -- cast to double- or integer-coordinate point,
  - `p.mlen()`, `mlen(p)` -- manhattan length: abs(x) + abs(y),
  - `p.len()`, `len(p)` -- calculate length: sqrt(x^2 + y^2),
  - `p.norm()`, `norm(p)` -- normalize: p/len(p),
  - `p.rint()`, `rint(p)` -- set coordinates to nearest integer values,
  - `p.floor()`, `floor(p)` -- set coordinates to nearest smaller integers,
  - `p.ceil()`, `ceil(p)` -- set coordinates to nearest larger integers,
  - `p.abs()`,  `abs(p)` -- set coordinates to their absolute values,
  - `p.rotate2d(pc,a)`,  `rotate2d(p,pc,a)` -- rotate around central point pc by angle a (rad, clockwise) in x-y plane,
  - `p.flatten()`,  `flatten(p)` -- project the point to x-y plane.
  - `pscal(p1,p2)` -- scalar product: p1.x*p2.x + p1.y*p2.y + p1.z*p2.z,
  - `dist(p1,p2)` -- distance between points: (p1-p2).len().

- Point can be converted to a string and back
  (and thus used inside Opt class). String representation is a
  JSON array with two numerical fields `[x,y,z]` or `[x,y]` if `z==0`.

\ref Point "Class reference..."

-----------------
## Rect class

Rect<T> is a 2D rectangle with coordinates of arbitrary numerical type T.
Rectangle is defined by top-left corner coordinates `x,y`, width `w`,
height `h` and empty flag `e`.

- Coordinates are counted from top-left.
  Top-left corner has lowest coordinates, bottom-right corner
  has highest coordinates. Width and height are always non-negative.

- There is a difference between empty rectangle (such as a
  bounding box of a zero-point line) and zero-size rectangle
  (bounding box of a one-point line). Many functions throw
  error if rectangle is empty.

- If 3D points are used in operations with 2D rectangles, only `x` and `y`
coordinates are involved.

- Constructors:
  - `Rect()` -- empty rectangle,
  - `Rect(x,y,w,h)` -- non-empty rectangle with x,y,w,h values
     (it is possible to use negative w and h here, then rectangle will be
      put on top of y or on the left of x),
  - `Rect(p1,p2)` -- non-empty rectangle with opposite corners p1,p2.
  - `Rect(string)` -- read from std::string (see below)

- Typedefs:
  - `dRect` is a `Rect<double>`,
  - `iRect` is a `Rect<int>`.

- Arithmetic operations (`r` are rectangles, `p` points, `k` numbers):
  - `r+=p, r-=p, r+p, p+r, r-p, -r`
  - `r*=k, r/=k, r*k, k*r, r/k`
  - `r==r, r!=r`
  - `r<r, r<=r, r>=r, r>r`

- Other operations:
  - `r1.swap(r2)` -- swap rectangles
  - `r.empty()` -- check if rectangle is empty
  - `r.zsize()` -- check if rectangle has zero size (w==0 or h==0, but not empty)
  - `r.tcl(), r.trc(), r.brc(), r.blc(), r.cnt()` -- top-left, top-right, bottom-left, bottom-left corners and central point,
  - `dRect(r)`, `iRect(r)` -- cast to double- or integer-coordinate rectangle
  - `r.rint()`, `rint(r)` -- set coordinates to nearest integer values,
  - `r.floor()`, `floor(r)` -- shrink the rectangle to nearest integers,
  - `r.ceil()`, `ceil(r)` -- expand the rectangle to nearest integers,
  - `r.pump(k)`, `pump(r,k)` -- pump rectangle to each side by k value,
  - `r.pump(kx,ky)`, `pump(r,kx,ky)` -- pump by kx and ky in x and y directions,
  - `r.expand(p)`, `expand(r,p)` -- expand rectangle to cover point p,
  - `r1.expand(r2)`, `expand(r1,r2)` -- expand rectangle to cover rectangle r2,
  - `r1.intersect(r2)`, `intersect(r1,r2)` -- intersection with rectangle r2,
  - `r.contains_l(p)`, `contains_l(r,p)` -- check if rectangle contains a point, only lower bound is included,
  - `r.contains_u(p)`, `contains_u(r,p)` -- check if rectangle contains a point, only upper bound is included,
  - `r.contains_n(p)`, `contains_n(r,p)` -- check if rectangle contains a point, bounds are not included,
  - `r.contains(p)`, `contains(r,p)` -- check if rectangle contains a point, all bounds are included,
  - `r1.contains(r2)`, `contains(r1,r2)` -- check if rectangle contains another rectangle,

- Rect can be converted to a string and back
  (and thus used inside Opt class). String representation is a
  JSON array with four numerical fields `[x,y,w,h]` or empty array.

\ref Rect "Class reference..."

-----------------
## Line class

Line is a std::vector of Point.

- Constructors:
  - `Line()` -- empty line,
  - `Line(string)` -- read from std::string (see below)

- Typedefs:
  - `dLine` is a `Line<double>`,
  - `iLine` is a `Line<int>`.

- Arithmetic operations (`l` are lines, `p` points, `k` numbers):
  - `l+=p, l-=p, l+p, p+l, l-p, -l`
  - `l*=k, l/=k, l*k, k*l, l/k`
  - `l==l, l!=l`
  - `l<l, l<=l, l>=l, l>l`

- Other operations:
  - `dLine(l)`, `iLine(l)` -- cast to double- or integer-coordinate line
  - `l.length()`, `length(l)` -- line length
  - `l.invert()`, `invert(l)` -- invert the line
  - `l1.is_shifted(l2, sh)`, `is_shifted(l1, l2, sh)` -- check if line l2 is a shifted version of l1, return the shift
  - `l.rint()`, `rint(l)` -- set coordinates to nearest integer values,
  - `l.bbox2d()`, `bbox2d(l)` -- return a bounding box (Rect object) in x-y plane,
  - `l.rotate2d(pc,a)`,  `rotate2d(l,pc,a)` -- rotate around central point pc by angle a (rad, clockwise) in x-y plane,
  - `l.flatten()`,  `flatten(l)` -- project the line to x-y plane.
  - `rect_to_line(r)` -- convert a rectangle to line.

- Line can be converted to a string and back
  (and thus used inside Opt class). String representation is a
  JSON array with zero or more points (example: "[[1,2,1],[3,4,2],[0,0]]").

\ref Line "Class reference..."

-----------------
## MultiLine class

Line with multiple segments (std::vector of Line).

- Constructors:
  - `MiltiLine()` -- empty line,
  - `MiltiLine(string)` -- read from std::string (see below).

- Typedefs:
  - `dMultiLine` is a `MultiLine<double>`,
  - `iMultiLine` is a `MultiLine<int>`.

- Arithmetic operations (`l` are MultiLines, `p` points, `k` numbers):
  - `l+=p, l-=p, l+p, p+l, l-p, -l`,
  - `l*=k, l/=k, l*k, k*l, l/k`,
  - `l==l, l!=l`,
  - `l<l, l<=l, l>=l, l>l`.

- Other operations:
  - `dMultiLine(l)`, `iMultiLine(l)` -- cast to double- or integer-coordinate MultiLine,
  - `l.length()`, `length(l)` -- line length (sum of segments' lengths),
  - `l.bbox2d()`, `bbox2d(l)` -- return a bounding box in x-y plane (Rect object),
  - `l.rint()`, `rint(l)` -- set coordinates to nearest integer values,
  - `l.rotate2d(pc,a)`,  `rotate2d(l,pc,a)` -- rotate around central point pc by angle a (rad, clockwise) in x-y plane.
  - `l.flatten()`,  `flatten(l)` -- project the multiline to x-y plane.

- MultiLine can be converted to a string and back
  (and thus used inside Opt class). String representation is a
  JSON array with zero or more lines.

\ref MultiLine "Class reference..."

-----------------
## LineWalker class

Class for walking alone a line (2D).

- Constructor: `LineWalker lw(dline)`.
- Other methods:
  - `lw.length()` -- get line length,
  - `lw.pt()`     -- get current point,
  - `lw.dist()`   -- get current distance from the line beginning,
  - `lw.tang()`   -- get unit tangent vector at current point,
  - `lw.norm()`   -- get unit normal vector at current point,
  - `lw.get_points(d)` -- get part of line with `d` length, starting from current point, move current point by `d`,
  - `lw.move_begin()` --  move current point to the first node,
  - `lw.move_end()` -- move current point to the last node,
  - `lw.move_frw(d)` -- move current point forward by `d` distance,
  - `lw.move_bck(d)` -- move current point backward by `d` distance,
  - `lw.move_frw_to_node()` - move current point forward to the nearest node,
  - `lw.move_bck_to_node()` - move current point backward to the nearest node,
  - `lw.is_begin()` -- is current point at the first node,
  - `lw.is_end()`   -- is current point at the last node.

\ref LineWalker "Class reference..."

-----------------
## ConvBase class

Trivial point transformation with two factors for scaling before
and after the transformation.. Children can redefine frw_pt() and bck_pt()
methods to build more complicated transformations.

Note that in some cases forward and backward conversions are different
(accuracy is always calculated in source units).

- `ConvBase()` -- Constructor.

- `frw_pt(dPoint &), bck_pt(dPoint &)` -- Functions to be redefined in children,
   forward and backwart in-place point conversion. By default is is
   just a rescaling with `rescale_src*rescale_dst` factor.

- `frw(dPoint &), bck(dPoint &), frw(dLine &), bck(dLine &),
   frw(dMultiLine &), bck(MultidLine &)` -- Convert points
    (save as frw_pt, bck_pt), lines and multilines (without changing number of points).

- `dLine frw_acc(const dLine & l, double acc) const`
- `dLine bck_acc(const dLine & l, double acc) const` --
  Convert a line. Each segment can be divided to provide
  accuracy `<acc>` in source units (both for `frw_acc` and `bck_acc`).

- `dRect frw_acc(const dRect & R, double acc) const`,
- `dRect bck_acc(const dRect & R, double acc) const` --
  Convert a rectagle and return bounding box of resulting figure.
  Accuracy `<acc>` is measured in source units (both for frw_acc and bck_acc).

- `void rescale_src(const double s)`
- `void rescale_dst(const double s)` -- change scale factors applied before
  and after conversions. If childs do not do the scaling they should redefine
  this.

- TODO: convert angles, convert scales

\ref ConvBase "Class reference..."

-----------------
## ConvMulti class

Composite point transformation, child of ConvBase.

Methods (&cnv is a pointer to a ConvBase class or its child, frw is
a boolean flag for direction of the transformation `true` means forward):
- `ConvMulti()` -- empty (trivial transformation),
- `ConvMulti(&cnv1, &cnv2, frw1, frw2)` -- combine two transformations,
- `push_front(&cnv, frw)` -- add a transformation to the beginning of the list,
- `push_back(&cnv, frw)`  -- add a transformation to the end of the list.

\ref ConvMulti "Class reference..."

-----------------
## ConvAff class

2D affine transformation, child of ConvBase.
Works only with `x` and `y` coordinates.

Methods (map is a std::map(dPoint,dPoint)):
 - `ConvAff()` -- constructor, trivial transformation,
 - `ConvAff(map)` -- build a transformation using the map,
 - `reset()` -- reset to the trivial transformation,
 - `reset(map)` -- reset using the map,
 - `det()` -- forward conversion determinant,
 - `shift_src(p)` -- shift by vector `p` before the transformation,
 - `shift_dst(p)` -- shift by vector `p` after the transformation,
 - `rescale_src(kx,ky)` -- rescale `x` and `y` before thetransformation,
 - `rescale_dst(kx,ky)` -- rescale `x` and `y` after thetransformation,

\ref ConvTriv "Class reference..."

-----------------
## ConvGeo class

Geo transformation, libproj wrapper, child of ConvBase.

If point coordinate `z <= -1e7` then 2D transformation is done.
`rescale_src()` and `rescale_dst()` affects only `x` and `y` coordinates.

Constructor:
 - `ConvGeo(const std::string & src, const std::string & dst = "+datum=WGS84 +proj=lonlat");`

\ref ConvGeo "Class reference..."

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

\ref Cache "Class reference..."

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

\ref SizeCache "Class reference..."

-----------------
## IConv class

\ref ConvTriv "Class reference..."

-----------------
## TimeFmt group

In mapsoft unix time in ms is used.

Functions:
* `std::string time_utc_iso8601(const time_t t)` --
   Convert mapsoft time to UTC time represented as ISO 8601 string.
* `time_t parse_utc_iso8601(const std::string & str)` --
   Convert UTC time represented as ISO 8601 string (used in GPX) to a mapsoft time.
