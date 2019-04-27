#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include "io_gpx.h"
#include "time_fmt/time_fmt.h"
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

using namespace std;

/**
 libxml2 manual: http://www.xmlsoft.org/html/index.html
 xmlreader module: http://www.xmlsoft.org/html/libxml-xmlreader.html
 xmlwriter module: http://www.xmlsoft.org/html/libxml-xmlwriter.html
 GPX 1.1 format: http://www.topografix.com/GPX/1/1/
 GPX 1.0 format: http://www.topografix.com/gpx_manual.asp
 example with all fields: https://github.com/tkrajina/gpxgo/tree/master/test_files

GPX 1.1 Structure:
<gpx version creator>
  <metadata>
    <name> <desc> <author> <copyright> <link> <time> <keywords> <bounds> <extensions>
  <wpt lat lon>
    <ele> <time> <magvar> <geoidheight>
    <name> <cmt> <desc> <src> <link> <sym> <type>
    <fix> <sat> <hdop> <vdop> <pdop> <ageofdgpsdata>
    <dgpsid> <extensions>

  <rte>
    <name> <cmt> <desc> <src> <link> <number> <type> <extensions>
    <rtept> [0..*] -- same as <wpt>

  <trk>
    <name> <cmt> <desc> <src> <link> <number> <type> <extensions> -- same as in rte
    <trkseg> [0..*]
      <trkpt> [0..*] -- same as <wpt>
      <extensions>
  <extensions>

GPX 1.0 Structure:
<gpx version creator>
  <name> <desc> <author> <email> <url> <urlname> <time> <keywords> <bounds>
  <wpt lat lon>
    <ele> <time> <magvar> <geoidheight>
    <name> <cmt> <desc> <src> <url> <urlname> <sym> <type>
    <fix> <sat> <hdop> <vdop> <pdop> <ageofdgpsdata>
    <dgpsid>

  <rte>
    <name> <cmt> <desc> <src> <url> <urlname> <number>
    <rtept> [0..*] -- same as <wpt>

  <trk>
    <name> <cmt> <desc> <src> <url> <urlname> <number> -- same as in rte
    <trkseg> [0..*]
      <trkpt> [0..*] -- same as <wpt>


-----------------
<wpt> fields:

Attributes: lat, lon -- The latitude and longitude of the point. Decimal
degrees, WGS84 datum. (x and y fields in the mapsoft GeoWpt structure).
Two mandatory fields.

* ele -- Elevation (in meters) of the point. (z field in the mapsoft
  GeoWpt structure).
* time -- Creation/modification timestamp for element. Date and time in
  are in Univeral Coordinated Time (UTC), not local time! Conforms to ISO
  8601 specification for date/time representation. Fractional seconds are
  allowed for millisecond timing in tracklogs. (opt["time"] in the mapsoft
  GeoWpt structure).

All other elements are stored in the Opt object and handeled as strings.

* magvar -- Magnetic variation (in degrees) at the point.
* geoidheight -- Height (in meters) of geoid (mean sea level) above WGS84
  earth ellipsoid. As defined in NMEA GGA message.
* name -- The GPS name of the waypoint. This field will be transferred to and from
  the GPS. GPX does not place restrictions on the length of this field or
  the characters contained in it. It is up to the receiving application to
  validate the field before sending it to the GPS.
* cmt -- GPS waypoint comment. Sent to GPS as comment. In mapsoft2 field name is "comm".
* desc -- A text description of the element. Holds additional information
  about the element intended for the user, not the GPS.
* src -- Source of data. Included to give user some idea of reliability
  and accuracy of data. "Garmin eTrex", "USGS quad Boston North", e.g.
* link -- Link to additional information about the waypoint.
* sym -- Text of GPS symbol name. For interchange with other programs,
  use the exact spelling of the symbol as displayed on the GPS. If the GPS
  abbreviates words, spell them out.
* type -- Type (classification) of the waypoint.
* fix -- Type of GPX fix. (none, 2d, 3d, dgps, pps)
* sat -- Number of satellites used to calculate the GPX fix.
* hdop -- Horizontal dilution of precision.
* vdop -- Vertical dilution of precision.
* pdop -- Position dilution of precision.
* ageofdgpsdata -- Number of seconds since last DGPS update.
* dgpsid -- ID of DGPS station used in differential correction.
* extensions -- You can add extend GPX by adding your own elements from
  another schema here.

    Rte/Trk attributes:

* name -- GPS name of route.
* cmt -- GPS comment for route. In mapsoft2 field name is "comm".
* desc -- Text description of route for user. Not sent to GPS.
* src -- Source of data. Included to give user some idea of reliability
  and accuracy of data.
* link -- Links to external information about the route.
* number -- GPS route number.
* type -- Type (classification) of route.
* extensions -- You can add extend GPX by adding your own elements from
  another schema here.

-----------------
    support in Mapsoft:

reading:
- skip gpx attributes, <metadata> and all <extensions>
- rte -- load as waypoint list, everything except extensions (TODO: test!)
- wpt: everything except extensions
- trk: everything except extensions
- - trkseg: everything except extensions
- - - trkpt: only <lat> <lon> <ele> <time> tags

writing:
- skip gpx attributes, <metadata> and all <extensions>
- wpt - all waypoint lists are written together.
        everithing except extensions
- trk - everithing except extensions
- - trkseg: everything except extensions
- - - trkpt: only <lat> <lon> <ele> <time> tags
*/


