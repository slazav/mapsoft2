#include <map>
#include <string>
#include <vector>
#include <iomanip>

#include "err/err.h"
#include "fig_opt.h"

using namespace std;

void fig_get_opts(const vector<string> & comment, Opt & opts){
  vector<string>::const_iterator i;
  for (i=comment.begin(); i!=comment.end(); i++){
    if (i->size()>0 && (*i)[0] == '\\'){
      int p1 = 1;
      int p2 = i->find('=');
      std::string key, val;
      if (p2<p1) opts.put(i->substr(p1,-1), 1);
      else opts.put(i->substr(p1,p2-p1), i->substr(p2+1,-1));
    }
  }
}
void fig_get_opts(const Fig & f, Opt & opts) {fig_get_opts(f.comment, opts);}
void fig_get_opts(const FigObj & o, Opt & opts) {fig_get_opts(o.comment, opts);}

void fig_del_opts(vector<string> & comment){
  Opt ret;
  vector<string> new_comm;
  vector<string>::const_iterator i;
  for (i=comment.begin(); i!=comment.end(); i++){
    if (i->size()>0 && (*i)[0] == '\\') continue;
    new_comm.push_back(*i);
  }
  swap(comment, new_comm);
}

void fig_del_opts(Fig & f){ fig_del_opts(f.comment); }
void fig_del_opts(FigObj & o){ fig_del_opts(o.comment); }

void fig_add_opts(vector<string> & comm, const Opt & opts){
  Opt o;
  fig_get_opts(comm, o);
  fig_del_opts(comm);
  o.put(opts);
  Opt::const_iterator i;
  for (i=o.begin(); i!=o.end(); i++){
    comm.push_back(string("\\") + i->first + "=" + i->second);
  }
}

void fig_add_opts(Fig & f, const Opt & opts) {fig_add_opts(f.comment, opts);}
void fig_add_opts(FigObj & o, const Opt & opts) {fig_add_opts(o.comment, opts);}
