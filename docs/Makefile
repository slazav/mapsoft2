PAGES := index modules mapdb

HTMLS := $(patsubst %,%.htm,$(PAGES))\
         $(patsubst %,%.ru.htm,$(PAGES))

all: $(HTMLS)


%.ru.htm: defs.m4 %.m4
	m4 -DLANG=RU $+ > $@

%.htm: defs.m4 %.m4
	m4 -DLANG=EN $+ > $@

clean:
	rm -f *.htm