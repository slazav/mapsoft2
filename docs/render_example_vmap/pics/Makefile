PICS := avt kladb cross cerkov pam ohotn bash shaht razv dom trig trig_hr kan\
        ldp per per1a per1b per2a per2b per3a per3b pernk kladb pupyr yama skala\
        bol_l bol_h redk vyr_n vyr_o ledn sand zd ur_vod por vdp pesch\
        rossip1 rossip2 sand_ov camp

PNG := $(patsubst %,%.png,$(PICS))
SVG := $(patsubst %,%.svg,$(PICS))

all: $(SVG)
clean:
	rm -f *.png *.svg *.gif

# fig2dev -t option only allowed for GIF transparent color
%.gif: %.fig
	fig2dev -m7.5 -Lgif -t#ffffff "$<" > "$@"

%.png: %.gif
	convert -trim "$<" -repage +0+0 PNG32:"$@"

%.svg: %.fig
	fig2dev -m7.5 -Lsvg "$<" > "$@"
