#ifndef OZI_H
#define OZI_H

///\addtogroup libmapsoft
///@{
///\defgroup Ozi
///Read OziExplorer files
///@{

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

// See: http://www.oziexplorer3.com/eng/help/fileformats.html

/*******************************************************************/
/// Unpack Ozi-style CSV lines.
/**
## Format description (from http://www.oziexplorer3.com/eng/help/fileformats.html)
 * Each field separated by a comma.
 * Comma's not allowed in text fields, character 209 can be used instead and a comma will be substituted.
 * Non essential fields need not be entered but comma separators must still be used (example ,,).
 * Defaults will be used for empty fields.
 * Any number of the last fields in a data line need not be included at all not even the commas.

 If (count>0) vector of this size is returned. Extra fields will be silently ignored,
 Missing fields are returned as empty strings.
*/
std::vector<std::string> unpack_ozi_csv(const std::string & str, unsigned int count=0){
  int p1=0, p2=0;
  std::vector<std::string> ret;
  do {
    p2=str.find(',', p1);
    std::string field = str.substr(p1,p2-p1);
    std::replace( field.begin(), field.end(), (char)209, ',');
    ret.push_back(field);
    if (count && ret.size()==count) break;
    p1=p2+1;
  }
  while (p2!=std::string::npos);
  if (count) ret.resize(count);
  return ret;
}

/*******************************************************************/
/// Pack Ozi-style CSV lines.
/// Commas will be substituted by character 209.
/// Newline characters will be substituted by space.
/// Empty fields in the end will be skipped.
std::string pack_ozi_csv(const std::vector<std::string> & vec){
  std::string ret;

  // find last non-empty field
  size_t end=vec.size();
  while (end>0 && vec[end-1]=="") end--;

  for (size_t i=0; i!=end; ++i){
    std::string field = vec[i];
    std::replace( field.begin(), field.end(), ',', (char)209);
    std::replace( field.begin(), field.end(), '\n', ' ');
    ret += (i==0?"":",") + field;
  }
  return ret;
}

/*******************************************************************/
/// Read wpt file.
/**

## Waypoint File (.wpt) -- from http://www.oziexplorer3.com/eng/help/fileformats.html
Line 1 : File type and version information
Line 2 : Geodetic Datum used for the Lat/Lon positions for each waypoint
Line 3 : Reserved for future use
Line 4 : GPS Symbol set - not used yet

Waypoint data:
Field 1 : Number - for Lowrance/Eagles and Silva GPS receivers this is
  the storage location (slot) of the waypoint in the gps, must be unique.
  For other GPS receivers set this number to -1 (minus 1). For
  Lowrance/Eagles and Silva if the slot number is not known (new waypoints)
  set the number to -1.
Field 2 : Name - the waypoint name, use the correct length name to suit the GPS type.
Field 3 : Latitude - decimal degrees.
Field 4 : Longitude - decimal degrees.
Field 5 : Date - see Date Format below, if blank a preset date will be used
Field 6 : Symbol - 0 to number of symbols in GPS
Field 7 : Status - always set to 1
Field 8 : Map Display Format
Field 9 : Foreground Color (RGB value)
Field 10 : Background Color (RGB value)
Field 11 : Description (max 40), no commas
Field 12 : Pointer Direction
Field 13 : Garmin Display Format
Field 14 : Proximity Distance - 0 is off any other number is valid
Field 15 : Altitude - in feet (-777 if not valid)
Field 16 : Font Size - in points
Field 17 : Font Style - 0 is normal, 1 is bold.
Field 18 : Symbol Size - 17 is normal size
Field 19 : Proximity Symbol Position
Field 20 : Proximity Time
Field 21 : Proximity or Route or Both
Field 22 : File Attachment Name
Field 23 : Proximity File Attachment Name
Field 24 : Proximity Symbol Name
*/

/*******************************************************************/
/// Read EVT.
/**

## Event File (.evt) -- from http://www.oziexplorer3.com/eng/help/fileformats.html

Line 1 : File type and version information
Line 2 : Geodetic Datum used for the Lat/Lon positions for each event
Line 3 : Reserved for future use
Line 4 : Reserved for future use

Event data:
Field 1 : Number - this is the location in the array (max 1000), must be
  unique, usually start at 1 and increment.
Field 2 : Latitude - decimal degrees.
Field 3 : Longitude - decimal degrees.
Field 4 : Symbol - 0 to number of symbols in GPS
Field 5 : Map Display Format - not yet used, set to 0
Field 6 : Foreground Color (RGB value)
Field 7 : Background Color (RGB value)
Field 8 : Symbol Size - 17 is normal size
*/

/*******************************************************************/
/// Read PLT.
/**
## Track File (.plt) -- from http://www.oziexplorer3.com/eng/help/fileformats.html

Line 1 : File type and version information
Line 2 : Geodetic Datum used for the Lat/Lon positions for each trackpoint
Line 3 : "Altitude is in feet" - just a reminder that the altitude is always stored in feet
Line 4 : Reserved for future use
Line 5 : multiple fields as below
    Field 1 : always zero (0)
    Field 2 : width of track plot line on screen - 1 or 2 are usually the best
    Field 3 : track color (RGB)
    Field 4 : track description (no commas allowed)
    Field 5 : track skip value - reduces number of track points plotted, usually set to 1
    Field 6 : track type - 0 = normal , 10 = closed polygon , 20 = Alarm Zone
    Field 7 : track fill style - 0 =bsSolid; 1 =bsClear; 2 =bsBdiagonal; 3 =bsFdiagonal; 4 =bsCross;
    5 =bsDiagCross; 6 =bsHorizontal; 7 =bsVertical;
    Field 8 : track fill color (RGB)
Line 6 : Number of track points in the track, not used, the number of points is determined when reading the points file

Trackpoint data:
Field 1 : Latitude - decimal degrees.
Field 2 : Longitude - decimal degrees.
Field 3 : Code - 0 if normal, 1 if break in track line
Field 4 : Altitude in feet (-777 if not valid)
Field 5 : Date - see Date Format below, if blank a preset date will be used
Field 6 : Date as a string
Field 7 : Time as a string

Note that OziExplorer reads the Date/Time from field 5, the date and time in fields 6 & 7 are ignored.

Example
-27.350436, 153.055540,1,-777,36169.6307194, 09-Jan-99, 3:08:14
-27.348610, 153.055867,0,-777,36169.6307194, 09-Jan-99, 3:08:14 

Point File (.pnt)

Line 1 : File type and version information
Line 2 : Geodetic Datum used for the Lat/Lon positions for each point
Line 3 : Reserved for future use
Line 4 : Reserved for future use
Line 5 : multiple fields as below

    Field 1 : point set fore color (RGB)
    Field 2 : point set back color (RGB)
    Field 3 : point set size
    Field 4 : point set font size
    Field 5 : point set format
    Field 6 : point set style
    Field 7 : point set description (no commas)

Point data

    One line per point
    each field separated by a comma
    non essential fields need not be entered but comma separators must still be used (example ,,)
    defaults will be used for empty fields

Field 1 : Latitude - decimal degrees.
Field 2 : Longitude - decimal degrees.
Field 3 : rotation angle
Field 4 : point name
Field 5 : description 1 (no commas)
Field 6 : description 2 (no commas)
Field 7 : description 3 (no commas)

*/

#endif
