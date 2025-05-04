Name:         mapsoft2
Version:      2.10
Release:      alt1

Summary:      mapsoft2 - programs for working with maps and geodata
Group:        Sciences/Geosciences
Url:          https://slazav.github.io/mapsoft2/
Packager:     Vladislav Zavjalov <slazav@altlinux.org>
License:      GPL-3.0

Source:        %name-%version.tar

BuildRequires: gcc-c++ libgtkmm3-devel libcairomm-devel
BuildRequires: libjansson-devel libxml2-devel libzip-devel zlib-devel libproj-devel
BuildRequires: libjpeg-devel libgif-devel libtiff-devel libpng-devel libdb4.7-devel
BuildRequires: librsvg-devel libcurl-devel libsqlite3-devel
BuildRequires: /usr/bin/pod2man /usr/bin/pod2html /usr/bin/unzip
BuildRequires: transfig ImageMagick-tools
%description
mapsoft2 - programs for working with maps and geodata

%package vmap-data
Summary: mapsoft-vmap-data - example of data and scripts for vector map handling
Group: Sciences/Geosciences
Requires: %name = %version-%release
BuildRequires: perl-Text-Iconv
BuildArch: noarch

%description vmap-data
Example of data, scripts, xfig libraries for vector map handling.


%prep
%setup -q

%build
tar -xvf modules.tar
export SKIP_IMG_DIFFS=1
%make

%install
%makeinstall initdir=%buildroot%_initdir

%files
%_bindir/ms2*
%_mandir/man1/ms2*
%_mandir/man5/mapsoft2*
%dir %_datadir/mapsoft2
%_datadir/mapsoft2/mapsoft2.css
%_datadir/mapsoft2/maps_menu.json
%_datadir/applications/ms2view.desktop

