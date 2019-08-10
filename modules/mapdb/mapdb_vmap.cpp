#include <fstream>
#include <sstream>
#include <stdint.h>
#include <cstring>
#include <string>

#include "mapdb.h"
#include "vmap/vmap.h"
#include "read_words/read_words.h"

using namespace std;

/* Import/export of old mapsoft VMAP format */

/**********************************************************/
/// Import objects from VMAP1 file.
void
MapDB::import_vmap(const std::string & vmap_file, const Opt & opts){

  // type conversion tables (point, line, polygon)
  vector<iLine> cnvs;
  cnvs.resize(3);
  // default configuration 0->0
  for (int i=0; i<3; i++)
    cnvs[i].push_back(iPoint(0,0));

  // data level
  int level = 0;


  // Read configuration file.
  if (opts.exists("config")){
    int line_num[2] = {0,0};

    // reset default configuration:
    for (int i=0; i<3; i++) cnvs[i] = iLine();

    ifstream ff(opts.get<string>("config"));
    while (1){
      vector<string> vs = read_words(ff, line_num, true);
      if (vs.size()<1) continue;

      int cl = -1;
      if (vs[0]=="point" || vs[0]=="poi") cl=0;
      if (vs[0]=="line" || vs[0]=="multiline") cl=1;
      if (vs[0]=="polygon") cl=2;

      if (cl>=0 &&  vs.size()>1 && vs.size()<3){
        cnvs[cl].push_back(iPoint(
          str_to_type<int>(vs[1]),
          vs.size()<3? 0:str_to_type<int>(vs[2])));
        continue;
      }

      throw Err() << "bad configuration file at line "
                  << line_num[0];
    }
  }

  if (opts.exists("cnv_point"))   cnvs[0] = opts.get<dLine>("cnv_point");
  if (opts.exists("cnv_line"))    cnvs[0] = opts.get<dLine>("cnv_line");
  if (opts.exists("cnv_polygon")) cnvs[0] = opts.get<dLine>("cnv_polygon");

  // read VMAP file
  ifstream in(vmap_file);
  VMap vmap_data = read_vmap(in);
  for (auto const & o:vmap_data){

    // skip empty objects
    if (o.is_empty()) continue;

    MapDBObj o1;
    if (o.type & 0x100000) o1.cl = MAPDB_LINE;
    if (o.type & 0x200000) o1.cl = MAPDB_POLYGON;

    // convert type
    for (auto const & cnv: cnvs[o1.cl]){
      if (cnv.x == 0 || cnv.x == o.type & 0xFFFFF) {
        o1.type = cnv.y? cnv.y : o.type & 0xFFFFF;
        break;
      }
    }

    // skip unknown types
    if (!o1.type) continue;

    // name
    o1.name = o.text;
    // comments
    for (auto const & c:o.comm) o1.comm += c + '\n';
    if (o1.comm.size()) o1.comm.resize(o1.comm.size()-1); // cut trailing '\n'

    // direction
    if (o.dir<0 || o.dir>2)
      throw Err() << "wrong MP direction: "<< o.dir;
    o1.dir = (MapDBObjDir)o.dir;

    // source
    if (o.opts.exists("Source")) o1.src=o.opts.get<string>("Source");

    // angle (deg -> deg)
    if (o.opts.exists("Angle")) o1.angle=o.opts.get<float>("Angle");

    // add object
    uint32_t id = add(o1);

    // set coordinates
    set_coord(id, (dMultiLine)o);
  }

  // border
  dMultiLine brd;
  brd.push_back(vmap_data.brd);
  set_map_brd(brd);

  // map name
  set_map_name(vmap_data.name);

}

/**********************************************************/
/// Export objects to VMAP1 file.
void
MapDB::export_vmap(const std::string & vmap_file, const Opt & opts){

  // type conversion tables (point, line, polygon)
  vector<iLine> cnvs;
  // default configuration 0->0
  cnvs.resize(3);
  for (int i=0; i<3; i++)
    cnvs[i].push_back(iPoint(0,0));

  // Read configuration file.
  if (opts.exists("config")){

    // reset default configuration:
    for (int i=0; i<3; i++) cnvs[i] = iLine();

    int line_num[2] = {0,0};
    ifstream ff(opts.get<string>("config"));
    while (1){
      vector<string> vs = read_words(ff, line_num, true);
      if (vs.size()<1) continue;

      int cl = -1;
      if (vs[0]=="point" || vs[0]=="poi") cl=0;
      if (vs[0]=="line" || vs[0]=="multiline") cl=1;
      if (vs[0]=="polygon") cl=2;

      if (cl>=0 &&  vs.size()>1 && vs.size()<3){
        cnvs[cl].push_back(iPoint(
          str_to_type<int>(vs[1]),
          vs.size()<3? 0:str_to_type<int>(vs[2])));
        continue;
      }

      throw Err() << "bad configuration file at line "
                  << line_num[0];
    }
  }

  if (opts.exists("cnv_point"))   cnvs[0] = opts.get<dLine>("cnv_point");
  if (opts.exists("cnv_line"))    cnvs[0] = opts.get<dLine>("cnv_line");
  if (opts.exists("cnv_polygon")) cnvs[0] = opts.get<dLine>("cnv_polygon");

  VMap vmap_data;
  uint32_t key = 0;
  std::string str = objects.get_first(key);

  while (key!=0xFFFFFFFF){
    MapDBObj o;
    o.unpack(str);

    VMapObj o1;

    // convert type
    for (auto const & cnv: cnvs[o.cl]){
      if (cnv.x == 0 || cnv.x == o.type) {
        o1.type = cnv.y? cnv.y : o.type;
        break;
      }
    }

    // skip unknown types
    if (!o1.type) continue;

    if (o.cl == MAPDB_LINE)    o1.type |= 0x100000;
    if (o.cl == MAPDB_POLYGON) o1.type |= 0x200000;

    // name
    o1.text = o.name;

    // comments
    if (o.comm.size()){
      int pos1=0, pos2=0;
      do {
        pos2 = o.comm.find('\n', pos1);
        o1.comm.push_back(o.comm.substr(pos1,pos2));
        pos1 = pos2+1;
      } while (pos2!=string::npos);
    }

    // direction
    o1.dir = o.dir;

    // source
    if (o.src!="") o1.opts.put("Source", o.src);

    // angle (deg->deg)
    if (o.angle!=0) o1.opts.put("Angle", o.angle);

    // points
    o1.dMultiLine::operator=(get_coord(key));

    vmap_data.push_back(o1);
    str = objects.get_next(key);
  }

  // map border (only first segment)
  dMultiLine brd = get_map_brd();
  if (brd.size()>0) vmap_data.brd = *brd.begin();

  // map name
  vmap_data.name = get_map_name();

  // write vmap file
  ofstream out(vmap_file);
  write_vmap(out, vmap_data);
}

