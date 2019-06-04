#include <fstream>

#include "vmap.h"
#include "read_words/read_words.h"
#include "mp/mp.h"

using namespace std;
/**********************************************************/


void
VMap::add(const VMapObj & o){
  // get last id + 1
  int id = storage.size()? storage.rbegin()->first + 1 : -1;
  if (id < 100) id=100; // values below 100 are reserved.

  // insert object
  storage.insert(make_pair(id, o));

  // update bbox
  dRect r = o.bbox2d();
  bbox.expand(r);

  // update statial index
  geo_ind.put(id, r);
}

/**********************************************************/
void
VMap::import_mp(
      const string & mp_file,
      const string & conf_file){

  // type conversion tables (point, line, polygon)
  vector<list<pair<int,int> > > cnvs;
  cnvs.resize(3);

  // data level
  int level = 0;

  // Read conf_file.
  if (conf_file == ""){
    // default configuration 0->0
    for (int i=0; i<3; i++)
      cnvs[i].push_back(make_pair(0,0));
  }
  else {
    int line_num[2] = {0,0};
    ifstream ff(conf_file);
    while (1){
      vector<string> vs = read_words(ff, line_num, true);
      if (vs.size()<1) continue;

      int cl = -1;
      if (vs[0]=="point" || vs[0]=="poi") cl=0;
      if (vs[0]=="line" || vs[0]=="multiline") cl=1;
      if (vs[0]=="polygon") cl=2;

      if (cl>=0 &&  vs.size()>1 && vs.size()<3){
        cnvs[cl].push_back(make_pair(
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

  // read MP file
  MP mp_data;
  ifstream in(mp_file);
  read_mp(in, mp_data);

  for (auto const & o:mp_data){

    VMapObj v;

    if (o.Class<0 || o.Class>2)
      throw Err() << "wrong MP class: "<< o.Class;
    v.cl = (VMapObjClass)o.Class;

    // convert type
    for (auto const & cnv: cnvs[v.cl]){
      if (cnv.first == 0 || cnv.first == o.Type) {
        v.type = cnv.second? cnv.second : o.Type;
        break;
      }
    }

    // skip unknown types
    if (!v.type) continue;

    // name
    v.name = o.Label;

    // comments
    for (auto const & c:o.Comment) v.comm += c + '\n';

    // direction
    if (o.Direction<0 || o.Direction>2)
      throw Err() << "wrong MP direction: "<< o.Direction;
    v.dir = (VMapObjDir)o.Direction;

    // source
    if (o.Opts.exists("Source")) v.src=o.Opts.get<string>("Source");

    // choose data level (move to MP?)
    int l = -1;
    if (level < o.Data.size() && o.Data[level].size()>0) l = level;
    if (level <= o.EndLevel){
      for (int i = level; i>0; i--){
        if (i<o.Data.size() && o.Data[i].size()>0) {l=i; break;}
      }
    }
    if (l==-1) continue; // no data for the requested level
    v.dMultiLine::operator=(o.Data[l]); // set data

    add(v);
  }

}
/**********************************************************/

void
VMap::export_mp(
      const string & mp_file,
      const string & conf_file){

  // type conversion tables (point, line, polygon)
  vector<list<pair<int,int> > > cnvs;
  cnvs.resize(3);

  // Read conf_file.
  if (conf_file == ""){
    // default configuration 0->0
    for (int i=0; i<3; i++)
      cnvs[i].push_back(make_pair(0,0));
  }
  else {
    int line_num[2] = {0,0};
    ifstream ff(conf_file);
    while (1){
      vector<string> vs = read_words(ff, line_num, true);
      if (vs.size()<1) continue;

      int cl = -1;
      if (vs[0]=="point" || vs[0]=="poi") cl=0;
      if (vs[0]=="line" || vs[0]=="multiline") cl=1;
      if (vs[0]=="polygon") cl=2;

      if (cl>=0 &&  vs.size()>1 && vs.size()<3){
        cnvs[cl].push_back(make_pair(
          str_to_type<int>(vs[1]),
          vs.size()<3? 0:str_to_type<int>(vs[2])));
        continue;
      }

      throw Err() << "bad configuration file at line "
                  << line_num[0];
    }
  }

  MP mp_data;
  for (auto const & i: storage){
    if (i.first<100) continue;
    const VMapObj & o = i.second;
    MPObj o1;
    o1.Class = o.cl;

    // convert type
    for (auto const & cnv: cnvs[o.cl]){
      if (cnv.first == 0 || cnv.first == o.type) {
        o1.Type = cnv.second? cnv.second : o.type;
        break;
      }
    }

    // skip unknown types
    if (!o1.Type) continue;

    // name
    o1.Label = o.name;

    // comments
    int pos1=0, pos2=0;
    do { 
      pos2 = o.comm.find('\n', pos1);
      o1.Comment.push_back(o.comm.substr(pos1,pos2));
      pos1 = pos2+1;
    } while (pos2!=string::npos);

    // direction
    o1.Direction = o.dir;

    // source
    if (o.src!="") o1.Opts.put("Source", o.src);

    // points
    o1.Data.push_back((dMultiLine)o);

    if (o1.Data.size()) mp_data.push_back(o1);
  }
  ofstream out(mp_file);
  write_mp(out, mp_data);
}

