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
	install -Dm644 docs/man/mapsoft2.5 -t $(man5dir)
	install -Dm644 programs/ms2view/mapsoft2.css -t $(datadir)/mapsoft2
	install -Dm644 programs/ms2view/maps_menu.json -t $(datadir)/mapsoft2
	install -Dm644 programs/ms2view/ms2view.desktop -t $(datadir)/applications
	make -C vmap_data install

