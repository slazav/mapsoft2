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
uploading back to GPS. When we started creating our own maps:
<a href="https://nakarte.me/#m=7/55.61249/37.65564&l=O/Z">hiking maps
of Moscow region</a> and some <a href="https://nakarte.me/#m=2/20.13847/57.48047&l=O/Q">
mountain regions</a>.
All programs for working with geodata, raster and vector maps, SRTM-data,
etc, have been collected to _MAPSOFT1_ project.
<b>Mapsoft2</b> is an attempt to rewrite these programs fixing some
fundamental problems and improving the code.
',`
Мы начали писать программы для работы с геоданными и картами в
1997-1998 годах, когда в нашей походной компании появились первые
GPS-приемники. Сперва это были программы для скачивания треков, рисования
их на карте, редактирования и загрузки в GPS. Потом мы начали рисовать свои
собственные карты: <a href="https://nakarte.me/#m=7/55.61249/37.65564&l=O/Z">
походные карты Подмосковья</a> и некоторых <a href="https://nakarte.me/#m=2/20.13847/57.48047&l=O/Q">
горных районов</a>. Все программы для работы с геоданными, растровыми и векторными картами,
SRTM-данными и т.п. были собраны в проект _MAPSOFT1_.
<b>Mapsoft2</b> -- это попытка аккуратно переписать эти программы,
исправляя разные ошибки, заложенные в архитектуре, и улучшая код.
')dnl


<h2>ENRU(`Links', `Ссылки')</h2>

<ul>

<li><a href="https://github.com/slazav/mapsoft2">
ENRU(`Source code, bug tracker (github).', `Исходный код, баг-треккер (github).')
</a>

<li><a href="https://groups.google.com/forum/#!forum/mapsoft2">
ENRU(`Discussions (google group).', `Обсуждения (гугл-группа).')
</a>

</ul>

<p>ENRU(`Documentation', `Документация')</h2>

<ul>

<li>ENRU(`<a href="modules.htm">Source code library</a>',
        `<a href="modules.ru.htm">Библиотека исходного кода</a>')

<li><a href="geodata.htm">
  ENRU(`Geodata in mapsoft2 (RU)',
       `Геоданные в mapsoft2')</a>

<li><a href="vmap2.htm">
  ENRU(`Vector maps in mapsoft2 (RU)',
       `Векторные карты в mapsoft2')</a>

<li><a href="vmap2render.htm">
  ENRU(`Vector maps in mapsoft2: rendering raster images (RU)',
       `Векторные карты в mapsoft2: рендер растровых изображений')</a>

<li><a href="diff.htm">
    ENRU(`Differences between mapsoft1 and mapsoft2 (RU)',
         `Различия mapsoft1 и mapsoft2')</a>
</ul>

<p>ENRU(`man-pages', `man-страницы (EN)')</h2>

define(_PROG_,`<b><a href="man/$1.htm"><tt>$1</tt></a></b>')

<ul>

<li>STABLE _PROG_(ms2conv) -- ENRU(`Converter for geodata and raster maps
.', `Преобразования геоданных и растровых карт.')

<li>UNSTABLE _PROG_(ms2view) -- ENRU(`Viewer for geodata and raster maps.',
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
<div align=right>Vladislav Zavjalov, slazav at altlinux dot org</div>
