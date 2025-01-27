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
  ms2vmaptypes\
  ms2xyz\

prefix  ?= /usr
bindir  ?= $(prefix)/bin
datadir ?= $(prefix)/share

all:
	sh -e -c 'for i in $(PROGRAMS); do make -C $$i; done'

install:
	sh -e -c 'for i in $(PROGRAMS); do install -D -m755 $$i/$$i $(bindir)/$$i; done'
	install -Dm644 ms2view/mapsoft2.css -t $(datadir)/mapsoft2
	install -Dm644 ms2view/maps_menu.json -t $(datadir)/mapsoft2
	install -Dm644 ms2view/ms2view.desktop -t $(datadir)/applications

clean:
	sh -e -c 'for i in $(PROGRAMS); do make -C $$i clean; done'
