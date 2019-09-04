#ifndef IMGSRC_H
#define IMGSRC_H

#include "geom/rect.h"

/// ImageSource interface -- abstract class for loading
/// raster images.
struct ImageSource{

    /// Image dimensions
    virtual iPoint size() const = 0;

    /// Get bits per pixed for data
    virtual int get_bpp() const = 0;

    /// Get data line number
    virtual int get_line() const = 0;


    /// Goto line n (n should be >= get_line())
    virtual void goto_line(const int n) = 0;

    /// Get data line
    virtual unsigned char* get_data() const = 0;

    /// Get color value (at x coordinate of the current line)
    /// (no range checking!)
    virtual unsigned int get_col(const int x) const = 0;

};

#endif