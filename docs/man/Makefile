# pages generated from help messages
SRC1A := ms2conv ms2view ms2xyz ms2mkref ms2vmap ms2proj
## separate man-pages
SRC1B := ms2nom ms2tiles
SRC5  := mapsoft2


POD1A:= $(patsubst %, %.pod,   $(SRC1A))
POD1B:= $(patsubst %, %.pod,   $(SRC1B))
POD5 := $(patsubst %, %.pod,   $(SRC5))

MAN1 := $(patsubst %, %.1,   $(SRC1A) $(SRC1B))
MAN5 := $(patsubst %, %.5,   $(SRC5))
HTML := $(patsubst %, %.htm, $(SRC1A) $(SRC1B) $(SRC5))

all: $(MAN1) $(MAN5) $(HTML)

$(POD1A) $(POD1B) $(POD5): $(INC)

%.1 %.5: %.pod
	pod2man  $< -n $* -c mapsoft2 -r mapsoft2 > $@

%.htm: %.pod
	pod2html $< --title $* --css manpages.css > $@
	rm -f pod2htmd.tmp

clean:
	rm -f *.{1,5} *.htm *.pod

########################################

# pages generated from help messages
$(POD1A): %.pod: %.txt foot.txt
	make -C ../../programs/$*
	(../../programs/$*/$* --pod; cat $< foot.txt) > $@

## separate man-pages
$(POD1B) $(POD5): %.pod: %.txt foot.txt
	cat $< foot.txt > $@
