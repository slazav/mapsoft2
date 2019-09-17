### Using CSS to customise ms2view program

Structure of `ms2view` window is simple: a viewer, a statusbar and tables
with waypoints, tracks, etc. Appearence of these elements is controlled
by GTK3 theme. One can customize it using CSS styles. Styles are read
from two places:
* system-wide file `/usr/share/mapsoft2/mapsoft2.css`
* user-specific file `$HOME/.mapsoft2/mapsoft2.css`.

Some hints for writing css styles can be found in:
* https://developer.gnome.org/gtk3/stable/chap-css-overview.html
* https://developer.gnome.org/gtk3/stable/chap-css-properties.html

An example of existing style can be also useful (note that it is a
source `sccs` format):
* https://github.com/GNOME/gtk/blob/master/gtk/theme/Adwaita/_common.scss

Some names are assigned to mapsoft widgets to simplify access:
* #main_vbox -- top-level box which contain all other widgets
* #main_paned -- paned with the viewer and the notebook
* #status -- hbox with "busy" icon and the statusbar
* #panels -- notebook with tracks, waypoints, etc.

