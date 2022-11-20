PAGES := index modules
PAGES_RU := vmap2 vmap2render diff geodata

HTMLS := $(patsubst %,%.htm,$(PAGES))\
         $(patsubst %,%.ru.htm,$(PAGES))\
         $(patsubst %,%.htm,$(PAGES_RU))

all: $(HTMLS)


%.ru.htm: defs.m4 %.m4
	m4 -DLANG=RU $+ > $@

%.htm: defs.m4 %.m4
	m4 -DLANG=EN $+ > $@

mapdb_ru.htm: defs.m4 mapdb_ru.m4
	m4 -DLANG=RU $+ > $@

clean:
	rm -f *.htm