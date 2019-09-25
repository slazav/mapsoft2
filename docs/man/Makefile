SRC1 := ms2conv ms2view ms2nom ms2tiles ms2xyz ms2mkref
SRC5 := mapsoft2

MAN1 := $(patsubst %, %.1,   $(SRC1))
MAN5 := $(patsubst %, %.5,   $(SRC5))
HTML := $(patsubst %, %.htm, $(SRC1) $(SRC5))

all: $(MAN1) $(MAN5) $(HTML)

$(MAN1) $(MAN5) $(HTML): foot.m4 opt_general.m4 opt_mkref.m4

%.1 %.5: %.m4
	m4 $< | pod2man -n $* -c mapsoft2 -r mapsoft2 > $@

%.htm: %.m4
	m4 $< | pod2html --title $* --css manpages.css > $@
	rm -f pod2htmd.tmp

clean:
	rm -f *.{1,5} *.htm
