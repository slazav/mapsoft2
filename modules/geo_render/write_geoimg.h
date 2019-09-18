#ifndef WRITE_GEOIMG_H
#define WRITE_GEOIMG_H

#include <string>
#include "opt/opt.h"
#include "geo_data/geo_data.h"
#include "getopt/getopt.h"

// add options for saving images and map files (including --map)
void ms2opt_add_writegeoimg(ext_option_list & opts);

// Write geodata to a raster file. Return 0 if file was successfully saved,
// 1 if output format can not be determined, throw Err on other errors.
// Interface is similat to `write_geo` in modules/geo_data
bool write_geoimg(const std::string & ofile, GeoData & data, const Opt & opt);

#endif
