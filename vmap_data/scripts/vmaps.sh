# configuration file for vector map factory

VMAP_DIR=vmap;   # Source folder (vmap/vmap2)
IN_DIR=IN;       # Input folder. Maps are updated from .fig/.mp files in this folder
OUT_DIR=OUT;     # Output older for raster maps, mp/img files
DIFF_DIR=diff;   # Difference
FIG_DIR=fig;     # Fig templates
TILE_DIR=TILES;  # Tiles
BRD_DIR=brd;     # Global borders in gpx format -- for tiles
BRD2_DIR=brd;    # Global borders in gpx format -- for index files

VMAP_EXT=vmap;             # Extension/format of vector maps
NOM_MAG=2;     # Scaling of maps. Use NOM_MAP=2 for 1km sheets with 500m maps
DPI=400;
DPI_PR=150;
CMAP=conf/cmap.png;        # Colormap
CMAP_SRC=                  # nomenclatere name 
REND_CFG=conf/render.cfg;  # Render configuration
TYPEINFO=conf/types.cfg    # Type information
HTM_TEMPL=conf/map.htm;    # template for htm page
TYP=conf/slazav.typ
EXTRA_TRACKS="conf/MO.plt conf/MKAD.plt"

JPEG_SCALE=0.2;   # scale for jpeg preview images
INDEX_SCALE=0.05; # scale for index image (in addition to jpeg_scale)

SQLITEDB=$OUT_DIR/map.sqlitedb;
MBTILES=$OUT_DIR/map.mbtiles;
TLIST=$TILE_DIR/tile.list;

# programs (can be redifined in the local configuration)
MS2VMAP=ms2vmap
MS2RENDER=ms2render
MS2GEOFIG=ms2geofig
MS2CONV=ms2conv
MS2NOM=ms2nom

function vmap_update_cmap() {
  name=$1
  cmap=$2
  vmap=$VMAP_DIR/$name.$VMAP_EXT
  $MS2RENDER $vmap --out tmp_cmap.png --config "$REND_CFG" -t "$TYPEINFO"\
    --define "{\"nom_name\":\"$name\", \"border_style\":\"normal\"}"\
    --mkref nom --north --name "$name" --dpi $DPI --margins 10 --top_margin 30\
    --title "$name" --title_size 20\
    --cmap_save $cmap --png_format pal --png_format pal
  rm -rf tmp_cmap.png
}

function vmap_render_map() {
  ifile=$1
  name=$2
  png=$3
  map=$4
  title=$5
  dpi=${6:-$DPI}
  $MS2RENDER $ifile --out "$png" --config "$REND_CFG" -t "$TYPEINFO"\
    --define "{\"nom_name\":\"$name\", \"border_style\":\"normal\"}"\
    --mkref nom --north --name "$name" --dpi "$dpi" --margins 10 --top_margin 30\
    --title "$title" --title_size 20\
    --cmap_load "$CMAP" --png_format pal --map "$map"
}
