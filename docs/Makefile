PAGES := index modules
PAGES_RU := vmap2 vmap2_2020 vmap2render diff geodata vmap2types

HTMLS := $(patsubst %,%.htm,$(PAGES))\
         $(patsubst %,%.ru.htm,$(PAGES))\
         $(patsubst %,%.htm,$(PAGES_RU))

all: $(HTMLS) types_render


%.ru.htm: defs.m4 %.m4
	m4 -DLANG=RU $+ > $@

%.htm: defs.m4 %.m4
	m4 -DLANG=EN $+ > $@

mapdb_ru.htm: defs.m4 mapdb_ru.m4
	m4 -DLANG=RU $+ > $@

vmap2types.m4: ../vmap_data/types.cfg types2html
	./types2html ../vmap_data/types.cfg > vmap2types.m4

types_render: ../vmap_data/types.cfg
	../programs/ms2vmaptypes/ms2vmaptypes render --types ../vmap_data/types.cfg\
	  --config ../vmap_data/render.cfg --obj_scale 2 -o types_render

clean:
	rm -f *.htm