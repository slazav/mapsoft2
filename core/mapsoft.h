#ifndef MAPSOFT_H
#define MAPSOFT_H

// error handling
#include <err/err.h>

// mapsoft options
#include <opt/opt.h>
#include <getopt/getopt.h>

// 2D points, lines, rectangles
#include <2d/point.h>
#include <2d/line.h>
#include <2d/line_walker.h>
#include <2d/multiline.h>

// 2D conversions
#include <conv/conv_base.h>
#include <conv/conv_triv.h>
#include <conv/conv_multi.h>
#include <conv/conv_geo.h>
#include <conv/conv_aff.h>

// cache
#include <cache/cache.h>
#include <cache/sizecache.h>

// wrapper for iconv library
#include <iconv/iconv.h>


// reading/writing OziExplorer files
#include <ozi/ozi.h>



#endif