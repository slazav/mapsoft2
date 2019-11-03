HEADER(index, Mapsoft2)

<p>
ENRU(`
A collection of tools and libraries for working with maps and geo-data.
',`
Набор библиотек и программ для работы с геоданными и картами.
')dnl

<!--#################################################################-->
<h2>ENRU(`About',`О проекте')</h2>

ENRU(`
We started writing programs for handling GPS-data and maps since
first GPS-receivers appeared in our hiking group at 1997-1998. First it
was tools for downloading tracks, putting them on a map, editing, and
uploading back to GPS. When we started creating our own maps: hiking maps
of Moscow region and some mountain regions:
',`
Мы начали писать программы для работы с геоданными и картами в
1997-1998 годах, когда в нашей походной компании появились первые
GPS-приемники. Сперва это были программы для скачивания треков, рисования
их на карте, редактирования и загрузки в GPS. Потом мы начали рисовать свои
собственные карты: походные карты Подмосковья и некоторых горных районов:
')dnl

<ul>
ENRU(`
<li><a href="https://nakarte.me/#m=7/55.61249/37.65564&l=O/Z">Maps of Moscow region at nakarte.me</a>
<li><a href="http://slazav.mccme.ru/maps/podm_txt.htm">About Moscow region maps (RU)</a>
<li><a href="https://nakarte.me/#m=2/20.13847/57.48047&l=O/Q">Mountain maps at nakarte.me</a>
',`
<li><a href="https://nakarte.me/#m=7/55.61249/37.65564&l=O/Z">Карты Подмосковья на сайте nakarte.me</a>
<li><a href="http://slazav.mccme.ru/maps/podm_txt.htm">О картах Подмосковья</a>
<li><a href="https://nakarte.me/#m=2/20.13847/57.48047&l=O/Q">Карты горных районов на сайте nakarte.me</a>
')dnl
</ul>

<p>
ENRU(`
All programs for working with geodata, raster and vector maps, SRTM-data,
etc, have been collected to _MAPSOFT1_ project.
',`
Все программы для работы с геоданными, растровыми и векторными картами,
SRTM-данными и т.п. были собраны в проект _MAPSOFT1_.
')dnl

<p>ENRU(
`<b>Mapsoft-2</b> is an attempt to rewrite these programs fixing some
fundamental problems and improving the code.',
`<b>Mapsoft-2</b> -- это попытка аккуратно переписать эти программы,
исправляя разные ошибки, заложенные в архитектуре, и улучшая код.')dnl

<h2>ENRU(`Links', `Ссылки')</h2>

<ul>

<li><a href="https://github.com/slazav/mapsoft2">
ENRU(`Source code, bug tracker (github).', `Исходный код, баг-треккер (github).')
</a>

<li><a href="https://groups.google.com/forum/#!forum/mapsoft2">
ENRU(`Discussions (google group).', `Обсуждения (гугл-группа).')
</a>

<li><a href="https://github.com/ushakov/mapsoft">
ENRU(`Old mapsoft code (github).',`Старый исходный код (mapsoft)')
</a>

</ul>

<!--#################################################################-->
<h2>ENRU(`Source code library', `Библиотека исходного кода')</h2>

ENRU(
` Mapsoft consists of many different pieces of code: simple and sophisticated,
stable and experimental, good and bad. Some of this pieces are also used
in other projects, not related to maps. I found very useful to make a
"source code library" which consists of separate "modules" (something
like _GNULIB_). Each module has its own documentation, tests, and it can
be easily transferred to another project.',
` Mapsoft состоит из множества разнообраных частей: простых и сложных,
стабильных и меняющихся, хороших и плохих. Некоторые из этих частей также
используются в других проектах, никак не связанных с картами.
Оказалось очень удобным организовать все в виде "библиотеки исходного
кода", состоящей из отдельных модулей (что-то вроде _GNULIB_). Каждый
модуль содержит собственную документацию, тесты, программы и может быть
легко перенесен в другой проект вместе со своими зависимостями.')dnl

<br><a href="modules.htm"><b>ENRU(`Read more...', `Подробнее...')</b></a>


<!--#################################################################-->
<h2>ENRU(`Programs', `Программы')</h2>

<ul>

define(_PROG_,`<b><a href="man/$1.htm"><tt>$1</tt></a></b>')

<li>STABLE _PROG_(ms2conv) -- ENRU(`Converter for geodata and raster maps
.', `Преобразования геоданных и растровых карт.')

<li>STABLE _PROG_(ms2view) -- ENRU(`Viewer for geodata and raster maps.',
`Вьюер для геоданных и растровых карт.')

<li>STABLE _PROG_(ms2nom) -- ENRU(`Program for soviet nomenclature map
calculations. Coordinates of a map sheet, adjacent sheets, sheets
covering a given region, etc.', `Программа для вычислений, связанных с
советской номенклатурой карт: координаты данного листа, соседние листы,
покрывающие данный район листы и т.п.')

<li>STABLE _PROG_(ms2tiles) -- ENRU(`Program for TMS/Google tile
calculations. Coordinates of a tile, tiles covering a given region,
etc.', `Программа для вычислений, связанных с тайлами TMS/Google:
координаты данного тайла, покрывающие данный район тайлы и т.п.')

<li>STABLE _PROG_(ms2xyz) -- ENRU(`Program for writing tracks as
text tables. Time, distance, velocity calculations, adjustable format.',
`Программа для вывода треков в виде текстовой таблицы. Вычисления времен,
расстояний, скоростей, настраиваемый формат.')

<li>STABLE _PROG_(ms2proj) -- ENRU(`Coordinate converter.',
`Преобразование координат.')

<li>UNUSABLE _PROG_(ms2vmap) -- ENRU(`Program for working with vector
maps.', `Программа для работы с векторными картами.')

</ul>

<p>ENRU(`Here STABLE - more or less stable programs, UNSTABLE - unstable
programs, interface will be changed, UNUSABLE - unusable programs.',`
Здесь STABLE - более-менее стабильные программы, UNSTABLE - нестабильные
программы, интерфейс которых будет меняться, UNUSABLE - недоделанные программы,
которые пока нельзя использовать.')

<!--#################################################################-->
<h2>ENRU(`Supported file formats', `Поддерживаемые форматы файлов')</h2>

<ul>
<li>ENRU(`Geodata',`Геоданные'): OziExplorer, GPX, KML, GarminUtils, GeoJSON.
<li>ENRU(`Raster images',`Растрове изображения'): JPEG, PNG, TIFF, GIF.
<li>FIG     - ENRU(`vector graphics format', `формат векторной графики')
<li>MP      - ENRU(`"Polish" format for vector maps', `"Польский" формат векторных карт')
<li>VMAP    - ENRU(`mapsoft1 format for vector maps', `формат векторных карт в mapsoft1')
<li>SHP+DBF - ENRU(`format for vector map data', `формат данных для векторных карт')
<li><a href="mapdb.htm">MapDB</a> --
ENRU(`Mapsoft2 native format for vector maps.',
`Формат mapsoft2 для хранения векторных карт.')</a>
</ul>

<a href="mapdb_ru.htm"> ENRU(`Vector maps in mapsoft2 (RU)',`Векторные карты в Mapsoft2')</a>

<!--#################################################################-->
<div align=right>Vladislav Zavjalov, slazav at altlinux dot org</div>
