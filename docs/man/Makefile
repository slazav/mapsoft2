# pages generated from help messages
SRC1 := ms2conv ms2view ms2xyz ms2vmap ms2proj ms2tiles ms2nom
## separate man-pages
SRC5 := mapsoft2


POD1 := $(patsubst %, %.pod,   $(SRC1))
POD5 := $(patsubst %, %.pod,   $(SRC5))

MAN1 := $(patsubst %, %.1,   $(SRC1))
MAN5 := $(patsubst %, %.5,   $(SRC5))
HTML := $(patsubst %, %.htm, $(SRC1) $(SRC5))

all: $(MAN1) $(MAN5) $(HTML)

%.1 %.5: %.pod
	pod2man  $< -n $* -c mapsoft2 -r mapsoft2 > $@

%.htm: %.pod
	pod2html $< --title $* --css manpages.css > $@
	rm -f pod2htmd.tmp

clean:
	rm -f *.{1,5} *.htm *.pod

########################################

# pages generated from help messages
$(POD1): %.pod: %.txt foot.txt
	(../../programs/$*/$* --pod; cat $< foot.txt) > $@

## separate man-pages
$(POD5): %.pod: %.txt foot.txt
	cat $< foot.txt > $@
