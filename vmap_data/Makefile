P = ../programs/ms2vmapdb/ms2vmapdb

prefix  ?= /usr
bindir  ?= $(prefix)/bin
datadir ?= $(prefix)/share

.PHONY: all install pics clean
all: figlib_podm figlib_hr pics

figlib_podm: types.cfg
	${P} fig_lib -t types.cfg -D '{"hr":"0"}' -o figlib_podm

figlib_hr: types.cfg
	${P} fig_lib -t types.cfg -D '{"hr":"1"}' -o figlib_hr
pics:
	make -C pics

install:
	install -D -m755 scripts/* $(bindir);\
	install -D -m644 slazav.typ map_templ.htm render.cfg types.cfg $(datadir)/mapsoft2/
	mkdir -p $(datadir)/mapsoft2
	rm -rf $(datadir)/mapsoft2/pics $(datadir)/xfig/Libraries/{hr,podm}
	cp -r pics $(datadir)/mapsoft2/pics
	mkdir -p $(datadir)/xfig/Libraries
	cp -r figlib_hr $(datadir)/xfig/Libraries/hr
	cp -r figlib_podm $(datadir)/xfig/Libraries/podm
clean:
	rm -rf figlib_hr figlib_podm
