# configuration file for vector map factory

VMAP_DIR=vmap;   # Source folder (vmap/vmap2)
IN_DIR=IN;       # Input folder. Maps are updated from .fig/.mp files in this folder
OUT_DIR=OUT;     # Output folder for raster maps, mp/img files
DIFF_DIR=diff;   # Folder for diff files
FIG_DIR=fig;     # Fig templates
BRD_DIR=brd;     # Global borders in gpx format -- for tiles
BRD2_DIR=brd;    # Global borders in gpx format -- for index files
CROP_NOM=0;      # crop maps in vmaps_in (only for nomenclature maps)

VMAP_EXT=vmap;       # Extension/format of vector maps
NOM_MAG=2;           # Scaling of maps. Use NOM_MAP=2 for 1km sheets with 500m maps
DPI=400;             # DPI for rendering images
DPI_PR=150;          # DPI for rendering preview images
DPI_MAP=200;         # DPI for "original" map reference (normally 200 or 100)
STYLE_HR=0;          # hr variable to be used in render.cfg and types.cfg
STYLE_V2_PER=0;      # new style of passes
GRID=0;              # draw grid

CMAP=conf/cmap.png;        # Colormap
CMAP_SRC=                  # nomenclatere name used for colormap source
REND_CFG=/usr/share/mapsoft2/render.cfg;  # Render configuration
TYPEINFO=/usr/share/mapsoft2/types.cfg    # Type information
HTM_TEMPL=/usr/share/mapsoft2/map_templ.htm;    # template for htm page

# index files (one set per BRD2_DIR/*.gpx)
TYPSRC=/usr/share/mapsoft2/typ.txt  # typ file source for garmin IMG
EXTRA_TRACKS=;    # extra tracks to be added to index image
JPEG_SCALE=0.2;   # scale for jpeg preview images
INDEX_SCALE=0.05; # scale for index image (in addition to jpeg_scale)

# tiles (single set for all maps)
TILE_DIR=TILES;
SQLITEDB=$OUT_DIR/map.sqlitedb;
MBTILES=$OUT_DIR/map.mbtiles;
MBTILES_NAME="slazav"
MBTILES_ATTR="http://slazav.xyz/maps/"
TLIST=$TILE_DIR/tile.list;
TILE_MAXZ=14;  # max z-index
TILE_MAXE=6    # max index for empty map

# programs (can be redifined in the local configuration)
MS2VMAP=ms2vmap
MS2RENDER=ms2render
MS2GEOFIG=ms2geofig
MS2CONV=ms2conv
MS2NOM=ms2nom

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
  tmppng=$(maketemp vmaps_cmap_XXXXXX.png)
  $MS2RENDER $vmap --out $tmppng\
    --config "$REND_CFG" -t "$TYPEINFO" --define "$(vmap_defs "$name")"\
    --mkref nom --north --name "$name" --dpi $DPI --margins 10 --top_margin 30\
    --title "$name" --title_size 20\
    --cmap_save $cmap --png_format pal --png_format pal
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
    --config "$REND_CFG" -t "$TYPEINFO" --define "$(vmap_defs "$name")"\
    --mkref nom --north --name "$name" --dpi "$dpi" --margins 10 --top_margin 30\
    --title "$title" --title_size 20\
    --cmap_load "$CMAP" --png_format pal ${map:+--map $map}
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
