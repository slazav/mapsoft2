#ifndef GOBJ_MULTI_H
#define GOBJ_MULTI_H

#include <map>
#include <vector>
#include <memory>
#include "gobj.h"

// Combine multiple GObj into one.

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

  GObjMulti(std::shared_ptr<ConvBase> c =
              std::shared_ptr<ConvBase>(new ConvBase)): GObj(c){}


  // Add new object at some depth (larger depth - earlier the object is drawn)
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
    data.emplace(-depth, D); // we use negative depth for correct sorting

    stop_drawing = false;
    signal_redraw_me().emit(iRect());
  }

  // size(), number of objects
  int size() const { return data.size(); }

private:

  // find an object
  std::multimap<int, GObjData>::iterator
  find(const std::shared_ptr<GObj> & o) {
    for (auto i = data.begin(); i!=data.end(); ++i)
      if (i->second.obj == o) return i;
    return data.end();
  }

  // find an object (const version)
  std::multimap<int, GObjData>::const_iterator
  find(const std::shared_ptr<GObj> & o) const {
    for (auto i = data.begin(); i!=data.end(); ++i)
      if (i->second.obj == o) return i;
    return data.end();
  }

public:

  // get object depth (-1 if there is no such object)
  int get_depth(std::shared_ptr<GObj> o) const{
    auto it = find(o);
    if (it==data.end())
      throw Err() << "GObjMulti::get_depth: no such object";
    return -it->first;
  }

  // get object visibility trow Err if there is no such object)
  bool get_visibility(std::shared_ptr<GObj> o) const{
    auto it = find(o);
    if (it==data.end())
      throw Err() << "GObjMulti::get_visibility: no such object";
    return it->second.on;
  }

  // set object depth
  void set_depth(std::shared_ptr<GObj> o, int depth){
    if (!o) return;

    stop_drawing = true;
    auto lock = get_lock();

    auto it = find(o);
    if (it==data.end()) return;
    if (it->first == -depth) return;
    data.emplace(-depth, it->second);
    data.erase(it);

    stop_drawing = false;
    signal_redraw_me().emit(iRect());
  }

  // set object visibility
  void set_visibility(std::shared_ptr<GObj> o, bool on){
    if (!o) return;

    stop_drawing = true;
    auto lock = get_lock();

    auto it = find(o);
    if (it==data.end()) return;
    it->second.on = on;

    stop_drawing = false;
    signal_redraw_me().emit(iRect());
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

    stop_drawing = false;
    signal_redraw_me().emit(iRect());
  }

  // delete all objects
  void clear(){
    stop_drawing = true;
    auto lock = get_lock();

    for (auto & o:data) o.second.redraw_conn.disconnect();
    data.clear();

    stop_drawing = false;
    signal_redraw_me().emit(iRect());
  }

  // Draw all objects
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

  // Rescale
  void on_rescale(double k) override{
    // Note that `on_rescale` is called instead on `rescale`.
    // Thus cnv is not modified, locking is not done, signal_redraw_me
    // is not emitted in sub-objects, it is done in GobjMulti::rescale!
    for (auto const & p:data) p.second.obj->on_rescale(k);
  }

  // Set cnv
  void on_set_cnv() override{
    // Note that `on_set_cnv` is called instead on `set_cnv`.
    // Same effects as in on_rescale().
    for (auto const & p:data) p.second.obj->on_set_cnv();
  }

};

#endif