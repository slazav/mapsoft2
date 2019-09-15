#ifndef GOBJ_MULTI_H
#define GOBJ_MULTI_H

#include <map>
#include <vector>
#include <memory>
#include "gobj.h"

// Combine multiple GObj into one

class GObjMulti : public GObj {
private:

  // add additional information to GObj
  struct GObjData {
    std::shared_ptr<GObj> obj;
    bool on; // on/off
    sigc::connection redraw_conn;
  };

  std::multimap<int, GObjData> data;

public:

  GObjMulti(std::shared_ptr<ConvBase> c): GObj(c){}


  // add new object
  void add(int depth, std::shared_ptr<GObj> o){
    if (!o) return;

    o->set_cnv(cnv);

    stop_drawing = true;
    auto lock = get_lock();

    GObjData D;
    D.obj = o;
    D.on = true;

    auto & sig = signal_redraw_me();
    D.redraw_conn = o->signal_redraw_me().connect(
      sigc::mem_fun (&sig, &sigc::signal<void, iRect>::emit));

    data.emplace(depth, D);
  }

  // find an object
  std::multimap<int, GObjData>::iterator find(std::shared_ptr<GObj> o) {
    for (auto i = data.begin(); i!=data.end(); ++i)
      if (i->second.obj == o) return i;
    return data.end();
  }

  // delete an object
  void del(std::shared_ptr<GObj> o){
    if (!o) return;

    stop_drawing = true;
    auto lock = get_lock();

    auto it = find(o);
    if (it==data.end()) return;
    it->second.redraw_conn.disconnect();
    data.erase(it);
  }

  // draw all objects
  int draw(const CairoWrapper & cr, const dRect & draw_range) override{
    int res = GObj::FILL_NONE;
    for (auto const & p:data){
      if (!p.second.on) continue;
      cr->save();
      int res1 = p.second.obj->draw(cr, draw_range);
      cr->restore();
      if (res1 != GObj::FILL_NONE &&
          res!=GObj::FILL_ALL) res=res1;
    }
    return res;
  }

  // rescale
  void on_rescale(double k) override{
    for (auto const & p:data) p.second.obj->on_rescale(k);
  }

  // set cnv
  void on_set_cnv() override{
    for (auto const & p:data) p.second.obj->set_cnv(cnv);
  }

};

#endif