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
## Point class

`Point<T> is a 2-d point with coordinates of arbitrary numerical type T.

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

\ref Line "Class reference..."

-----------------
## MultiLine class

\ref MultiLine "Class reference..."

-----------------
## ConvBase class

\ref ConvBase "Class reference..."

-----------------
## ConvTriv class

\ref ConvTriv "Class reference..."

