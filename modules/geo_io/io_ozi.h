#ifndef IO_OZI_H
#define IO_OZI_H

#include <iostream>

#include "geo_data/geo_data.h"
#include "opt/opt.h"

/* Unpack Ozi-style CSV lines.
Format description (from http://www.oziexplorer3.com/eng/help/fileformats.html):
- Each field separated by a comma.
- Comma's not allowed in text fields, character 209 can be used instead
  and a comma will be substituted.
- Non essential fields need not be entered but comma separators must
  still be used (example ,,).
- Defaults will be used for empty fields.
- Any number of the last fields in a data line need not be included at
  all not even the commas.

 If (count>0) vector of this size is returned. Extra fields will be
 silently ignored, Missing fields are returned as empty strings.
*/
std::vector<std::string> unpack_ozi_csv(const std::string & str, unsigned int count=0);

/* Pack Ozi-style CSV lines.
   Commas will be substituted by character 209.
   Newline characters will be substituted by space.
   Empty fields in the end will be skipped.
*/
std::string pack_ozi_csv(const std::vector<std::string> & vec);

void read_ozi      (const char *fname, GeoData & data, const Opt & opt = Opt());
void write_ozi_plt (const char *fname, const GeoTrk & trk, const Opt & opt = Opt());
void write_ozi_wpt (const char *fname, const GeoWptList & wpt, const Opt & opt = Opt());
void write_ozi_map (const char *fname, const GeoMap & map, const Opt & opt = Opt());

#endif
