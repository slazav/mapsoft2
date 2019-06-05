#include <fstream>
#include <sstream>
#include <stdint.h>
#include <cstring>
#include <string>

#include "vmap.h"
#include "read_words/read_words.h"
#include "mp/mp.h"

using namespace std;

/**********************************************************/
// To keep objects in a database we pack everything in std::strings.
// Usually it is something silmilar to RIFF format:
// 4-byte tag, 4-byte size, data.

// pack a string
void
vmap_pack_str(ostream & s, const char *tag, const std::string & str){
  if (strlen(tag)!=4) throw Err() << "vmap_pack_str: 4-byte tag expected";
  s.write(tag, 4);
  uint32_t size = str.size();
  s.write((char *)&size, sizeof(uint32_t));
  s.write(str.data(), str.size());
  if (s.fail()) throw Err() << "vmap_pack_str: write error";
}

// pack any type
template <typename T>
void
vmap_pack(ostream & s, const char *tag, const T & v){
  if (strlen(tag)!=4) throw Err() << "vmap_pack_str: 4-byte tag expected";
  s.write(tag, 4);
  uint32_t size = sizeof(T);
  s.write((char *)&size, sizeof(uint32_t));
  s.write((char *)&v, size);
  if (s.fail()) throw Err() << "vmap_pack_str: write error";
}

// Pack a multiline with LonLat coordinates (as a multiple "crds" tags).
// Double values are multiplied by 1e7 and rounded to integer values.
void
vmap_pack_crds(ostream & s, const dMultiLine & ml){
  for (auto const &l:ml) {
    s.write("crds", 4);
    uint32_t size = l.size()*2*sizeof(int32_t); // 2 ints per point
      s.write((char *)&size, sizeof(uint32_t));
    for (auto p:l) {
      while (p.x >  180) p.x-=360;
      while (p.x < -180) p.x+=360;
      while (p.y >   90) p.y-=180;
      while (p.y <  -90) p.y+=180;
      int32_t crd[2] = {(int32_t)rint(p.x * 1e7), (int32_t)rint(p.y * 1e7)};
      s.write((char *)crd, 2*sizeof(int32_t));
    }
  }
  if (s.fail()) throw Err() << "vmap_pack_crds: write error";
}

// read 4-byte tag
std::string
vmap_unpack_tag(istream & s){
  std::string tag(4,'\0');
  s.read((char*)tag.data(), 4);
  if (s.eof()) return std::string();
  if (s.fail()) throw Err() << "vmap_unpack_tag: read error";
  return tag;
}

// unpack string (tag is already read)
std::string
vmap_unpack_str(istream & s){
  uint32_t size;
  s.read((char*)&size, sizeof(uint32_t));
  std::string str(size, '\0');
  s.read((char*)str.data(), size);
  if (s.fail()) throw Err() << "vmap_unpack_str: read error";
  return str;
}

// unpack any type (tag is already read)
template <typename T>
T vmap_unpack(istream & s){
  uint32_t size;
  s.read((char*)&size, sizeof(uint32_t));
  if (size != sizeof(T)) throw Err() << "vmap_unpack: bad data size";
  T ret;
  s.read((char*)&ret, size);
  if (s.fail()) throw Err() << "vmap_unpack_str: read error";
  return ret;
}

// unpack coordinate line (tag is already read)
dLine
vmap_unpack_crds(istream & s){
  uint32_t size;
  s.read((char*)&size, sizeof(uint32_t));
  dLine ret;
  for (int i=0;i<size/2/sizeof(int32_t);i++) {
    int32_t crd[2];
    s.read((char*)crd, 2*sizeof(int32_t));
    dPoint p(crd[0]/1e7, crd[1]/1e7);
    ret.push_back(p);
  }
  if (s.fail()) throw Err() << "vmap_unpack_crds: read error";
  return ret;
}



/**********************************************************/
// pack object to a string (for DB storage)
string
VMapObj::pack() const {
  ostringstream s;

  // two integer numbers: class, type:
  int32_t v;
  v = (int32_t)cl;   s.write((char *)&v, sizeof(int32_t));
  v = (int32_t)type; s.write((char *)&v, sizeof(int32_t));

  // optional direction (int value)
  if (dir!=NO) vmap_pack<uint32_t>(s, "dir ", (uint32_t)dir);

  // optional angle (integer value, 1/1000 degrees)
  if (angle!=0) vmap_pack<int32_t>(s, "angl", (int32_t)(angle*1000));

  // optional text fields (4-byte tag, 4-byte length, data);
  if (name!="") vmap_pack_str(s, "name", name);
  if (comm!="") vmap_pack_str(s, "comm", comm);
  if (src!="") vmap_pack_str(s, "src ", src);

  // coordinates (4-byte tag, 4-byte length, data);
  vmap_pack_crds(s, *this);
  return s.str();
}

