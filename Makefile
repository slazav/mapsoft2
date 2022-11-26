PROGRAMS :=\
  ms2conv\
  ms2geofig\
  ms2img\
  ms2mp\
  ms2nom\
  ms2proj\
  ms2render\
  ms2tiles\
  ms2view\
  ms2vmap\
  ms2vmapdb\
  ms2xyz\

#################################
## define paths

prefix  ?= /usr
bindir  ?= $(prefix)/bin
datadir ?= $(prefix)/share
mandir  ?= $(datadir)/man
man1dir ?= $(mandir)/man1
man5dir ?= $(mandir)/man5


#################################
## Build rules

.PHONY: all man vmap_data clean
all: $(PROGRAMS) man vmap_data

ms2%:
	make -C programs/$@
man:
	make -C docs/man
vmap_data:
	make -C vmap_data

clean:
	make -C programs clean
	make -C modules  clean
	make -C vmap_data clean

#################################
## Install rules

install:
	for i in $(PROGRAMS); do\
	  install -D -m755 programs/$$i/$$i $(bindir)/$$i;\
	  install -D -m644 docs/man/$$i.1 $(man1dir)/$$i.1;\
	done
	install -D -m644 docs/man/mapsoft2.5 $(man5dir)/mapsoft2.5
	install -D -m644 programs/ms2view/mapsoft2.css   $(datadir)/mapsoft2/mapsoft2.css
	install -D -m644 programs/ms2view/maps_menu.json $(datadir)/mapsoft2/maps_menu.json
	make -C vmap_data install

