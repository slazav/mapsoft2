# pages generated from help messages
SRC1 := ms2conv ms2geofig ms2img ms2mp ms2nom ms2proj\
        ms2render ms2tiles ms2view ms2vmap ms2vmapdb ms2xyz

## separate man-pages
SRC5 := mapsoft2

prefix  ?= /usr
bindir  ?= $(prefix)/bin
datadir ?= $(prefix)/share
mandir  ?= $(datadir)/man
man1dir ?= $(mandir)/man1
man5dir ?= $(mandir)/man5

POD1 := $(patsubst %, %.pod,   $(SRC1))
POD5 := $(patsubst %, %.pod,   $(SRC5))

MAN1 := $(patsubst %, %.1,   $(SRC1))
MAN5 := $(patsubst %, %.5,   $(SRC5))
HTML := $(patsubst %, %.htm, $(SRC1) $(SRC5))

all: $(MAN1) $(MAN5) $(HTML)

%.1: %.pod
	pod2man  $< -n $* -c mapsoft2 -r mapsoft2 > $@

%.5: %.pod
	pod2man  $< -n $* -c mapsoft2 -r mapsoft2 > $@

%.htm: %.pod
	pod2html $< --title $* --css manpages.css > $@
	rm -f pod2htmd.tmp

install: $(MAN1) $(MAN5)
	for i in $(MAN1); do install -D -m644 $$i $(man1dir)/$$i; done
	for i in $(MAN5); do install -D -m644 $$i $(man5dir)/$$i; done

clean:
	rm -f *.{1,5} *.htm *.pod

########################################

# pages generated from help messages
$(POD1): %.pod: %.txt foot.txt
	(../../programs/$*/$* --pod; cat $< foot.txt) > $@

## separate man-pages
$(POD5): %.pod: %.txt foot.txt
	cat $< foot.txt > $@
