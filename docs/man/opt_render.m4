=head2 Options for drawing tracks:

=over 2

=item B<<  --trk_draw_mode <arg> >>

track drawing mode (normal, speed, height)

=item B<<  --trk_draw_th <arg> >>

line thickness, overrides track setting

=item B<<  --trk_draw_color <arg> >>

color (for normal drawing mode), overrides track color

=item B<<  --trk_draw_dots <arg> >>

draw dots (for normal drawing mode), default: 1

=item B<<  --trk_draw_arrows <arg> >>

draw arrows (for normal drawing mode), default: 0

=item B<<  --trk_draw_min <arg> >>

min value (km/h for speed mode, m for height mode)

=item B<<  --trk_draw_max <arg> >>

max value (km/h for speed mode, m for height mode)

=item B<<  --trk_draw_grad <arg> >>

color gradient (for speed or height modes), default: BCGYRM

=back

=head2 Options for drawing waypoints:


=over 2

=item B<<  --wpt_text_font <arg> >>

waypoint font (default: "serif")

=item B<<  --wpt_text_size <arg> >>

waypoint font size, pixels (default: 10)

=item B<<  --wpt_text_pad <arg> >>

waypoint text padding, pixels (default: 2)

=item B<<  --wpt_draw_size <arg> >>

waypoint dot radius, pixels (default: 3)

=item B<<  --wpt_line_width <arg> >>

waypoint line width, (default: 1)

=item B<<  --wpt_stick_len <arg> >>

length of waypoint flag stick (default: 10)

=item B<<  --wpt_color <arg> >>

waypoint color (default: 0xFF000000)

=item B<<  --wpt_bgcolor <arg> >>

waypoint background color (default: 0xFFFFFFFF)

=item B<<  --wpt_adj <arg> >>

adjust waypoint flag positions to prevent collisions (default: 1)

=item B<<  --wpt_adj_brd <arg> >>

adjust waypoint flag positions to prevent boundary collisions (default: 0)

=item B<<  --wpt_skip_far <arg> >>

skip points if their labels can not be placed close enough (default: 5*wpt_stick_len)

=back


=head2 Options for drawing maps:

=over 2

=item B<<  --map_smooth <arg> >>

Smooth map drawing (interpolation for small scales, averaging for large ones), default 0

=item B<<  --map_clip_brd <arg> >>

Clip map to its border (default 1)

=item B<<  --map_draw_refs <arg> >>

Draw map reference points (ARGB color, default 0)

=item B<<  --map_draw_brd <arg> >>

Draw map border (ARGB color, default 0)

=item B<<  --map_fade <arg> >>

Fade the map (0..100, default is 0, no fading)

=back

