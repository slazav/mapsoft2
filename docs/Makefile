PAGES := index modules mapdb

HTMLS := $(patsubst %,%.htm,$(PAGES))\
         $(patsubst %,%.ru.htm,$(PAGES))

all: $(HTMLS) mapdb_ru.htm


%.ru.htm: defs.m4 %.m4
	m4 -DLANG=RU $+ > $@

%.htm: defs.m4 %.m4
	m4 -DLANG=EN $+ > $@

mapdb_ru.htm: defs.m4 mapdb_ru.m4
	m4 -DLANG=RU $+ > $@

clean:
	rm -f *.htm