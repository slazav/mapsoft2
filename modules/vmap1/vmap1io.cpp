#include <list>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib> // for atoi

#include "vmap1.h"

#define CUR_VER 3.2

const int vmap1_point_scale = 1000000;
using namespace std;

/***************************************/

// read key-value pair separated by tab char
int vmap1_get_kv(const string &s, string &key, string &val){
  if (s=="") return 1;
  int tab=s.find('\t', 0);
  if (tab==-1){
    cerr << "skipping bad line: " << s << "\n";
    return 1;
  }
  int st=0;
  while (s[st]==' ') st++;
  key=s.substr(st, tab-st);
  val=s.substr(tab+1, -1);
  return 0;
}

// read a single point (x,y), divide result by vmap1_point_scale
dPoint vmap1_read_pt(istream & IN){
  dPoint p;
  char sep;
  IN >> p.x >> sep >> p.y >> std::ws;
  if (sep!=','){
    cerr << "bad point: " << IN.rdbuf() << "\n";
    return dPoint();
  }
  p/=vmap1_point_scale;
  return p;
}

// Read label parameters: align, horizontality or angle, font size
// v<3.1:  <align:0|1|2> <hor:0|1> <ang>
// v>=3.1: L|C|R H|<ang>
// v>=3.2: L|C|R H|<ang> S<fsize>

void vmap1_read_lab_pars(istream & IN, VMap1Lab & l, double ver){
  if (ver<3.1){
    IN >> l.dir >> l.hor >> l.ang;
  }
  else { // version 3.1...
    char c;
    IN >> c;
    switch (c){
      case 0: case 'l': case 'L': l.dir=0; break;
      case 1: case 'c': case 'C': l.dir=1; break;
      case 2: case 'r': case 'R': l.dir=2; break;
    }
    string s;
    IN >> s;
    if (s.size() && s[0]=='H' || s[0]=='h'){
      l.hor=1;
      l.ang=0.0;
    }
    else{
      l.hor=0;
      l.ang=atof(s.c_str());
    }
    IN >> s; // version 3.2
    if (s.size() && s[0]=='S')  l.fsize = atoi(s.c_str()+1);
    else l.fsize = 0;
  }
}

VMap1Lab vmap1_read_lab_pos(const string & s, double ver){
  VMap1Lab ret;
  istringstream IN1(s);
  ret.pos = vmap1_read_pt(IN1);
  vmap1_read_lab_pars(IN1, ret, ver);
  return ret;
}

VMap1Lfull vmap1_read_lbuf(const string & s, double ver){
  VMap1Lfull ret;
  istringstream IN1(s);
  ret.pos = vmap1_read_pt(IN1);
  vmap1_read_lab_pars(IN1, ret, ver);
  ret.ref = vmap1_read_pt(IN1);
  getline(IN1, ret.text);
  return ret;
}

dLine
read_vmap1_points(istream & IN, string & s){
  dLine ret;
  string key,val;
  if (vmap1_get_kv(s, key, val)!=0){
    cerr << "wrong call of read_vmap1_points()!\n";
    return ret;
  }
  s=val;
  do {
    istringstream IN1(s);
    while (IN1.good()){
      ret.push_back(vmap1_read_pt(IN1));
    }
    getline(IN, s);
  } while (s[0]=='\t');

  return ret;
}


VMap1Obj
read_vmap1_object(istream & IN, string & s, double ver){
  VMap1Obj ret;
  string key,val;
  bool read_ahead=false;

  if ((vmap1_get_kv(s, key, val)!=0) || (key!="OBJECT")){
    cerr << "wrong call of read_vmap1_object()!\n";
    return ret;
  }

  istringstream IN1(val);
  IN1 >> setbase(16) >> ret.type >> ws;
  getline(IN1,ret.text);

  while (!IN.eof() || read_ahead){
    if (!read_ahead) getline(IN, s);
    else read_ahead=false;
    if (vmap1_get_kv(s, key, val)!=0) continue;

    if (ver<3.1 && key=="TEXT"){  // backward comp
      ret.text=val;
      continue;
    }
    if (key=="DIR"){
      ret.dir=atoi(val.c_str());
      continue;
    }
    if (key=="OPT"){
      string k,v;
      if (vmap1_get_kv(val, k, v)!=0){
        cerr << "bad options in: " << s << "\n";
        continue;
      }
      ret.opts.put(k,v);
      continue;
    }
    if (key=="COMM"){
      ret.comm.push_back(val);
      continue;
    }
    if (key=="LABEL"){
      ret.labels.push_back(vmap1_read_lab_pos(val, ver));
      continue;
    }
    if (key=="DATA"){
      ret.push_back(read_vmap1_points(IN, s));
      read_ahead=true;
      continue;
    }
    break; // end of object
  }
  return ret;
}

