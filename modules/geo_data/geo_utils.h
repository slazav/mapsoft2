#ifndef GEO_LON0_H
#define GEO_LON0_H

/********************************************************************/
// Functions for working with cordinate prefixes and lon0 parameter
// in soviet map coordinates

// Get lon0 (-3,3,9,15...).
// Example: 0.25 -> 3
int lon2lon0(const double lon);

// Get prefix (1..60) for given longitude.
// Example 38.5 -> 7
int lon2pref(const double lon);

// Extract prefix from coordinate and return lon0.
// Example: 7800000 -> 39
int crdx2lon0(const double X);

// Extract non-prefix part.
// Example: 7800000 -> 800000
double crdx2nonpref(const double X);

std::string GEO_PROJ_SU(double lon);

/********************************************************************/

#endif