// All names which are keps in opt, handeled as strings
// and have same spelling in GPX and in mapsoft.
// comm/cmt, time, ele/z are processed separately.
const char *gps_wpt_names[] = {
  "magvar", "geoidheight", "name", "desc", "src", "link", "sym", "type", "fix",
  "sat", "hdop", "vdop", "pdop", "ageofdgpsdata", "dgpsid", NULL};

/// Trk/rte fields.
/// comm/cmt processed separately.
const char *gps_trk_names[] = {
  "name", "desc", "src", "link",
  "number", "type", NULL};

/********************************************************************/
/// Write GPX.
/// options:
///   xml_compr:   compress the output? 0|1, default 0;
///   xml_indent:  use indentation? 0|1, default 1;
///   xml_ind_str: indentation string, default "  ";
///   xml_qchar:   quoting character for attributes, default \'
///   gpx_write_rte: write waypoint lists as routes, 0|1, default 0
void
write_gpx (const char* filename, const GeoData & data, const Opt & opts){

  LIBXML_TEST_VERSION

  // create XML writer
  xmlTextWriterPtr writer =
    xmlNewTextWriterFilename(filename, opts.get<int>("xml_compr", 0));
  if (writer == NULL)
    throw Err() << "write_gpx: can't write to file: " << filename;

  try {
    // set some parameters
    int indent = opts.get<int>("xml_indent", 1);
    char qchar = opts.get<char>("xml_qchar", '\'');
    xmlChar *ind_str = (xmlChar *)opts.get<std::string>("xml_ind_str", "  ").c_str();

    if (xmlTextWriterSetIndent(writer, indent)<0 ||
        xmlTextWriterSetIndentString(writer, ind_str)<0 ||
        xmlTextWriterSetQuoteChar(writer, qchar)<0)
      throw "setting xml writer parameters";

    // start XML document
    if (xmlTextWriterStartDocument(writer, "1.0", "UTF-8", NULL)<0)
      throw "starting the xml document";

    // start GPX element.
    // BAD_CAST converts (const char*) to BAD_CAST 
    if (xmlTextWriterStartElement(writer, BAD_CAST "gpx")<0 ||
        xmlTextWriterWriteAttribute(writer,
          BAD_CAST "version", BAD_CAST "1.1")<0 ||
        xmlTextWriterWriteAttribute(writer,
          BAD_CAST "creator", BAD_CAST "mapsoft-2")<0)
      throw "starting <gpx> element";

    int use_rte = opts.get<int>("gpx_write_rte", 0);

    // Writing waypoints:
    for (int i = 0; i < data.wpts.size(); i++) {
      if (use_rte){
        if (xmlTextWriterStartElement(writer, BAD_CAST "rte")<0)
          throw "starting <rte> element";

        // other option elements:
        for (const char **fn = gps_trk_names; *fn!=NULL; fn++){
          if (!data.wpts[i].opts.exists(*fn)) continue;
          if (xmlTextWriterWriteFormatElement(writer,
             BAD_CAST *fn, "%s",
                 data.wpts[i].opts.get<string>(*fn).c_str())<0)
            throw "writing element";
        }
      }
      for (GeoWptList::const_iterator wp = data.wpts[i].begin();
                                      wp != data.wpts[i].end(); ++wp) {

        const xmlChar* pt = BAD_CAST (use_rte? "rtept" : "wpt");
        if (xmlTextWriterStartElement(writer, pt)<0 ||
            xmlTextWriterWriteFormatAttribute(writer,
              BAD_CAST "lat", "%.7f", wp->y)<0 ||
            xmlTextWriterWriteFormatAttribute(writer,
              BAD_CAST "lon", "%.7f", wp->x)<0)
          throw "starting <wpt> element";

        // altitude
        if (wp->have_alt() &&
            xmlTextWriterWriteFormatElement(writer,
              BAD_CAST "ele", "%.2f", wp->z)<0)
          throw "writing <ele> element";

        // time
        if (wp->opts.exists("time") &&
            xmlTextWriterWriteFormatElement(writer,
              BAD_CAST "time", "%s",
                 write_utc_iso_time(wp->opts.get<time_t>("time")).c_str())<0)
          throw "writing <time> element";

        // cmt
        if (wp->opts.exists("comm") &&
            xmlTextWriterWriteFormatElement(writer,
              BAD_CAST "cmt", "%s",
                 wp->opts.get<string>("comm").c_str())<0)
          throw "writing <cmt> element";

        // other option elements:
        for (const char **fn = gps_wpt_names; *fn!=NULL; fn++){
          if (!wp->opts.exists(*fn)) continue;
          if (xmlTextWriterWriteFormatElement(writer,
             BAD_CAST *fn, "%s",
                 wp->opts.get<string>(*fn).c_str())<0)
            throw "writing element";
        }

        if (xmlTextWriterEndElement(writer) < 0)
          throw "closing <wpt> element";
      }
      if (use_rte){
        if (xmlTextWriterEndElement(writer)<0)
          throw "closing <rte> element";
      }
    }


    // Write tracks:
    for (int i = 0; i < data.trks.size(); ++i) {

      if (xmlTextWriterStartElement(writer, BAD_CAST "trk")<0)
          throw "starting <trk> element";

      // other option elements:
      for (const char **fn = gps_trk_names; *fn!=NULL; fn++){
        if (!data.trks[i].opts.exists(*fn)) continue;
        if (xmlTextWriterWriteFormatElement(writer,
           BAD_CAST *fn, "%s",
               data.trks[i].opts.get<string>(*fn).c_str())<0)
          throw "writing element";
      }

      for (GeoTrk::const_iterator tp = data.trks[i].begin(); tp != data.trks[i].end(); ++tp) {

        // close <trkseg> if needed
        if (tp->start && tp != data.trks[i].begin() &&
           xmlTextWriterEndElement(writer) < 0)
             throw "closing <trkseg> element";

        // open <trkseg> if needed
        if ((tp->start || tp == data.trks[i].begin()) &&
           xmlTextWriterStartElement(writer, BAD_CAST "trkseg") < 0)
             throw "starting <trkseg> element";

        // open <trkpt>
        if (xmlTextWriterStartElement(writer, BAD_CAST "trkpt")<0 ||
            xmlTextWriterWriteFormatAttribute(writer,
              BAD_CAST "lat", "%.7f", tp->y)<0 ||
            xmlTextWriterWriteFormatAttribute(writer,
              BAD_CAST "lon", "%.7f", tp->x)<0)
          throw "starting <trkpt> element";

        // altitude
        if (tp->have_alt() &&
            xmlTextWriterWriteFormatElement(writer,
              BAD_CAST "ele", "%.2f", tp->z)<0)
          throw "writing <ele> element";

        // time
        if (tp->t && xmlTextWriterWriteFormatElement(writer,
              BAD_CAST "time", "%s",
                 write_utc_iso_time(tp->t).c_str())<0)
          throw "writing <time> element";


        if (xmlTextWriterEndElement(writer) < 0)
          throw "closing <trkpt> element";
      }
      if (xmlTextWriterEndElement(writer) < 0)
          throw "closing <trkpt> element";

      if (xmlTextWriterEndElement(writer) < 0)
          throw "closing <trk> element";
    }

    if (xmlTextWriterEndDocument(writer) < 0)
      throw "closing xml document";
  }
  catch (const char *c){
    xmlFreeTextWriter(writer);
    throw Err() << "write_gpx: error in " << c;
  }

  // free resources
  xmlFreeTextWriter(writer);

  return;
}

