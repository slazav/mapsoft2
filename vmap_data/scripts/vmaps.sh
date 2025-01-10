# configuration file for vector map factory

# put vmaps.sh file in your map folder to overwrite some of these settings

# Map name. Used in
# - map name in mbtiles database
# - map name in img file
MAP_NAME="default_map"

# Map attribution. Used in
# - man attribution in mbtiles database
MAP_ATTR="mapsoft"

############################################################
## Map source configuration
VMAP_DIR=vmap;   # Source folder
VMAP_EXT=vmap;   # Extension/format of vector maps (could be vmap2, vmap, or even mp)

REND_CFG=/usr/share/mapsoft2/render.cfg;  # Render configuration
TYPEINFO=/usr/share/mapsoft2/types.cfg    # Type information

############################################################
## Image rendering
BRD_DIR=brd;     # map borders in gpx format
CMAP=cmap.png;   # Colormap
CMAP_SRC=        # map name used as colormap source

# render parameters:
DPI_MAP=200;     # DPI for "original" map reference (normally 200 or 100)
DPI=400;         # DPI for rendering images
STYLE_HR=0;      # hr variable to be used in render.cfg and types.cfg
STYLE_V2_PER=1;  # new style of passes
GRID=1;          # draw grid

# png files
OUT_DIR=OUT;     # Output folder for raster maps (if empty then raster images are not rendered)

# tiles
TILE_DIR=TILES;  # tiles (if empty, tiles are not rendered)
TILE_MAXZ=14;    # max z-index
TILE_MAXE=6      # max index for empty map
SQLITEDB="";
MBTILES="";
TLIST="";

############################################################
## IMG rendering (see vmaps_img script)
IMG_DIR=OUT;   # img files (if empty, img files are not rendered)
TYPSRC=/usr/share/mapsoft2/typ.txt;  # typ file source for garmin IMG
GMT=gmt;                             # gmt program
CGPSM=cgpsmapper-static              # cgpsmapper program
IMGMAP="";                           # combined img file
IMG_FID=891;                         # FID parameter for IMG files
IMG_CP=1251;                         # CodePage parameter for MP/TYP/IMG files

############################################################
## Map editing
IN_DIR=IN;       # Input folder. Maps are updated from .fig/.mp files in this folder
FIG_DIR=fig;     # Folder for Fig templates
CROP_NOM=0;      # crop maps in vmaps_in (only for nomenclature maps)
NOM_MAG=2;       # Scaling of maps. Use NOM_MAP=2 for 1km sheets with 500m maps

DIFF_DIR=diff;   # Folder for diff files
DPI_DIFF=150;    # DPI for rendering diff images

REG_DIR=;        # borders for region index files (to be removed?)

# vmaps_wp_update
WPASS_DIR=wpasses
WPASS_PREF=wpasses

############################################################
## Generating index files:
EXTRA_TRACKS=;    # extra tracks to be added to index image
JPEG_SCALE=0.2;   # scale for jpeg preview images
INDEX_SCALE=0.05; # scale for index image (in addition to jpeg_scale)

# new index files (not used yet)
INDEX_TXT="";
INDEX_HTM="";

# programs (can be redifined in the local configuration)
MS2VMAP=ms2vmap
MS2RENDER=ms2render
MS2GEOFIG=ms2geofig
MS2CONV=ms2conv
MS2NOM=ms2nom

######################################################################

function vmap_defs() {
  name="$1"
  border_style="${2:-normal}"
  echo "{\"nom_name\":\"$name\", \"border_style\":\"$border_style\", "\
       " \"dpi_val\":\"$DPI_MAP\", \"hr\":\"$STYLE_HR\", "\
       " \"v2_per\":\"$STYLE_V2_PER\", \"grid\":\"$GRID\"}"
}

function vmap_update_cmap() {
  name=$1
  cmap=$2
  vmap=$VMAP_DIR/$name.$VMAP_EXT
  tmppng=$(mktemp vmaps_cmap_XXXXXX.png)
  $MS2RENDER $vmap --out $tmppng\
    --config "$REND_CFG" -t "$TYPEINFO" --define "$(vmap_defs "$name")"\
    --mkref nom --north --name "$name" --dpi $DPI --margins 10 --top_margin 30\
    --title "$name" --title_size 20\
    --cmap_save $cmap --png_format pal --cmap_add 0 --cmap_colors 254
  rm -rf $tmppng
}

