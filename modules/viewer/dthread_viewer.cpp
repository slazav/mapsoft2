#include "dthread_viewer.h"
#include "cairo/cairo_wrapper.h"

#define TILE_SIZE (256)

DThreadViewer::DThreadViewer(GObj * pl) :
    SimpleViewer(pl),
    updater_needed(true) {

  if (!Glib::thread_supported()) Glib::thread_init();
  done_signal.connect(sigc::mem_fun(*this, &DThreadViewer::on_done_signal));

  updater_mutex = new(Glib::Mutex);
  updater_cond = new(Glib::Cond);
  updater_thread =
    Glib::Thread::create(sigc::mem_fun(*this, &DThreadViewer::updater), true);

}

DThreadViewer::~DThreadViewer(){
  updater_mutex->lock();
  updater_needed = false;
  updater_cond->signal();
  updater_mutex->unlock();
  updater_thread->join(); // waiting for our thread to exit
  delete(updater_mutex);
  delete(updater_cond);
}

void
DThreadViewer::redraw(const iRect & range){
  updater_mutex->lock();
  stop_drawing=true;
  tiles_cache.clear();
  updater_mutex->unlock();
  SimpleViewer::redraw(range);
}

iRect
DThreadViewer::tile_to_rect(const iPoint & key) const{
  return iRect(key, key + iPoint(1,1))*TILE_SIZE;
}

void
DThreadViewer::updater(){
  do {

    // generate tiles
    updater_mutex->lock();
    if (!tiles_todo.empty()){

      iPoint key = *tiles_todo.begin();

      stop_drawing=false;
      updater_mutex->unlock();

      CairoWrapper crw;
      crw.set_surface_img(TILE_SIZE, TILE_SIZE);
      crw->set_color(get_bgcolor());
      crw->paint();

      auto * o = get_obj();
      if (o){
        dRect r = tile_to_rect(key);
        o->get_lock();
        crw->save();
        crw->translate(-r.tlc());
        try { o->draw(crw, r); }
        catch (Err e){ std::cerr << "Viewer warning: " << e.str() << "\n"; }
        crw->restore();
      }
      crw.get_surface()->flush();

      updater_mutex->lock();
      if (!stop_drawing){
        if (tiles_cache.count(key)>0) tiles_cache.erase(key);
        tiles_cache.insert(std::make_pair(key, crw));
        tiles_done.push(key);
        tiles_todo.erase(key);
        done_signal.emit();
      }
    }
    updater_mutex->unlock();

    // cleanup queue
    iRect scr = iRect(get_origin().x, get_origin().y,  get_width(), get_height());
    iRect tiles_to_keep = ceil(dRect(scr)/(double)TILE_SIZE);

    updater_mutex->lock();
    std::set<iPoint>::iterator qit=tiles_todo.begin(), qit1;
    while (qit!=tiles_todo.end()) {
      if (tiles_to_keep.contains(*qit)) qit++;
      else {
        qit1=qit; qit1++;
        tiles_todo.erase(qit);
        qit=qit1;
      }
    }
    updater_mutex->unlock();

    // cleanup caches
    tiles_to_keep = expand(tiles_to_keep, TILE_MARG);

    updater_mutex->lock();

    auto it=tiles_cache.begin();
    while (it!=tiles_cache.end()) {
      if (tiles_to_keep.contains(it->first)) it++;
      else it = tiles_cache.erase(it);
    }

    updater_mutex->unlock();

    updater_mutex->lock();
    if (tiles_todo.empty()) updater_cond->wait(*updater_mutex);
    updater_mutex->unlock();
  }
  while (updater_needed);
}


void DThreadViewer::on_done_signal(){

  while (!tiles_done.empty()){
    iPoint key=tiles_done.front();

    if (tiles_cache.count(key)){
      iPoint pt = key*TILE_SIZE-get_origin();
      queue_draw_area(pt.x,pt.y, TILE_SIZE, TILE_SIZE);
    }

    updater_mutex->lock();
    tiles_done.pop();
    updater_mutex->unlock();
  }
  if (tiles_todo.empty()) signal_idle().emit();
}


void DThreadViewer::draw(const CairoWrapper & crw, const iRect & r){

  if (!r) {redraw(); return;}
  iRect tiles = ceil(dRect(r + get_origin())/(double)TILE_SIZE);
  iPoint key;

  updater_mutex->lock();
  if (tiles_todo.empty()) signal_busy().emit();
  updater_mutex->unlock();

  crw->save();
  for (key.y = tiles.y; key.y<tiles.y+tiles.h; key.y++){
    for (key.x = tiles.x; key.x<tiles.x+tiles.w; key.x++){

      iRect rect = tile_to_rect(key) - get_origin();
      crw->rectangle(rect);
      crw->reset_clip();
      crw->clip(); // set clip region

      // draw the tile from cache
      if (tiles_cache.count(key)>0){
        crw->set_source(
          tiles_cache.find(key)->second.get_surface(),
          rect.x, rect.y);
      }
      else { // no tile in cache
        // background painting
        crw->set_color(get_bgcolor());

        // put this tile in todo queue
        if (tiles_todo.count(key)==0){
          updater_mutex->lock();
          tiles_todo.insert(key);
          updater_cond->signal();
          updater_mutex->unlock();
        }
      }

      crw->paint();
    }
  }
  crw->restore();

  updater_mutex->lock();
  if (tiles_todo.empty()) signal_idle().emit();
  updater_mutex->unlock();
}

const int DThreadViewer::TILE_MARG;