/********************************************************************/
#define TYPE_ELEM      1
#define TYPE_ELEM_END 15
#define TYPE_TEXT      3
#define TYPE_SWS      14

#define NAMECMP(x) (xmlStrcasecmp(name,(const xmlChar *)x)==0)
#define GETATTR(x) (const char *)xmlTextReaderGetAttribute(reader, (const xmlChar *)x)
#define GETVAL     (const char *)xmlTextReaderConstValue(reader)

// read <extensions>. In gpx/track/rte/wpt/trkseg
int
read_ext_node(xmlTextReaderPtr reader, Opt & opts){
  std::string state = "";
  while(1){
    int ret =xmlTextReaderRead(reader);
    if (ret != 1) return ret;

    const xmlChar *name = xmlTextReaderConstName(reader);
    int type = xmlTextReaderNodeType(reader);

    if (type == TYPE_SWS) continue;

    else if (type == TYPE_ELEM_END) {
      if (NAMECMP("extensions")) break;
    }

  }
  return 1;
}


// read waypoint / rte point
int
read_wpt_node(xmlTextReaderPtr reader, GeoWptList & data){
  GeoWpt wpt;
  wpt.y = atof(GETATTR("lat"));
  wpt.x = atof(GETATTR("lon"));
  std::string state="";
  while(1){
    int ret =xmlTextReaderRead(reader);
    if (ret != 1) return ret;

    const xmlChar *name = xmlTextReaderConstName(reader);
    int type = xmlTextReaderNodeType(reader);

    if (type == TYPE_SWS) continue;

    else if (NAMECMP("extensions") && (type == TYPE_ELEM)){
      cerr << "Warning: skip <extensions> in <wpt>/<rtept>\n";
      ret = read_ext_node(reader, data.opts);
      if (ret != 1) return ret;
    }

    else if (type == TYPE_ELEM) {
      state = "";
      if (NAMECMP("ele"))  state = "ele";
      if (NAMECMP("time")) state = "time";
      if (NAMECMP("cmt"))  state = "comm";
      for (const char **fn = gps_wpt_names; *fn!=NULL; fn++){
        if (NAMECMP(*fn)) state = *fn;
      }
      if (state == "")
         cerr << "Warning: Unknown node \"" << name << "\" in wpt (type: " << type << ")\n";
    }
    else if (type == TYPE_ELEM_END) {
      if (NAMECMP("ele") || NAMECMP("time") || NAMECMP("cmt")) state = "";
      for (const char **fn = gps_wpt_names; *fn!=NULL; fn++){
        if (NAMECMP(*fn)) state = "";
      }
      if (NAMECMP("wpt") || NAMECMP("rtept")) break;
      if (state != "")
         cerr << "Warning: Unknown node \"" << name << "\" in wpt (type: " << type << ")\n";
    }

    else if (type == TYPE_TEXT){
      if (state == "ele")  wpt.z = atof(GETVAL);
      if (state == "comm") wpt.opts.put<std::string>("comm", GETVAL);
      if (state == "time") wpt.opts.put<time_t>("time", parse_utc_time(GETVAL));
      for (const char **fn = gps_wpt_names; *fn!=NULL; fn++){
        if (state == *fn) wpt.opts.put<std::string>(*fn, GETVAL);
      }
    }
    else {
      cerr << "Warning: Unknown node \"" << name << "\" in wpt (type: " << type << ")\n";
    }
  }
  data.push_back(wpt);
  return 1;
}

