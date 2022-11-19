## Real example for vector map rendering.

Run scripts in the following order

#### 1. update_vmap2:

Download vmap files from github:slazav/map_podm, github:slazav/map_hr if
they are missing (`<name>.vmap`). Convert to VMap2 databases (`*.vmap2db, *.vmap2gh`)
and to vmap2 text files (`*.vmap2`).

#### 2. render_image

Render images (`<name>.png`) and OziExplorer references (`<name>.map`).
For one of the maps use border from `border.gpx` file. For both maps create
colormaps (`<name>.cmap.png`) files. Add transparent colors to colormaps,
this will be needed to generate tiles with transparent background.

#### 3. render_tiles

Render tiles (`<name>.tiles/`) using colormaps from the previous step.