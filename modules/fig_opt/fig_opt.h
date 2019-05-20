#ifndef FIG_OPT_H
#define FIG_OPT_H

#include <vector>
#include <string>

#include "fig/fig.h"
#include "opt/opt.h"

/// Extract options from vector<string>, add to opts.
void fig_get_opts(const std::vector<std::string> & comment, Opt & opts);

/// Extract options from Fig.
void fig_get_opts(const Fig & f, Opt & opts);

/// Extract options from FigObj.
void fig_get_opts(const FigObj & o, Opt & opts);



/// Remove options from vector<string>.
void fig_del_opts(std::vector<std::string> & comment);

/// Remove options from Fig.
void fig_del_opts(Fig & f);

/// Remove options from FigObj.
void fig_del_opts(FigObj & o);



/// Add options to vector<string>.
void fig_add_opts(std::vector<std::string> & comment, const Opt & opts);

/// Add options to Fig.
void fig_add_opts(Fig & f, const Opt & opts);

/// Add options to FigObj.
void fig_add_opts(FigObj & o, const Opt & opts);

#endif