int
read_trkpt_node(xmlTextReaderPtr reader, GeoTrk & trk, bool start){
  GeoTpt pt;
  bool is_ele = false, is_time=false;

  pt.y = atof(GETATTR("lat"));
  pt.x = atof(GETATTR("lon"));

  while(1){
    int ret =xmlTextReaderRead(reader);
    if (ret != 1) return ret;

    const xmlChar *name = xmlTextReaderConstName(reader);
    int type = xmlTextReaderNodeType(reader);

    if (type == TYPE_SWS) continue;

    else if (NAMECMP("extensions") && (type == TYPE_ELEM)){
      Opt o;
      cerr << "Warning: skip <extensions> in <trkpt>\n";
      ret = read_ext_node(reader, o);
      if (ret != 1) return ret;
    }

    else if (NAMECMP("ele")){
      if (type == TYPE_ELEM) is_ele = true;
      if (type == TYPE_ELEM_END) is_ele = false;
    }
    else if (NAMECMP("time")){
      if (type == TYPE_ELEM) is_time = true;
      if (type == TYPE_ELEM_END) is_time = false;
    }
    else if (type == TYPE_TEXT){
      if (is_ele)
        pt.z = atof(GETVAL);
      if (is_time)
        pt.t = parse_utc_time(GETVAL);
    }

    else if (NAMECMP("trkpt") && (type == TYPE_ELEM_END)){
      break;
    }
    else {
      cerr << "Warning: Unknown node \"" << name << "\" in trkpt (type: " << type << ")\n";
    }
  }
  pt.start = start;
  trk.push_back(pt);
  return 1;
}


