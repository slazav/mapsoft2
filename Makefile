PROGRAMS := ms2conv ms2mkref ms2nom ms2tiles ms2view ms2xyz
# ms2proj  -- to be written
# ms2vmap -- not usable yet
# ms2fig  -- to be written

#################################
## define paths

bindir  ?= /usr/bin
datadir ?= /usr/share
mandir  ?= /usr/share/man
man1dir ?= $(mandir)/man1
man5dir ?= $(mandir)/man5


#################################
## Build rules

all: $(PROGRAMS) man
ms2%:
	make -C programs/$@
man:
	make -C programs/man

#################################
## Install rules

install:
	for i in $(PROGRAMS); do\
	  install -D -m755 programs/$$i/$$i $(bindir)/$$i;\
	  install -D -m644 docs/man/$$i.1 $(man1dir)/$$i.1;\
	done
	install -D -m644 programs/man/mapsoft2.5 $(man5dir)/mapsoft2.5
	install -D -m644 programs/ms2view/mapsoft2.css $(datadir)/mapsoft2/mapsoft2.css
