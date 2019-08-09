#include <fstream>
#include <sstream>
#include <stdint.h>
#include <cstring>
#include <string>

#include "mapdb.h"
#include "mp/mp.h"
#include "read_words/read_words.h"

using namespace std;

/* Import/export of MP format */

/**********************************************************/
void
MapDB::import_mp(const string & mp_file, const Opt & opts){

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

      if ((vs[0]=="level") && vs.size()==2){
        level = str_to_type<int>(vs[1]);
        continue;
      }

      throw Err() << "bad configuration file at line "
                  << line_num[0];
    }
  }

  if (opts.exists("cnv_point"))   cnvs[0] = opts.get<dLine>("cnv_point");
  if (opts.exists("cnv_line"))    cnvs[0] = opts.get<dLine>("cnv_line");
  if (opts.exists("cnv_polygon")) cnvs[0] = opts.get<dLine>("cnv_polygon");
  if (opts.exists("data_level"))  level   = opts.get<int>("level");

  // read MP file
  MP mp_data;
  ifstream in(mp_file);
  read_mp(in, mp_data);

  for (auto const & o:mp_data){

    MapDBObj o1;

    if (o.Class<0 || o.Class>2)
      throw Err() << "wrong MP class: "<< o.Class;
    o1.cl = (MapDBObjClass)o.Class;

    // convert type
    for (auto const & cnv: cnvs[o1.cl]){
      if (cnv.x == 0 || cnv.x == o.Type) {
        o1.type = cnv.y? cnv.y : o.Type;
        break;
      }
    }

    // skip unknown types
    if (!o1.type) continue;

    // name
    o1.name = o.Label;

    // comments
    for (auto const & c:o.Comment) o1.comm += c + '\n';
    if (o1.comm.size()) o1.comm.resize(o1.comm.size()-1); // cut trailing '\n'

    // direction
    if (o.Direction<0 || o.Direction>2)
      throw Err() << "wrong MP direction: "<< o.Direction;
    o1.dir = (MapDBObjDir)o.Direction;

    // source
    if (o.Opts.exists("Source")) o1.src=o.Opts.get<string>("Source");

    // choose data level (move to MP?)
    int l = -1;
    if (level < o.Data.size() && o.Data[level].size()>0) l = level;
    if (level <= o.EndLevel){
      for (int i = level; i>0; i--){
        if (i<o.Data.size() && o.Data[i].size()>0) {l=i; break;}
      }
    }
    if (l==-1) continue; // no data for the requested level

    // add object
    uint32_t id = add(o1);

    // set coordinates
    set_coord(id, o.Data[l]);

  }

}
/**********************************************************/

void
MapDB::export_mp(const string & mp_file, const Opt & opts){

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

  MP mp_data;
  uint32_t key = 0;

  std::string str = objects.get_first(key);

  while (key!=0xFFFFFFFF){
    MapDBObj o;
    o.unpack(str);
    str = objects.get_next(key);

    MPObj o1;
    o1.Class = o.cl;

    // convert type
    for (auto const & cnv: cnvs[o.cl]){
      if (cnv.x == 0 || cnv.x == o.type) {
        o1.Type = cnv.y? cnv.y : o.type;
        break;
      }
    }

    // skip unknown types
    if (!o1.Type) continue;

    // name
    o1.Label = o.name;

    // comments
    if (o.comm.size()){
      int pos1=0, pos2=0;
      do {
        pos2 = o.comm.find('\n', pos1);
        o1.Comment.push_back(o.comm.substr(pos1,pos2));
        pos1 = pos2+1;
      } while (pos2!=string::npos);
    }

    // direction
    o1.Direction = o.dir;

    // source
    if (o.src!="") o1.Opts.put("Source", o.src);

    // points
    o1.Data.push_back(get_coord(key));

    if (o1.Data.size()) mp_data.push_back(o1);
  }
  ofstream out(mp_file);
  write_mp(out, mp_data);
}