int
read_trkseg_node(xmlTextReaderPtr reader, GeoTrk & trk){
  bool start=true;
  while(1){
    int ret =xmlTextReaderRead(reader);
    if (ret != 1) return ret;

    const xmlChar *name = xmlTextReaderConstName(reader);
    int type = xmlTextReaderNodeType(reader);

    if (type == TYPE_SWS) continue;

    else if (NAMECMP("extensions") && (type == TYPE_ELEM)){
      Opt o;
      cerr << "Warning: skip <extensions> in <trkseg>\n";
      ret = read_ext_node(reader, o);
      if (ret != 1) return ret;
    }

    else if (NAMECMP("trkpt") && (type == TYPE_ELEM)){
      ret = read_trkpt_node(reader, trk, start);
      if (ret != 1) return ret;
      start=false;
    }

    else if (NAMECMP("trkseg") && (type == TYPE_ELEM_END)){
      break;
    }

    else {
      cerr << "Warning: Unknown node \"" << name << "\" in trkseg (type: " << type << ")\n";
    }
  }
  return 1;
}

int
read_trk_node(xmlTextReaderPtr reader, GeoData & data){
  GeoTrk trk;
  std::string state;
  while(1){
    int ret =xmlTextReaderRead(reader);
    if (ret != 1) return ret;

    const xmlChar *name = xmlTextReaderConstName(reader);
    int type = xmlTextReaderNodeType(reader);

    if (type == TYPE_SWS) continue;

    else if (NAMECMP("extensions") && (type == TYPE_ELEM)){
      cerr << "Warning: skip <extensions> in <trk>\n";
      ret = read_ext_node(reader, trk.opts);
      if (ret != 1) return ret;
    }

    else if (NAMECMP("trkseg") && (type == TYPE_ELEM)){
      ret=read_trkseg_node(reader, trk);
      if (ret != 1) return ret;
    }

    else if (type == TYPE_ELEM) {
      state = "";
      if (NAMECMP("cmt"))  state = "comm";
      for (const char **fn = gps_trk_names; *fn!=NULL; fn++){
        if (NAMECMP(*fn)) state = *fn;
      }
      if (state == "")
         cerr << "Warning: Unknown node \"" << name << "\" in trk (type: " << type << ")\n";
    }

    else if (type == TYPE_ELEM_END) {
      if (NAMECMP("cmt")) state = "";
      for (const char **fn = gps_trk_names; *fn!=NULL; fn++){
        if (NAMECMP(*fn)) state = "";
      }
      if (NAMECMP("trk")) break;
      if (state != "")
         cerr << "Warning: Unknown node \"" << name << "\" in trk (type: " << type << ")\n";
    }

    else if (type == TYPE_TEXT){
      if (state == "comm") trk.opts.put<std::string>("comm", GETVAL);
      for (const char **fn = gps_trk_names; *fn!=NULL; fn++){
        if (state == *fn) trk.opts.put<std::string>(*fn, GETVAL);
      }
    }

    else {
      cerr << "Warning: Unknown node \"" << name << "\" in trk (type: " << type << ")\n";
    }
  }
  data.trks.push_back(trk);
  return 1;
}

