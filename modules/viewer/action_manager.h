#ifndef ACTION_MANAGER_H
#define ACTION_MANAGER_H

#include <sys/time.h>
#include <map>
#include <string>
#include "action.h"
#include "simple_viewer.h"
#include "geom/point.h"

///\addtogroup gred
///@{
///\defgroup action_manager
///@{

/** Adding "actions" to a viewer.
*/
class ActionManager : public std::map<std::string, Action *> {
public:

  ActionManager(SimpleViewer * v);

  /// Add action. If name=="" try to get it from action->get_name().
  void add(Action * a, std::string name="");

  /// Select current action by name
  void select(std::string name);

private:

  /// helper functions to be connected with viewer signals
  void click_begin (GdkEventButton * event);
  void click_end (GdkEventButton * event);

  Action * current;
  struct timeval click_started;
  Gdk::ModifierType state;
  iPoint p;
  SimpleViewer * viewer;
};

#endif
