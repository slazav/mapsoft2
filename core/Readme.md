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

- Opt object can be converted to a string and back (and thus used inside Opt).
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

`Point<T>` is a 2-d point with coordinates of arbitrary numerical type T.

- Constructors:
  - `Point()` -- point with zero coordinates,
  - `Point(x,y)` -- point with coordinates x and y,
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
  - `pscal(p1,p2)` -- scalar product: p1.x*p2.x + p1.y*p2.y,
  - `dist(p1,p2)` -- distance between points: (p1-p2).len().
```

- Point can be converted to a string and back
  (and thus used inside Opt). String representation is a
  JSON array with two numerical fields (x,y).

\ref Point "Class reference..."

-----------------
## Rect class

Rect<T> is a 2-d rectangle with coordinates of arbitrary numerical type T.
Rectangle is defined by top-left corner coordinates `(x,y)`, width `w`,
height `h` and empty flag `e`.

- Coordinates are counted from top-left.
  Top-left corner has lowest coordinates, bottom-right corner
  has highest coordinates. Width and height are always non-negative.

- In any range checks all sides are included in the rectangle.

- There is a difference between empty rectangle (such as a
  bounding box of a zero-point line) and zero-size rectangle
  (bounding box of a one-point line). Many functions throw
  error if rectangle is empty.

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
  - `r.tcl(), r.trc(), r.brc(), r.blc(), r.cnt()` -- top-left, top-right,
    bottom-left, bottom-left corners and central point,
  - `dRect(r)`, `iRect(r)` -- cast to double- or integer-coordinate rectangle
  - `r.rint()`, `rint(r)` -- set coordinates to nearest integer values,
  - `r.floor()`, `floor(r)` -- shrink the rectangle to nearest integers,
  - `r.ceil()`, `ceil(r)` -- expand the rectangle to nearest integers,
  - `r.pump(k)`, `pump(r,k)` -- pump rectangle to each side by k value,
  - `r.pump(kx,ky)`, `pump(r,kx,ky)` -- pump by kx and ky in x and y directions,
  - `r.expand(p)`, `expand(r,p)` -- expand rectangle to cover point p,
  - `r1.expand(r2)`, `expand(r1,r2)` -- expand rectangle to cover rectangle r2,
  - `r1.intersect(r2)`, `intersect(r1,r2)` -- intersection with rectangle r2,
  - `r.contains(p)`, `contains(r,p)` -- check if rectangle contains a point,
  - `r1.contains(r2)`, `contains(r1,r2)` -- check if rectangle contains another rectangle,
`
- Rect can be converted to a string and back
  (and thus used inside Opt). String representation is a
  JSON array with four numerical fields (x,y,w,h) or empty array.

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
  - `l.length`, `length(l)` -- line length
  - `l.invert`, `invert(l)` -- invert length
  - `l1.is_shifted(l2, sh)`, `is_shifted(l1, l2, sh)` -- check if line l2 is a shifted version of l1, return the shift
  - `l.bbox()`, `bbox(l)` -- return a bounding box (Rect object),
  - `l.rint()`, `rint(l)` -- set coordinates to nearest integer values,
  - `rect_to_line(r)` -- convert a rectangle to line
`
- Line can be converted to a string and back
  (and thus used inside Opt). String representation is a
  JSON array with zero or more points (example: "[[1,2],[3,4]]").

\ref Line "Class reference..."

-----------------
## MultiLine class

Line with multiple segments (std::vector of Line).

- Constructors:
  - `MiltiLine()` -- empty line,
  - `MiltiLine(string)` -- read from std::string (see below)

- Typedefs:
  - `dMultiLine` is a `MultiLine<double>`,
  - `iMultiLine` is a `MultiLine<int>`.

- Arithmetic operations (`l` are MultiLines, `p` points, `k` numbers):
  - `l+=p, l-=p, l+p, p+l, l-p, -l`
  - `l*=k, l/=k, l*k, k*l, l/k`
  - `l==l, l!=l`
  - `l<l, l<=l, l>=l, l>l`

- Other operations:
  - `dMultiLine(l)`, `iMultiLine(l)` -- cast to double- or integer-coordinate MultiLine
  - `l.length`, `length(l)` -- line length (sum of segments' lengths)
  - `l.bbox()`, `bbox(l)` -- return a bounding box (Rect object),
  - `l.rint()`, `rint(l)` -- set coordinates to nearest integer values,
`
- MultiLine can be converted to a string and back
  (and thus used inside Opt). String representation is a
  JSON array with zero or more lines.

\ref MultiLine "Class reference..."

-----------------
## ConvBase class

\ref ConvBase "Class reference..."

-----------------
## ConvTriv class

\ref ConvTriv "Class reference..."

-----------------
## ConvMulti class

\ref ConvMulti "Class reference..."

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