// read vmap native format
VMap1
read_vmap1(istream & IN){
  VMap1 ret;
  string s, key, val;
  bool read_ahead=false;

  double ver;
  IN >> s >> ver;
  if (s!="VMAP"){
    cerr << "error: not a VMAP file\n";
    return ret;
  }
  if (ver>CUR_VER){
    cerr << "error: Too new VMAP format. Update mapsoft package.\n";
    return ret;
  }
  if (ver<CUR_VER){
    cerr << "note: reading old VMAP format version: "
         << fixed << setprecision(1) << ver << " < " << CUR_VER << "\n";
  }

  while (!IN.eof() || read_ahead){
    if (!read_ahead) getline(IN, s);
    else read_ahead=false;
    if (vmap1_get_kv(s, key, val)!=0) continue;

    if (key=="NAME"){
      ret.name=val;
      continue;
    }
    if (key=="RSCALE"){
      ret.rscale=atof(val.c_str());
      continue;
    }
    if (key=="STYLE"){
      ret.style=val;
      continue;
    }
    if (key=="MP_ID"){
      ret.mp_id=atoi(val.c_str());
      continue;
    }
    if (key=="BRD"){
      ret.brd = read_vmap1_points(IN, s);
      read_ahead=true;
      continue;
    }
    if (key=="LBUF"){
      ret.lbuf.push_back(vmap1_read_lbuf(val, ver));
      continue;
    }
    if (key=="OBJECT"){
      ret.push_back(read_vmap1_object(IN, s, ver));
      read_ahead=true;
      continue;
    }
    cerr << "skipping bad line: " << s << "\n";
  }
  return ret;
}

/***************************************/


void write_vmap1_line(ostream & OUT, const dLine & L){
  int n=0;
  for (auto &i:L){
    if ((n>0)&&(n%4==0)) OUT << "\n\t";
    else if (n!=0) OUT << " ";
    OUT << int(i.x) << "," << int(i.y);
    n++;
  }
}
// write label position
void write_vmap1_lpos(ostream & OUT, const VMap1Lab & L){
  // coordinates
  OUT << int(L.pos.x) << "," << int(L.pos.y) << " ";
  // alignment (left,right,center)
  switch (L.dir){
    case 0: OUT << 'L'; break;
    case 1: OUT << 'C'; break;
    case 2: OUT << 'R'; break;
  }
  // angle (or H for horizontal labels)
  if (L.hor) OUT << " H";
  else  OUT << " " << setprecision(2) << round(L.ang*100)/100;

  // font size correction
  if (L.fsize) OUT << " S" << L.fsize;
}


// write vmap to ostream
// put vmap to mp
int
write_vmap1(ostream & OUT, const VMap1 & W){

  VMap1 WS (W);

  // Sort problem: we write rounded values, so order can change!
  // Rounding values before sort:

  for (auto &l:WS.lbuf){
    l.pos=rint(l.pos*vmap1_point_scale);
    l.ref=rint(l.ref*vmap1_point_scale);
  }
  for (auto &o:WS){
    for (auto &l:o){
      for (auto &p:l){
        p=rint(p*vmap1_point_scale);
      }
    }
    for (auto &l:o.labels){
      l.pos=rint(l.pos*vmap1_point_scale);
    }
  }
  for (auto &p:WS.brd){
    p=rint(p*vmap1_point_scale);
  }

  WS.sort();
  WS.lbuf.sort();

  OUT << "VMAP " << fixed << setprecision(1) << CUR_VER << "\n";
  if (WS.name!="") OUT << "NAME\t" << WS.name << "\n";
  OUT << "RSCALE\t" << int(WS.rscale) << "\n";
  OUT << "STYLE\t" << WS.style << "\n";
  OUT << "MP_ID\t" << WS.mp_id << "\n";
  if (WS.brd.size()>0){
    OUT << "BRD\t";  write_vmap1_line(OUT, WS.brd); OUT << "\n";
  }

  // lbuf
  for (auto const &l:WS.lbuf){
    OUT << "LBUF\t"; write_vmap1_lpos(OUT, l);
    OUT << " " << int(l.ref.x) << "," << int(l.ref.y);
    if (l.text.size()) OUT << " " << l.text;
    OUT << "\n";
  }

  for (auto &o:WS){
    OUT << "OBJECT\t" << "0x" << setbase(16) << o.type << setbase(10);
    if (o.text != "") OUT << " " << o.text;
    OUT << "\n";

    if (o.dir != 0)
      OUT << "  DIR\t" << o.dir << "\n";
    for (auto const &i:o.opts){
      OUT << "  OPT\t" << i.first << "\t" << i.second << "\n"; // protect \n!
    }
    for (auto const &i:o.comm){
      OUT << "  COMM\t" << i << "\n";
    }
    o.labels.sort();
    for (auto const &i:o.labels){
      OUT << "  LABEL\t";  write_vmap1_lpos(OUT, i); OUT << "\n";
    }
    for (auto const &i:o){
      OUT << "  DATA\t"; write_vmap1_line(OUT, i); OUT << "\n";
    }
  }

  return 0;
}

