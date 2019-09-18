PROGRAMS := ms2conv ms2mkref ms2nom ms2tiles ms2view ms2xyz
# ms2proj  -- to be written
# ms2vmap -- not usable yet
# ms2fig  -- to be written

#################################
## define paths

bindir  ?= /usr/bin

#################################
## Build rules

all: $(PROGRAMS)
ms2%:
	make -C programs/$@

#################################
## Install rules

install:
	for i in $(PROGRAMS); do\
	  install -D -m755 programs/$$i/$$i $(bindir)/$$i;\
	done