%files vmap-data
%_bindir/vmaps*
%_datadir/mapsoft2/render.cfg
%_datadir/mapsoft2/types.cfg
%_datadir/mapsoft2/pics
%_datadir/mapsoft2/typ.txt
%_datadir/mapsoft2/map_templ.htm
%_datadir/xfig/Libraries/*

%changelog
* Sun May 04 2025 Vladislav Zavjalov <slazav@altlinux.org> 2.10-alt1
- vmap2
  - create typ-file source from type configuration
  - rewrite mp/typ FID and CodePage handling 
  - fix angle handling (more accurate now)
  - when writing FIG files do not set MapType=label for text objects, do not require RefType field
  - group/group_ru fields in the type information file
  - fix object comparison (inf, nan values in scale and angle fields)
  - more functions for working with vmap2 object types
  - fix object selection range when updating labels
  - read/write vmap2.gz files (uses iofilters module which is gnu-specific!)
  - (!) use options instead of tags in VMap2 objects
  - write ref_pt field if ref_type is not set (now it's independent parameters)
  - support for multi-line text
  - fix error in line joining (min angle calculation)
- vmap_data:
  - rewrite default type configuration, add new types, split render config
  - settings for mp/typ/img FID and CodePage in vmaps.sh
  - a fiw errors fixed in scripts
- add program ms2vmaptypes for handling vmap types
  add module gis for reading GeoPackage/Postgis
  rewrite rewrite shape module
  start making vmap2 editor (--edit option for ms2vmap)
- rendering vmaps:
   - allow dash, cap, join drawing features for points (useful together with lines feature)
   - add stroke2 drawing feature
   - add stacks for drawing steps (save_to_stack drawing feature and render_stack command)
   - add shift drawing feature
   - add text_vspace drawing feature (multi-line text)
   - fix pattern scaling in "draw_pos fill" feature
   - change logic of move_to/move_from/rotate_to features:
     single list of rules with separate distances,
     all are tried until the first successful one
- map projections:
   - rewrite aliases, allow EPSG:* names
   - add aliases for NO and SE systems
   - support for a few EPSG projections when writing OziExplorer map files
- geo_mkref
  - allow fractional dpi setting in proj mode
  - fix setting border from multiple multy-segment tracks (each segment should form a separate loop)
  - support for --north option in nom_fi reference
- fig
  - fix fig metric resolution (1200dpi/1.05 -> 450)
  - when making compound set coords to zero (we can't calculate it for text; xfig will recalculate zero coords)
- image_tiles:
  - add ms2mbtiles tool
  - mbtiles: sync mode setting with default value 0 (fastest)
  - mbtiles: in layer_write() check if tile exists before deleting it
- geo_data: add geo_nearest_vertex_2d(), geo_maxdist_2d()
- geom: Line constructor with two input iterators
- geom: add dMultiLine::get_first_pt()
- geo_nom_fi: support for 1/2 maps (V5111AL), return scale from nom_to_range_fi
- image_tiles: add {b} special char in the url mask -- EPSG:3857 bbox
- write_geoimg: do not throw error if rendering range is empty (could be a valid situation)

* Fri Jan 03 2025 Vladislav Zavjalov <slazav@altlinux.org> 2.9-alt1
- Improve support for tiled maps. Add MBTiles support
  Now ms2view and ms2render can use mbtiles as input map format,
  ms2render can render tiles directly to mbtiles.
- Support for Finnish standard maps (e.g. V51) and Finnish grid.
- Rewrite map rendering scripts (vmap_data/scripts). Direct rendering
  to mbtiles. Update map data. Important fix of Garmin TYP file.
- Add ms2view desktop (#70, thanks to sikmir)
- Fix and improve contour rendering and smoothing (image_cnt module, ms2geofig program)
- vmap2:
  - allow larger type numbers (3-bytes instead of 2)
  - new features for rendering vmaps: "grid_labels", "draw_pos fill"
  - add a few functions for custom map filtering
  - support for objects types > 0xFFFF
- write_geoimg:
  - mbtiles support
  - default background: transparent for tiled maps, white for others
  - add --fillcolor, --zfill, --swapy options
- viewer:
  - Objects can check drawing range, in advance. This is used in
    tile rendering to avoid reading/creating tiles which are not needed
    (slazav/mapsoft2#61).
  - Get rid of prepare_range mechanism. It was an attempt to do
    parallel downloading of tiles, but it ignored caching and did a lot more
    additional work. Now tile downloading is requested one by one, during
    rendering.
- geo_mkref:
  - rearrange code, move creation of individual reftypes to separate functions
  - fix --border_file option for multi-segment tracks
  - move geo_mkref module inside geo_data
- geom:
  - rect_to_line for empty rectangle gives empty line instead of exception
  - line filtering with Ramer-Douglas-Peucker algorithm
- conv: fix ConvBase::frw_acc for conversions with different x,y,z scaling (such as geo projections)
- image: fix error in coordinate range check
- shape:
  - fix reading objects with nParts=0
  - add shape_dump program
  - add methods for getting dbf field parameters
- pulkovo grid: fix zone boundaries
- rainbow: return lowest color for NaN value
- filename: add ls, glob, remove wrappers
- module system: fix multi-threaded builds (slazav/mapsoft2#69)
- geo_data: fix error in writing Ozi map reference
- jnx module: a playground for jnx format

* Tue Nov 05 2024 Vladislav Zavjalov <slazav@altlinux.org> 2.8-alt1
build system:
 - notests makefile target: much faster build without tests
 - -Wstrict-aliasing=1 by default
err/assert_err, image: fix compilation warnings, remove c++17 features
tmpdir: update for new libzip interface
geom:
 - rect_crop_multi() function
 - Line::area() method
 - simplify check_hole() function
 - fix nearest_pt() for 0- and 1- point lines
 - fix error in join_cross()
geo_data/io_gpx: do not show 1s timestamp in track points (default nakarte timestamp)
fig_geo: add --compound option for fig_add_*() functions
geo_render/gobj_trk: fix speed drawing mode
srtm: a few important fixes
image_cnt: rewrite river/ridge tracing code
vmap2: fix conversion of muplipart objects to fig/gpx
vmap rendering: allow move_to feature for areas and points; add move_from feature
vmap_data:
 - scripts: do not crop maps by default when updating from MP/FIG
 - scripts: always crop map when generating mp/img
 - add source for typ file (2012, A.Tonis, corrections by S.Orlov, S.V.)
 - remove compiled slazav.typ file
 - use line:0x24 (former swamp line) as glacier crevasse
 - render.cfg: draw borders with semi-transparent line
 - render.cfg: move roads from rivers; move summits to auto summits
 - add labels to some point objects
 - tune xfig text size
ms2nom program: --cover_ratio option
ms2geofig: add --compound option for adding fig data

* Fri Jul 26 2024 Vladislav Zavjalov <slazav@altlinux.org> 2.7-alt1
- image_cnt, srtm, ms2geofig: add tracing of rivers and mountain
  ridges (my old code from mapsoft1 with some improvements);
  move code for finding summits to image_cnt; fix SRTM::get_img() method
  and use it for contour and summit tracing to avoid data smoothing;
  cleanup code and tests in image_cnt.
- geom/poly_tools.h: add join_cross function (join crossing parts of a MultiLine);
  use it in vmap2 when saving fig and gpx; use it in ms2geofig when tracing steep slopes.
- image: add ImageR::get_double_range method
- srtm: fix error in deg->m conversion
- downloader: fix test for libcurl>=8.9.0 (fix altlinux.build)
- cairo: remove deprecated functions
- fix a few memory-handling problems (thanks to valgrind)

* Mon Jul 22 2024 Vladislav Zavjalov <slazav@altlinux.org> 2.6-alt1
image:
 - Add support for reading tiled TIFF images
 - An important fix for tiled image data handling (much faster rendering)
 - Fix slazav/mapsoft2#67 error (important for new libtiff)
 - A few fixes and interface changes
image_cnt - new module:
 - Finding contours on the image, smart contour filtering
srtm:
 - Rewrite data handling and interpolation (one more time)
 - Fix data handling for much faster operation
 - Using image_cnt module
 - Using overlay data (see https://github.com/slazav/alos_overlay)
vector maps:
 - remove set_dpi command in types.cfg; set fig font size for each object separately
 - improve default label positions: move to the nearest object point; label_def_mshift option
vmap_data:
 - types.cfg: adjust fig font size, add label_def_mshift for some objects
 - render.cfg: tune fonts for summits and passes
 - always use UTF8 encoding in fig files
 - vmaps_wp_update: use single wpasses.cnv
ms2geofig program:
 - apply --cnt_minpts option only to closed contours
 - new contour finding; add cnt_vtol, scnt_vtol options; remove smooth option
 - --add_comm option (adding additional comments to new fig objects)

* Fri May 31 2024 Vladislav Zavjalov <slazav@altlinux.org> 2.5-alt1
A few bug fixes:
- err: fix problems with NaN values and with multiple evaluations in assert scripts
- geom/line_walker: avoid NaN values for lines with repeated points
- fig: fix scaling/shifting
- srtm: fix error in find_peaks(), fix error with getting points between tiles
- vmap2gobj: fix problem with short_expand feature applied to zero-length lines
- vmap2: optional scale argument to fig_pic command in types file
- viewer/gobj_multi: set trivial cnv in constructor, check for NULL cnv

* Fri May 24 2024 Vladislav Zavjalov <slazav@altlinux.org> 2.4-alt1
- Rewrite SRTM interface (work is not finished). Support for data with
  different resolution, do not use srtm_width.txt file (note that ALOS has
  different resolution at high latitudes, previously it was just
  rescaled). Simplify interpolation code, allow switching between nearest,
  linear, cubic interpolation. Do not use interpolation in graphical
  layers when low-resolution picture is needed. Remove automatic
  interpolation of holes. Use overlays: manual creation/interpolation of
  holes and saving this information. Fix error with drawing of a
  semi-transparent srtm layer. Optimize contour finding.
- Rewrite GeoTrk class: now it is MultiLine<double,GeoTpt>
  instead of GeoTpt array with segment start flags. Massive
  changes in all code working with tracks.
- Add vxi module (VXI-11 interface)
- build system:
  - preliminary check for pkg-config libs.
  - remove support for local pkg-config files
  - PROG_DEPS variable - dependence on executables
- geom:
  - Line/Multiline definition with separate coordinate and point types
    MultiLine<double,GeoTpt>
  - Point, Rect, Line, Multiline: divide/multiply by point
  - Line, Multiline: npts method
  - MultiLine: add_point, add_segment, del_last_point methods
  - poly_tools.h: fix a mess with 2d/non-2d functions, use arbitrary
    distance functions (for using with GeoTrk)
  - poly_tools.h: poly_cross_2d: find crossings of two closed lines
- geo_data/filters: add --trk_reduce_acc, --trk_reduce_num options
- geo_data/geo_utils.h:
  - add geo_length_2d function for Line and MultiLine
  - add geo_bearing_2d function
- fig: write fractional font size
- downloader: set log_level via options; change default 0 (no messages)
- ocad: rewrite code, but still do not include it in vmap system
- vmap2: add dpi setting in type configuration (affects only font size in fig output)
- image:
  - support for 64/48 bit images
  - change internal representation of IMAGE_1 and IMAGE_24 formats
  - fix memory handling
  - support for PNM format 48/24/16/8/1-bit images
  - add 64/48 bit support for tiff format
  - a few image filters (some code transferred from me ph_scan repo):
    image_invert, image_crop, image_autocrop, image_autolevel, image_ir_undust
- a few modifications in vmap-data
- rewrite ms2xyz program,
  - --ref <trk> - calculating distances along a reference track
  - --break segment - restart calculation on every segment
- Add new filters to ms2img program. Now I'm using it to process images
  after film scanning: cropping, color levels, removing dust with IR channel.

* Fri Oct 20 2023 Vladislav Zavjalov <slazav@altlinux.org> 2.3-alt1
- fixes for macOS build (thanks to Nikolay Korotkiy)
- impreve module building system: use make program to extract dependencies from Makefiles.
- fig: support for utf8 files produced by xfig 3.2.9
- srtm: fix 1px shift in y-coordinate
- ocad: transfer code from mapsoft1 (no adaptation for mapsoft2)
- ms2geofig srtm: change scnt_minpts default; apply this filter before
    reducing number of points; --replace option
- Vector maps:
  - Do not connect labels with wrong name if object have other labels with correct name.
  - Render Pulkovo-1942 grid on vector maps (pulk_grid drawing feature).
  - Add label_maxnum drawing feature. Default 0 for labels, 1 for points.
  - Add short_expand and short_skip drawing features. Expand/skip too short lines.
  - Fix clip drawing feature.
  - Remove clip_border parameter. Use "brd clip" drawing feature instead.
  - Allow re-defining variables in config files.
  - Add outer drawing feature border step.
  - Save multisegment objects to FIG/GPX as multiple objects; detect holes when reading.
  - Much faster tile rendering for a small map piece in a large border.
  - Default render configuration: grid, new signs for passes, fix rendering
    of steep glaciers, expand short bridges to 4px, etc.

* Fri Jul 21 2023 Vladislav Zavjalov <slazav@altlinux.org> 2.2-alt1
- Fix hangs and crashes in downloader (thanks to Ilya Kurdyukov)

* Sat Jul 15 2023 Vladislav Zavjalov <slazav@altlinux.org> 2.1-alt1
- Vector maps: fix osm, xfig, and vmap interfaces, add gpx interface.
  New options: wpt_pref, update_type, update_types, label_names, osm_ids, osm_tags, crop_labels.
  Rename options: update_tag, update_type, update_types -> replace_*.
  Use system configuration files from /usr/share/mapsoft2/ by default.
  ifdef/ifndef/else/endif/define/define_if_undef commands in configuration files.
- Update vector map scripts and configuration (vmap_data/scrpts/, vmap_data/)
- ms2view: dialog for editing track point.
- geom: fis line_filter_v1 and line_rectcrop functions.
- Makefile: fix install rules (remove existing dirs).
- Fix for gcc13, include missing cstdint.
- Fix for proj >=9.2.1.
- Update documentation, examples and man pages.

* Sat Dec 03 2022 Vladislav Zavjalov <slazav@altlinux.org> 2.0-alt1
This is a major update with many incompatable and not very
well tested changes, mostly in vector map system.
- New vector map system: vmap2 and vmap2db formats,
  mapdb database is obsoleted.
- New programs ms2vmap and ms2vmapdb instead of ms2mapdb.
- Install all default data and scripts for map rendering.
- New ms2render program for rendering geodata, DEM, vector
  and raster maps; remove this code from ms2conv.
- New syntax of define command in vmap render configuration
  (everything else should work as before).
- ms2geofig: new actions: make_ref, get.
- Update documentation, examples and man pages.

* Sun Oct 23 2022 Vladislav Zavjalov <slazav@altlinux.org> 1.8-alt1
- switch to new libproj API;
- ms2conv: modify --join option
- ms2geofig: add srtm data, add raster maps
- ms2view: a few fixes of panoramic veiw

* Thu Jun 16 2022 Vladislav Zavjalov <slazav@altlinux.org> 1.7-alt1
- Drawing tracks: --trk_draw_width option
- Downloading tiled maps: --insecure, --user_agent, --http_ref options
- Making map reference: --coords_nom, --coords_file options
- TIFF: --tiff_compression option -- support many compression types
- better support for ALOS DEM files
- FIG/GeoFIG support
- fix build with gcc12.1.1

* Sun Apr 18 2021 Vladislav Zavjalov <slazav@altlinux.org> 1.6-alt1
- Drawing maps, tracks, waypoints, srtm-data:
    Fix projection setting if map boundaries can not be converted.
    Fix adjusting waypolit lable positions, avoid infinite loops.
    Fix multi-thread locking in waypoint rendering.
    Fix default track color (blue).
    Fix drawing summit labels on tiles (srtm layer).
    Use srtm_bgcolor if picture is out-of-scale
    When drawing tiled maps do not fail at empty images, return color 0
- Geodata, geo-conversions:
  - Read-only support for Polygon/MultiPolygon features in GeoJSON.
  - New alias: SU<n>N for soviet grid without zone prefix.
- Creating map references (mkref):
  - Allow floating-point arguments in --mag and --dpi options when creating a map.
  - Add --north option for --mkref=nom.
  - Set default border in "tiles" and "proj" modes.
- Vector maps:
  - Add `clip` drawing feature: set clipping for all following steps
  - Value of --clip-border setting is not changing then configuring `brd`
    step, it also affects out-of-scale filling.
  - Add `fit_patt_size` drawing option.
  - Always load patterns at original size.
  - Fix a few problems with pattern rendering.
  - Support lable scaling (import/export/rendering).
- Rendering tiles:
  - Always crop tile range to [0,0,2^z,2^z]
  - Create sub-directories when creating tiles, allow tile templates with subdirs
    (thanks to @ioctl).
  - A few optimizations for creating tile maps (thanks to @ioctl):
    do not create empty tiles, do not re-assemble old tiles in --tmap_scale mode,
    a bit faster color quantization.
- geo_nom, geo_tiles:
  - ms2nom: -W option: use WGS coordinates in calculations.
  - Fix problems with crossing lon=+180/-180 
  - Add --cover option with figure or geo-file argument (thanks to @ioctl).

* Thu Dec 03 2020 Vladislav Zavjalov <slazav@altlinux.org> 1.5-alt1
- Reading/writing geodata:
  - use filename as a waypoint list name when reading waypolints from gpx or wpt,
  - fix writing Ozi map files (Projection Setup line),
  - fix reading track comments from GeoJSON.
- Geodata filtering:
  - add name parameter to --join filter,
  - add --rescale_maps and --shift_maps filters (same as in mapsoft1)
- Update documentation; add geodata_ru.htm text.
- Map rendering:
  - add --map_min_sc, --map_max_sc, --map_def_col options,
  - fix and optimize drawing of multiple maps combined in a single map list.
- ms2view: many tools for editing geodata. Interface is not finished,
  but a few operations are possible: adding/moving/deleting track points,
  adding/moving/deleting/editing waypoints, adding/deleting/editing tracks.
- Fix error handling in libtiff interface.

* Sun Nov 15 2020 Vladislav Zavjalov <slazav@altlinux.org> 1.4-alt1
- GeoJSON format: add extension for writing/reading maps. Now mapsoft2
  geodata structure can be stored in json without losses.
- SRTM layer: fix a few problems: parameter dialog, tile drawing,
  coordinate calculation, srtm_width file reading. Add GeoTiff support.
- Track layer: fix error in color setting. Adding new tracks in the viewer.
- Map layer: fix handling of map lists with multiple maps, fix a few
  problems with switching different viewer projections.
- Maps menu: use {/usr/share/,${HOME}/.}mapsoft2/maps_menu.json
  instead of hardcoded list of maps.
- ms2conv: --srtm option, --htm option. --name, --comm filters.
- Rendering raster images: --add,--title,--title_size options,
  Fix paths in map-files. Fix --mag option. Fix border handling for
  multiple maps.
- MapDB: change configuration file format for MP import/export
  (similar to VMAP import/export). Fix error in geohash calculation.
  Fix label rendering (scale max_text_size properly).
- ANSI escape sequences (\n, \t etc.) and empty words ("") in all
  configuration files (in read_words).
- Avoid throwing c++ exceptions through c code (in image/io_jpeg,io_tiff),
  remove ExcludeArch: armh.
- Fix difference between 32- and 64-bit systems (in image/image_remap).
- Add ms2img program: converting raster images with mapsoft2 libraries.

* Fri Sep 11 2020 Vladislav Zavjalov <slazav@altlinux.org> 1.3-alt1
- viewer: fix a few problems with tiled maps, rescaling and bounding box setting;
  faster srtm layers.
- geodata rendering: fix border rendering, modify options for border setting.
  Allow rendering of tile sets.
- image/colors: fix color handling, add options for loading/saving colormaps.
- mapdb rendering: change reference, border, scale settings.
- spec: add /home/sla/mapsoft2ir %_datadir/mapsoft2 to files section

* Sat Aug 29 2020 Vladislav Zavjalov <slazav@altlinux.org> 1.2-alt1
Viewer:
- SRTM data support (map layer, panoramic view, drowing options dialog)
- Tile map support, improve viewer geo-reference algorythms
- "Track drawing options" dialog
- "Show point information" dialog
- "Use map reference" action
Other:
- fix error with reading integer values from GeoJSON
- `SU` projection: Pulkovo CS with automatic zone number
- fix loading PNG images with depth<8

* Mon Jun 22 2020 Vladislav Zavjalov <slazav@altlinux.org> 1.1.1-alt2
- ExcludeArch: armh (libjpeg + c++ exceptions problem)

* Sun Jun 21 2020 Vladislav Zavjalov <slazav@altlinux.org> 1.1.1-alt1
- update documentation, add GB projection alias, further development

* Sat Apr 04 2020 Vladislav Zavjalov <slazav@altlinux.org> 1.1-alt2
- fix build on i586 (rounding errors in modules/geom_tools/np.test.cpp)

* Sat Apr 04 2020 Vladislav Zavjalov <slazav@altlinux.org> 1.1-alt1
- add ms2mapdb program - work with mapsoft2 vector maps
  (conversion to/from mp/vmap, rendering)
- basic support for mapdb in ms2view
- ms2proj: --scale, --shift options
- update documentation
- change packaging, use git-submodules, fix a few errors.

* Wed Oct 09 2019 Vladislav Zavjalov <slazav@altlinux.org> 1.0-alt1
- v1.0 - first release, first build for altlinux
