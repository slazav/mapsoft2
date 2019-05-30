## geo_nom -- Soviet nomenclature map names

This functions work with Soviet nomenclature map names.
No coordinate conversions are done, for original maps
coordinates are always in Pulkovo-1942 datum.

Name of 1:1'000'000 map consists of following parts:
- An optional "x" prefix for southern hemisphere.
- A letter "a" .. "u" which represents a 4-degree latitude zone,
  starting from equator.
- Optional delimiter "-".
- A number 01 .. 60 which represents a 6-degree latitude zone,
  starting from -180 degree. Number of digits is important.

For 1:500'000 map a delimiter "-" and a number 1 .. 4 is added.
It divides the 1:1'000'000 map into 2x2 parts, counted from west to east
and north to south.

For 1:200'000 map a delimiter "-" and a number 01 .. 24 is added.
It divides the 1:1'000'000 map into 6x6 parts, counted from west to east
and from north to south.

For 1:100'000 map a delimiter "-" and a number 001 .. 144 is added.
It divides the 1:1'000'000 map into 12x12 parts, counted from west to east
and from north to south.

For 1:50'000 map a delimiter "-" and a number 1 .. 4 is added to the
1:100'000 map name. It divides it into 4x4 parts, counted from west to
east and from north to south.

Examples: "n36-001", "xa-12-01", "j42-1", "k43".

For maps "p" .. "s" (60 to 76 degrees of latitude) maps are grouped by two in
east-west direction:
- 1:1'000'000 maps are written as "p43,44" (always starting from even number).
- 1:500'000 maps are written as "p43-1,2" or "p43-3,4".
- 1:200'000 maps are written as "p43-01,02", "p43-03,04", etc..
- 1:100'000 maps are written as "p43-001,002", "p43-003,004", etc.
- 1:50'000 maps are written as "p43-001-1,2", or "p43-001-3,4".

For maps "t" .. "u" (76 to 84 degrees of latitude) maps are grouped by four in
east-west direction:

- 1:1'000'000 maps are written as "p43,44,45,46" (first number is multiple of 4 plus 1).
- 1:500'000 maps are written as "p43-1,2,p44-1,2".
- 1:200'000 maps are written as "p43-01,02,03,04".
- 1:100'000 maps are written as "p43-001,002,003,004".
- 1:50'000 maps are written as "p43-001-1,2,p44-001-1.2".

These functions can work with or without such grouping. Also an "extended" map
names with suffix `".<N>x<M>"` can be used, to group NxM maps together.

Example: "R36-01.2x2", "t01-001-1.10x10"

Map names can be seen here: https://nakarte.me/#m=4/56.19448/34.05762&l=O/Ng

-------
## Functions

```c++
dRect nom_to_range(const std::string & name, nom_scale_t & scale, bool ex = false);
```

-- Returns region and set scale for a given map name.

Use `ex=true` to enable extended regime:
- single sheets are allowed (T10-001 etc.)
- for a single sheets suffix `".<N>x<M>"` is allowed to multiply the range.

Examples:
T10-001  R36-01.2x2


```c++
std::string pt_to_nom(dPoint p, const nom_scale_t scale, const bool single=false);
```

-- Find map name for a given point (coordinates are Pulkovo-1942 datum)
Returns empty string if coordinates or scale is bad. If `<single>`
parameter is true, return a single sheet, like `T32-002` instead of
`T32-001,002,003,004`.


```c++
std::string nom_shift(const std::string & name, const iPoint & shift, const bool single=false);
```

-- Return nomenclature name of neighbor (defined by shift).
If there are multiple neighbors then west one is returned. If
`single==true` then read name in extended mode and return single sheet
names.

```c++
std::set<std::string> range_to_nomlist(const dRect & range,
           const nom_scale_t scale, const bool single=false);
```

Return all nomenclature names for a region. If range boundary coinsides
with the sheet boundary a minimum range is returned.
`range_to_nomlist(nom_to_range(name, sc),sc)` must return only the original name.

