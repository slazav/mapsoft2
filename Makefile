PROGRAMS := ms2conv ms2mapdb ms2nom ms2proj ms2tiles ms2view ms2xyz

# ms2fig  -- to be written

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


all: $(PROGRAMS) man
ms2%:
	make -C programs/$@
man:
	make -C docs/man

clean:
	make -C programs clean
	make -C modules  clean

#################################
## Install rules

install:
	for i in $(PROGRAMS); do\
	  install -D -m755 programs/$$i/$$i $(bindir)/$$i;\
	  install -D -m644 docs/man/$$i.1 $(man1dir)/$$i.1;\
	done
	install -D -m644 docs/man/mapsoft2.5 $(man5dir)/mapsoft2.5
	install -D -m644 programs/ms2view/mapsoft2.css $(datadir)/mapsoft2/mapsoft2.css