int
read_rte_node(xmlTextReaderPtr reader, GeoData & data){
  GeoWptList wptl;
  std::string state;
  while(1){
    int ret =xmlTextReaderRead(reader);
    if (ret != 1) return ret;

    const xmlChar *name = xmlTextReaderConstName(reader);
    int type = xmlTextReaderNodeType(reader);

    if (type == TYPE_SWS) continue;

    else if (NAMECMP("extensions") && (type == TYPE_ELEM)){
      cerr << "Warning: skip <extensions> in <rte>\n";
      ret = read_ext_node(reader, wptl.opts);
      if (ret != 1) return ret;
    }

    // points are same as wpt
    else if (NAMECMP("rtept") && (type == TYPE_ELEM)){
      ret=read_wpt_node(reader, wptl);
      if (ret != 1) return ret;
    }

    // fields are same as in trk!
    else if (type == TYPE_ELEM) {
      state = "";
      if (NAMECMP("cmt"))  state = "comm";
      for (const char **fn = gps_trk_names; *fn!=NULL; fn++){
        if (NAMECMP(*fn)) state = *fn;
      }
      if (state == "")
         cerr << "Warning: Unknown node \"" << name << "\" in rte (type: " << type << ")\n";
    }

    else if (type == TYPE_ELEM_END) {
      if (NAMECMP("cmt")) state = "";
      for (const char **fn = gps_trk_names; *fn!=NULL; fn++){
        if (NAMECMP(*fn)) state = "";
      }
      if (NAMECMP("rte")) break;
      if (state != "")
         cerr << "Warning: Unknown node \"" << name << "\" in rte (type: " << type << ")\n";
    }

    else if (type == TYPE_TEXT){
      if (state == "comm") wptl.opts.put<std::string>("comm", GETVAL);
      for (const char **fn = gps_trk_names; *fn!=NULL; fn++){
        if (state == *fn) wptl.opts.put<std::string>(*fn, GETVAL);
      }
    }

    else {
      cerr << "Warning: Unknown node \"" << name << "\" in rte (type: " << type << ")\n";
    }
  }
  data.wpts.push_back(wptl);
  return 1;
}



int
read_gpx_node(xmlTextReaderPtr reader, GeoData & data){
  GeoWptList wptl;
  bool is_meta=false;
  while(1){
    int ret =xmlTextReaderRead(reader);
    if (ret != 1) return ret;

    const xmlChar *name = xmlTextReaderConstName(reader);
    int type = xmlTextReaderNodeType(reader);

    if (type == TYPE_SWS) continue;

    // skip metadata
    else if (NAMECMP("metadata")){
      if (type == TYPE_ELEM) is_meta=true;
      if (type == TYPE_ELEM_END) is_meta=false;
    }
    else if (is_meta) continue;

    // read wpt,trk,rte tags
    else if (NAMECMP("wpt") && (type == TYPE_ELEM)){
      ret=read_wpt_node(reader, wptl);
      if (ret != 1) return ret;
    }
    else if (NAMECMP("trk") && (type == TYPE_ELEM)){
      ret=read_trk_node(reader, data);
      if (ret != 1) return ret;
    }
    else if (NAMECMP("rte") && (type == TYPE_ELEM)){
      ret=read_rte_node(reader, data);
      if (ret != 1) return ret;
    }

    // skip extensions
    else if (NAMECMP("extensions") && (type == TYPE_ELEM)){
      cerr << "Warning: skip <extensions> in <gpx>\n";
      ret=read_ext_node(reader, data.opts);
      if (ret != 1) return ret;
    }
    else if (NAMECMP("gpx") && (type == TYPE_ELEM_END)){
      break;
    }
    else {
      cerr << "Warning: Unknown node \"" << name << "\" in gpx (type: " << type << ")\n";
    }
  }
  data.wpts.push_back(wptl);
  return 1;
}


void
read_gpx(const char* filename, GeoData & data, const Opt & opts) {

  LIBXML_TEST_VERSION

  xmlTextReaderPtr reader;
  int ret;

  reader = xmlReaderForFile(filename, NULL, 0);
  if (reader == NULL){
    xmlFreeTextReader(reader);
    throw Err() << "Can't open GPX file " << filename;
  }

  // parse file
  while (1){
    ret = xmlTextReaderRead(reader);
    if (ret!=1) break;

    const xmlChar *name = xmlTextReaderConstName(reader);
    int type = xmlTextReaderNodeType(reader);
    if (NAMECMP("gpx") && (type == TYPE_ELEM))
      ret = read_gpx_node(reader, data);
    if (ret!=1) break;
  }

  // free resources
  xmlFreeTextReader(reader);

  if (ret != 0) throw Err() << "Can't parse GPX file " << filename;

}

