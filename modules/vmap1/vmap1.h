#ifndef VMAP1_H
#define VMAP1_H

#include <iostream>
#include <vector>
#include <string>
#include <list>

#include "geom/multiline.h"
#include "opt/opt.h"


// Mapsoft-1 VMAP format

/*****************************************************************/

// Label position
struct VMap1Lab {
  int    dir;   // 0: left, 1: center, 2: right
  double ang;   // angle, degrees
  int  fsize;   // Font size. Before connection to an object it is
                // an absolute fig size.
                // After connection it is a correction, 0, -1, +2 etc.
  dPoint pos;
  bool   hor; // horizontal text (angle is not modified when the map rotates)
  bool operator< (const VMap1Lab & o) const;
  VMap1Lab();
};

// Separate label.
// Used when label is detached from an object
// Additional fields: Reference point (nearest object point)
// and label text.
struct VMap1Lfull : public VMap1Lab { // for internal use
  dPoint ref;
  std::string text;
  bool operator< (const VMap1Lfull & o) const;
};

typedef enum{
  VMAP1_POI = 0,
  VMAP1_POLYLINE = 1,
  VMAP1_POLYGON = 2
} object_class;

struct VMap1Obj: public dMultiLine {
  int             type;  // MP type
  int             dir;   // direction from mp, arrows from fig
  std::string     text;  // label text
  std::list<VMap1Lab> labels;
  Opt                      opts; // some Key=Value fields
  std::vector<std::string> comm; // comments

  object_class get_class() const;
  bool operator< (const VMap1Obj & o) const;
  VMap1Obj();
};

struct VMap1 : std::list<VMap1Obj> {
  int         mp_id;
  std::string name;
  std::string style;
  double      rscale;
  dLine       brd;
  std::list<VMap1Lfull> lbuf; // buffer for ownerless labels

  VMap1();

  dRect range() const;

  // Add objects and lbuf from vmap W, set style etc from W
  void add(const VMap1 & W);
};

// Reading and writitng native format
VMap1 read_vmap1(std::istream & IN);
int write_vmap1(std::ostream & OUT, const VMap1 & W);

#endif
