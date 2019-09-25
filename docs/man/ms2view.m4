=head1 NAME

ms2view -- mapsoft2 viewer for geodata and raster maps


=head1 USAGE

	ms2view <options> <input files>


=head1 GENERAL OPTIONS

include(`opt_general.m4')


=head1 READING GEODATA

See B<GEODATA FORMATS> section in C<mapsoft2(5)> for supported formats
and related options.

=head1 SETTING UP MAP REFERENCE FOR THE VIEWER

include(`opt_mkref.m4')


=head1 DRAWING OPTIONS

include(`opt_render.m4')

=head1 CUSTOMIZING VIEWER INTERFACE WITH CSS STYLES

Structure of B<ms2view> window is simple: a viewer, a main menu, a
statusbar, and tables with waypoints, tracks, etc. Appearence of these
elements is controlled by GTK3 theme. One can customize it using CSS
styles. Styles are read from two places:
B</usr/share/mapsoft2/mapsoft2.css> -- system-wide file. By default it
contains a few definitions, mostly for making interface elements smaller.
Some tips about viewer structure can be also find there.
B<$HOME/.mapsoft2/mapsoft2.css> -- user-specific file.

Additional information about writing css styles can be found in GTK3
documentation or GTK theme sources:

=over 1

=item * L<https://developer.gnome.org/gtk3/stable/chap-css-overview.html>

=item * L<https://developer.gnome.org/gtk3/stable/chap-css-properties.html>

=item * L<https://github.com/GNOME/gtk/blob/master/gtk/theme/Adwaita/>

=back

include(`foot.m4')