// unpack object from a string (for DB storage)
void
VMapObj::unpack(const std::string & str) {

  // re-initialize
  *this = VMapObj();

  istringstream s(str);

  // class
  int32_t v;
  s.read((char*)&v, sizeof(int32_t));
  if (v<0 || v>2) throw Err() << "VMapObj::unpack: bad class value: " << v;
  cl = (VMapObjClass)v;

  // type
  s.read((char*)&v, sizeof(int32_t));
  type = v;

  // other fields
  while (1){
    string tag = vmap_unpack_tag(s);
    if (tag == "") break;
    else if (tag == "dir ") dir = (VMapObjDir)vmap_unpack<uint32_t>(s);
    else if (tag == "angl") angle = vmap_unpack<int32_t>(s)/1000.0;
    else if (tag == "name") name = vmap_unpack_str(s);
    else if (tag == "comm") comm = vmap_unpack_str(s);
    else if (tag == "src ") src = vmap_unpack_str(s);
    else if (tag == "crds") push_back(vmap_unpack_crds(s));
    else throw Err() << "Unknown tag: " << tag;
  }

}


/**********************************************************/
void
VMap::add(const VMapObj & o){
  // do not work with empty objects
  if (o.is_empty()) throw Err() << "VMap::add: empty object";

  // get last id + 1
  uint32_t id;
  storage.get_last(id);
  if (id == 0xFFFFFFFF) id = 0;
  else id++;
  if (id<100) id=100; // values below 100 are reserved.

  // insert object
  storage.put(id, o.pack());
  // update bbox
  dRect r = o.bbox2d();
  bbox.expand(r);

  // update statial index
  geo_ind.put(id, r);
}

/**********************************************************/
void
VMap::import_mp(const string & mp_file, const Opt & opts){

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

    VMapObj o1;

    if (o.Class<0 || o.Class>2)
      throw Err() << "wrong MP class: "<< o.Class;
    o1.cl = (VMapObjClass)o.Class;

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
    o1.dir = (VMapObjDir)o.Direction;

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
    o1.dMultiLine::operator=(o.Data[l]); // set data

    add(o1);
  }

}
/**********************************************************/

void
VMap::export_mp(const string & mp_file, const Opt & opts){

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
  uint32_t key = 100;

  std::string str = storage.get_first(key);

  while (key!=0xFFFFFFFF){
    VMapObj o;
    o.unpack(str);
    str = storage.get_next(key);

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
    o1.Data.push_back((dMultiLine)o);

    if (o1.Data.size()) mp_data.push_back(o1);
  }
  ofstream out(mp_file);
  write_mp(out, mp_data);
}


/**********************************************************/
/// Import objects from VMAP1 file.
void
VMap::import_vmap1(const std::string & vmap_file, const Opt & opts){

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
  VMap1 vmap1_data = read_vmap1(in);
  for (auto const & o:vmap1_data){

    // skip empty objects
    if (o.is_empty()) continue;

    VMapObj o1;
    if (o.type & 0x100000) o1.cl = LINE;
    if (o.type & 0x200000) o1.cl = POLYGON;

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
    o1.dir = (VMapObjDir)o.dir;

    // source
    if (o.opts.exists("Source")) o1.src=o.opts.get<string>("Source");

    // data
    o1.dMultiLine::operator=(o); // set data

    add(o1);
  }

}

/**********************************************************/
/// Export objects to VMAP1 file.
void
VMap::export_vmap1(const std::string & vmap_file, const Opt & opts){

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

  VMap1 vmap1_data;
  uint32_t key = 100;
  std::string str = storage.get_first(key);

  while (key!=0xFFFFFFFF){
    VMapObj o;
    o.unpack(str);
    str = storage.get_next(key);

    VMap1Obj o1;

    // convert type
    for (auto const & cnv: cnvs[o.cl]){
      if (cnv.x == 0 || cnv.x == o.type) {
        o1.type = cnv.y? cnv.y : o.type;
        break;
      }
    }

    // skip unknown types
    if (!o1.type) continue;

    if (o.cl == LINE)    o1.type |= 0x100000;
    if (o.cl == POLYGON) o1.type |= 0x200000;

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

    // points
    o1.dMultiLine::operator=(o);

    vmap1_data.push_back(o1);
  }
  ofstream out(vmap_file);
  write_vmap1(out, vmap1_data);
}

