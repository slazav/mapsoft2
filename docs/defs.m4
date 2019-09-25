divert(-1)

#define(L_RU, `ifelse(LANG,RU,`$1')')
#define(L_EN, `ifelse(LANG,EN,`$1')')
#define(L_PR, `ifelse(LANG,RU,`.ru',`')')

define(ENRU, `ifelse(LANG,EN,`$1',`$2')')

##################
## HEADER(fname, title)
define(HEADER, `
<div align=right>
ENRU(`Language:',`Язык:')
ifelse(LANG,RU,`<b>RU</b>',
               `<a href="$1.ru.htm">RU</a>')
ifelse(LANG,EN,`<b>EN</b>',
               `<a href="$1.htm">EN</a>')
</div>
ifelse($1, index, `',`
ENRU(`<a href="index.htm">Main page...</a>',
`<a href="index.ru.htm">На главную...</a>')
')

<h2>$2</h2>
')
##################

define(_MOD_, `<b><a href="https://github.com/slazav/mapsoft2/tree/master/modules/$1">$1</a></b>')

define(_LIBICONV_, `<a href="https://www.gnu.org/software/libiconv/">libiconv</a>')
define(_GNULIB_, `<a href="https://www.gnu.org/software/gnulib/">gnulib</a>')
define(_MAPSOFT1_, `<a href="https://github.com/ushakov/mapsoft">mapsoft</a>')
define(_PKGCONFIG_, `<a href="https://www.freedesktop.org/wiki/Software/pkg-config/">pkg-config</a>')
define(_LIBZIP_, `<a href="https://libzip.org/">libzip</a>')
define(_LIBPROJ_, `<a href="https://proj4.org/">libproj</a>')
define(_LIBCAIRO_, `<a href="https://www.cairographics.org/')
define(_GEOHASH_, `<a href="https://en.wikipedia.org/wiki/Geohash">Geohash</a>')
define(_SHAPELIB_, `<a href="http://shapelib.maptools.org/">shapelib</a>')

define(WWW, `<a href="$1">ifdef(`$2',`$2',`$1')</a>')

define(STABLE,   `<img src="img/stable.png" alt="stable">&nbsp;')
define(UNSTABLE, `<img src="img/unstable.png" alt="unstable">&nbsp;')
define(UNUSABLE, `<img src="img/unusable.png" alt="unusable">&nbsp;')
##################

divert(0)dnl
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<html>
<head>
  <title>mapsoft2 documentation</title>
  <meta name="keywords" content="mapsof2">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  ifelse(LANG,RU,`<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>')
</head>
<body bgcolor="#FFFFFF">

divert(10)dnl
</body>
</html>
divert