function vmap_render_map() {
  ifile=$1
  name=$2
  png=$3
  map=$4
  title=$5
  dpi=${6:-$DPI}
  $MS2RENDER $ifile --out "$png"\
    $BRD_DIR/*.gpx --trk_draw_transp 0.5 --trk_draw_dots 0 --trk_draw_width 3\
    --config "$REND_CFG" -t "$TYPEINFO" --define "$(vmap_defs "$name")"\
    --mkref nom --north --name "$name" --dpi "$dpi" --margins 10 --top_margin 30\
    --title "$title" --title_size 20\
    --cmap_load "$CMAP" --png_format pal ${map:+--map $map}
}

function vmap_render_tiles() {
  name=$1
  brd=$2
  vmap=$VMAP_DIR/$name.$VMAP_EXT
  $MS2RENDER $vmap\
    --config "$REND_CFG" -t "$TYPEINFO" --define "$(vmap_defs "$name" clip)"\
    --tmap --add --out "$TILE_DIR/{x}-{y}-{z}.png"\
    --zmin 0 --zfill $TILE_MAXE --zmax $TILE_MAXZ --fillcolor 0xFFBF6000\
    --png_format pal --cmap_load $CMAP\
    --border_file $brd\
    --tmap_scale 1 --fit_patt_size;\
}

function vmap_render_mbtiles() {
  name=$1
  brd=$2
  vmap=$VMAP_DIR/$name.$VMAP_EXT
  $MS2RENDER $vmap\
    --config "$REND_CFG" -t "$TYPEINFO" --define "$(vmap_defs "$name" clip)"\
    --add --out "$MBTILES"\
    --zmin 0 --zfill $TILE_MAXE --zmax $TILE_MAXZ --fillcolor 0xFFBF6000\
    --png_format pal --cmap_load $CMAP\
    --border_file $brd\
    --tmap_scale 1 --fit_patt_size;\
}

function vmap_render_mp() {
  name=$1
  mp=$2
  mp_id=$3
  vmap="$VMAP_DIR/$name.$VMAP_EXT"
  $MS2VMAP "$vmap" -o "$mp"\
    -t "$TYPEINFO" --define "$(vmap_defs "$name")"\
    --mp_name "$name" --mp_id "$mp_id" --mp_cp "$IMG_CP" --crop_nom "$name"
}

function list_vmaps {
 find $VMAP_DIR -maxdepth 1 -name "*.$VMAP_EXT" | sort
}

# list all vmaps which are newer then png or map
function list_vmap_nt_png {
  for vmap in $(list_vmaps); do
    png="$OUT_DIR/$(basename $vmap .$VMAP_EXT).png"
    jpg="$OUT_DIR/$(basename $vmap .$VMAP_EXT).jpg"
    map="$OUT_DIR/$(basename $vmap .$VMAP_EXT).map"
    [ "$vmap" -nt "$png" -o "$vmap" -nt "$map" -o "$vmap" -nt "$jpg" ] && echo "$vmap" ||:
  done
}

# list all vmaps which are newer then img or mp.zip
function list_vmap_nt_img {
  for vmap in $(list_vmaps); do
    img="$IMG_DIR/$(basename $vmap .$VMAP_EXT).img"
    mpz="$IMG_DIR/$(basename $vmap .$VMAP_EXT).mp.zip"
    [ "$vmap" -nt "$img" -o "$vmap" -nt "$mpz" ] && echo "$vmap" ||:
  done
}

# list all vmaps which are newer then tile tstamp file
function list_vmap_nt_tiles {
  for vmap in $(list_vmaps); do
    tst="$TILE_DIR/$(basename $vmap .$VMAP_EXT).tstamp"
    [ "$vmap" -nt "$tst" ] && echo "$vmap" ||:
  done
}

# list all vmaps which are newer then MBTILES
function list_vmap_nt_mbtiles {
  for vmap in $(list_vmaps); do
    [ "$vmap" -nt "$MBTILES" ] && echo "$vmap" ||:
  done
}

# list all vmaps which are newer then INDEX_TXT
function list_vmap_nt_index {
  for vmap in $(list_vmaps); do
    [ "$vmap" -nt "$INDEX_TXT" ] && echo "$vmap" ||:
  done
}

# check if a file exists and is newer then any png
function check_nt_png {
  n="$(set +f; ls -1t $OUT_DIR/*.png | head -1)"
  [ -f "$1" -a "$1" -nt "$n" ]
}

# check if a file exists and is newer then any tiles
function check_nt_tiles {
  n="$(set +f; ls -1t $TILE_DIR/*.tstamp | head -1)"
  [ -f "$1" -a "$1" -nt "$n" ]
}

# check if a file exists and is newer then any img
# we want to use only img which correspond to vmaps
function check_nt_img {
  f="$1"
  for vmap in $(list_vmaps); do
    img="$IMG_DIR/$(basename $vmap .$VMAP_EXT).img"
    [ "$f" -nt "$img" ] || f=$img
  done
  [ "$f" = "$1" ]
}

# Print date of the last commit for a given file
function vmap_git_date {
  f="$1"
  [ -f "$f" ] || return 1
  t="$(git log --format="%at" -n1 -- "$f" ||:)"
  [ "$t" == "" ] || date -d "@$t" "+%Y-%m-%d"
}

# Return true if file exists and has clean git status
function vmap_git_status {
  f="$1"
  [ -f $f ] || return 1
  st="$(git status --porcelain -- "$f")"
  [ "$st" = "" ]
}

# Check that git is clean for a list of files
# (names without paths or extensions are allowed, vmap files are checked)
function vmap_git_status_list {
  clean=1
  for i in "$@"; do
    name=${i%.*}
    name=${name##*/}
    vmap="$VMAP_DIR/$name.$VMAP_EXT"

    if ! vmap_git_status $vmap; then
      echo "NON-CLEAN GIT STATUS: $name"
      clean=""
    fi
  done
  [ "$clean" = 1 ]
}

