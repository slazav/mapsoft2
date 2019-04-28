#ifndef MAPSOFT_H
#define MAPSOFT_H

// error handling
#include <err/err.h>

// mapsoft options
#include <opt/opt.h>
#include <words/words.h>
#include <getopt/getopt.h>

// points, lines, rectangles
#include <geom/point.h>
#include <geom/line.h>
#include <geom/line_walker.h>
#include <geom/multiline.h>

// point conversions
#include <conv/conv_base.h>
#include <conv/conv_multi.h>
#include <conv/conv_geo.h>
#include <conv/conv_aff.h>

// cache
#include <cache/cache.h>
#include <cache/sizecache.h>

// wrapper for iconv library
#include <iconv/iconv.h>

// geodata
#include <geo/geo_data.h>
#include <geo/io_gpx.h>
#include <geo/io_kml.h>

// time formatting
#include <time_fmt/time_fmt.h>

// reading/writing OziExplorer files
#include <ozi/ozi.h>



#endif