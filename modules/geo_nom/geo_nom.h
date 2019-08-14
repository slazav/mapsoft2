#ifndef GEO_NOM_H
#define GEO_NOM_H

#include "geom/rect.h"
#include <string>
#include <set>
#include <iostream>

typedef enum {
 SC_1M,
 SC_500k,
 SC_200k,
 SC_100k,
 SC_50k,
} nom_scale_t;

// input/output for nom_scale_t
std::ostream & operator<< (std::ostream & s, const nom_scale_t & sc);
std::istream & operator>> (std::istream & s, nom_scale_t & sc);

/*
Returns region and set scale for a given soviet nomenclature map.
Coordinates are in Pulkovo-1942 datum.
Returns empty dRect if name is bad.

Supported scales: 1:1000000, 1:200000, 1:500000, 1:100000, 1:50000.

Examples:
1:1000000: xA-10  A-10  A10  P-11,12  T-11,12,13,14
1:500000: xA-10  A10-1  P-10-1,2  T-11-1,2,T12-1,2
1:200000: xA-10-01  A10-01  P-10-01,02 T-10-01,02,03,04
1:100000: xA-10-001  A10-001  P-10-001,002  T-10-001,002,003,004
1:50000:  xA10-001-1  A10-001-1  P-10-001-1,2  T-11-001-3,4,T-12-001-3,4

Use ex=true to enable extended regime:
- single sheets are allowed (T10-001 etc.)
- for a single sheet suffix ".<N>x<M>" is allowed to multiply the range.

Examples:
T10-001  R36-01.2x2
*/
dRect nom_to_range(const std::string & name, nom_scale_t & scale, bool ex = false);


/*
Find soviet map nomenclature name for a given point.
Coordinates must be in Pulkovo-1942 datum
(p.x is longitude, p.y is latitude).
Returns empty string if coordinates or scale is bad.
If single is true, return a single sheet, like T32-002 instead of T32-001,002,003,004.
*/
std::string pt_to_nom(dPoint p, const nom_scale_t scale, const bool single=false);


/*
Return nomenclature name of neighbor (defined by shift).
If there are multiple neighbors then south-west one is returned.
If single==true then read name in extended mode and return single sheet names.
*/
std::string nom_shift(const std::string & name, const iPoint & shift, const bool single=false);


/*
Return nomenclature name list for a region. If range boundary coinsides
with sheet boundary a minimum range is returned.
`range_to_nomlist(nom_to_range(name, sc),sc)` includes a single sheet <name>.
*/
std::set<std::string> range_to_nomlist(const dRect & range,
           const nom_scale_t scale, const bool single=false);

/********************************************************************/
// Functions for working with cordinate prefixes and lon0 parameter
// in soviet map coordinates

// Get lon0 (-3,3,9,15...).
// Example: 0.25 -> 3
double lon2lon0(const double lon);

// Get prefix (1..60) for given longitude.
// Example 38.5 -> 7
int lon2pref(const double lon);

// Extract prefix from coordinate and return lon0.
// Example: 7800000 -> 39
double crdx2lon0(const double X);

// Extract non-prefix part.
// Example: 7800000 -> 800000
double crdx2nonpref(const double X);

/********************************************************************/


#endif
