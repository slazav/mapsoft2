#include <vector>
#include <shapefil.h>

#include "err/err.h"
#include "geom/multiline.h"
#include "shape.h"

/**********************************************************/
// Implementation class
class Shape::Impl {
  public:
    int type;
    int num;
    std::shared_ptr<void> shp;
    std::shared_ptr<void> dbf;

    Impl(const char *fbase, bool create, int type);
    ~Impl() {}

    int put(const dMultiLine & ml);
    dMultiLine get(const int id);

    static int sh2type(int t);
    static int type2sh(int t);
};

/**********************************************************/
// Main class methods
Shape::Shape(const char *fbase, bool create, int type):
  impl(std::unique_ptr<Impl>(new Impl(fbase, create, type))) { }

int
Shape::get_type() const {return impl->type;}

int
Shape::get_num() const {return impl->num;}

int
Shape::put(const dMultiLine & ml) {return impl->put(ml);}

dMultiLine
Shape::get(const int id) {return impl->get(id);}

Shape::~Shape(){}

/**********************************************************/
// Implementation class methods

int
Shape::Impl::sh2type(int t){
  switch (t){
    case SHPT_POINT:   return MAP_POINT;
    case SHPT_ARC:     return MAP_LINE;
    case SHPT_POLYGON: return MAP_POLYGON;
    default: throw Err() << "Shape: unknown type: " << t;
  }
  return 0;
}

int
Shape::Impl::type2sh(int t){
  switch (t){
    case MAP_POINT:   return SHPT_POINT;
    case MAP_LINE:    return SHPT_ARC;
    case MAP_POLYGON: return SHPT_POLYGON;
    default: throw Err() << "Shape: unknown type: " << t;
  }
  return 0;
}


Shape::Impl::Impl(const char *fbase, bool create, int t){
  type = t;
  num = 0;
  if (create) {
    shp = std::shared_ptr<void>(SHPCreate(fbase, type2sh(t)), SHPClose);
    if (shp == NULL) throw Err() <<
        "Shape: can't create shape files: " << fbase << ".shp, " << fbase << ".shx" ;

    dbf = std::shared_ptr<void>(DBFCreate(fbase), DBFClose);
    if (dbf == NULL) throw Err() <<
        "Shape: can't create dbf file: " << fbase << ".dbf" ;
  }
  else {
    shp = std::shared_ptr<void>(SHPOpen(fbase, "rb+"), SHPClose);
    if (shp == NULL) throw Err() <<
      "Shape: can't open shape files: " << fbase << ".shp, " << fbase << ".shx" ;

    dbf = std::shared_ptr<void>(DBFOpen(fbase, "rb+"), DBFClose);
    if (dbf == NULL) throw Err() <<
      "Shape: can't open dbf file: " << fbase << ".dbf" ;

    // check type
    double mmin[4], mmax[4];
    SHPGetInfo((SHPHandle)shp.get(), &num, &type, mmin, mmax);
    type = sh2type(type);
    if (type!=t) throw Err() <<
      "Shape: wrong shapefile type: " << t;
  }
}

int
Shape::Impl::put(const dMultiLine & ml) {
  int nparts = ml.size();
  std::vector<int> parts;
  std::vector<double> x, y;
  int nverts = 0;
  for (auto l:ml){
    parts.push_back(nverts);
    nverts+=l.size();
    for (auto p:l) {
      x.push_back(p.x);
      y.push_back(p.y);
    }
  }
  SHPObject *o = SHPCreateObject(type2sh(type), -1,
      nparts, parts.data(), NULL,
      nverts, x.data(), y.data(), NULL, NULL);

  if (o==NULL) throw Err() << "Shape: can't create object";

  int id = SHPWriteObject((SHPHandle)shp.get(), -1, o);
  SHPDestroyObject(o);
  if (id==-1) throw Err() << "Shape: can't write object";
  num++;
  return id;
}

dMultiLine
Shape::Impl::get(const int id) {
  dMultiLine ret;
  SHPObject *o = SHPReadObject((SHPHandle)shp.get(), id);
  if (o==NULL) throw Err() << "Shape: can't read object: " << id;
  for (int p = 0; p<o->nParts; p++){
    dLine l;
    int j1 = o->panPartStart[p];
    int j2 = (p==o->nParts-1 ? o->nVertices : o->panPartStart[p+1]);
    for (int j=j1; j<j2; j++) l.push_back(dPoint(o->padfX[j],o->padfY[j]));
    ret.push_back(l);
  }
  SHPDestroyObject(o);
  return ret;
}

