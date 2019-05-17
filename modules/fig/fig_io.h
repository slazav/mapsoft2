#ifndef FIG_IO_H
#define FIG_IO_H

#include <iostream>
#include "fig_data.h"
#include "opt/opt.h"

/// Options:
/// * fig_enc=<enc> -- encoding for reading and writing files.
///   Default value = KOI8-R

/// Read FIG-file <filename> to <data>. If somethind already exists
/// in <data> then objects are appended and the header is overwrited.
/// Non-standard colors are converted to "long" values (> 0x1000000)
/// Text is converted to UTF8 encoding.
bool read_fig(const char* filename, Fig & data, const Opt & opts = Opt());

/// Write <data> to fig-file <filename>
/// Some tests are performed here:
/// - comments must be not more then 100 lines, 1022 chars each
bool write_fig(std::ostream & out, const fig_world & world, const Opt & opts = Opt());

bool write_fig(const std::string & file, const fig_world & world, const Opt & opts = Opt));

#endif
