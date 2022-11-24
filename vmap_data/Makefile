P = ../programs/ms2vmapdb/ms2vmapdb

figlib: figlib_podm figlib_hr

figlib_podm: types.cfg
	${P} fig_lib -t types.cfg -D '{"hr":"0"}' -o figlib_podm

figlib_hr: types.cfg
	${P} fig_lib -t types.cfg -D '{"hr":"0"}' -o figlib_hr