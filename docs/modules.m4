HEADER(modules,`
  ENRU(`Mapsoft2 source code library',`Библиотека исходного кода Mapsoft2')
')

<p>ENRU(`
Mapsoft2 code is organized as a set of separate "modules". Each
module can contain its own documentation, tests, programs, etc.. Some of
these modules are stable and used in my other projects, others may be
experimental, unstable, or even useless. Modules may depend on each other.
Makefile-based building system allows to build programs and libraries
which depend only on certain modules and to transfer modules between
different projects.
',`
Исходный код Mapsoft2 организован в виде набора отдельных
"модулей", каждый из которых может содержать код, документацию, тесты,
программы и т.п. Некоторые из этих модулей стабильны и используются мной
в разных проектах, другие могут быть экспериментальными, нестабильными
или бесполезными. Модули могут зависеть друг от друга. Сборочная система
(использующая make) позволяет собирать программы и библиотеки используя
только необходимые модули, а также переносить модули между различными
проектами.
')dnl

<p>ENRU(`
Modules are located in WWW(`https://github.com/slazav/mapsoft2-libs').
They can be used as git submodule or copied into a project.
',`
Модули находятся в WWW(`https://github.com/slazav/mapsoft2-libs'). Их можно подключить
в виде git submodule или cкопировать в проект.
')

<p>ENRU(`Use of git submodules is not supported by Altlinux building
system (<tt>gear</tt> program). It wants to have all source code
accessible from a certain git commit. This problem
is solved by packing all modules in a separate tarball (see <tt>update_modules</tt>
script) and keeping it in <tt>mapsoft2</tt> repository. If you do not
use <tt>gear</tt> for building the package, you can ignore this file.
',`
Использование git submodules не поддержиивается сборочной системой Altlinux
(а именно, программой gear). Она хочет, чтобы весь исходный код был доступен
из определенного коммита в git-репозитории. Эта проблема решается запаковкой
всех модулей в отдельный tar-файл (см. скрипт <tt>update_modules</tt>) и
хранением этого файла в репозитории <tt>mapsoft2</tt>. Если вы не используете
<tt>gear</tt> для сборки пакета, то этот файл можно игнорировать.
')

<h3>ENRU(`Building system',`Сборочная система')</h3>

<p>ENRU(` The building system is based on GNU make. All modules are
located in a single directory (usually <tt>modules</tt>). Programs, libraries and modules
which are not intended to be used by other modules can be located
anywhere. Each module has a <tt>Makefile</tt> there a few variables are
set to declare module components and <tt>Makefile.inc</tt> from the
<tt>modules</tt> directory is included. For building dependency tree a
separate script is used: <tt>modules/get_deps</tt>. For correct
dependency search including a header file <tt>file.h</tt> from a module
<tt>name</tt> should be always written as <tt>#include
"name/file.h"</tt>. Including local header file should be written as
<tt>#include "file.h"</tt>.',`

Сборочая система сделана на основе GNU make. Модули расположены в одной
директории (обычно <tt>modules</tt>). Программы, библиотеки и модули,
которые  не используются другими модулями, могут быть расположены в любом
месте. Каждый модуль содержит <tt>Makefile</tt> где описаны его
компоненты и включен файл <tt>Makefile.inc</tt> из директории
<tt>modules</tt>. Для построения дерева зависимостей используется
отдельный скрипт <tt>modules/get_deps</tt>. Для правильного поиска
зависимостей подключение заголовочного файла <tt>file.h</tt> из модуля
<tt>name</tt> надо записывать в виде <tt>#include "name/file.h"</tt>, а
локальных заголовочных файлов - в виде <tt>#include "file.h"</tt>.')

define(NAME, `<tt>&lt;name&gt;$1</tt>')dnl

<p>ENRU(`In <tt>Makefile</tt> of a module following variables can be defined:',
`В <tt>Makefile</tt> модуля могут быть определены следующие переменные:')

<ul>

<li><tt>MODDIR</tt> -- ENRU(
`Directory where all modules are located, <tt>".."</tt> by default.',
`Директория, в которой расположены модули, по умолчанию: <tt>".."</tt>.')

<li><tt>MOD_HEADERS</tt> -- ENRU(
`List of module headers (<tt>*.h</tt>).',
`Cписок заголовочных файлов модуля (<tt>*.h</tt>).')

<li><tt>MOD_SOURCES</tt> -- ENRU(
`List of module sources (<tt>*.cpp</tt>).',
`Список файлов с исходным кодом (<tt>*.cpp</tt>).')

<li><tt>PROGRAMS</tt> -- ENRU(
`Programs to be build. For each program NAME() a
source file NAME(.cpp) should exist.',
`Список программ, которые надо собрать. Для каждой программы NAME() должен
существовать файл с исходным кодом NAME(.cpp)')

<li><tt>SHARED_LIB</tt> -- ENRU(
`Shared library to build (all MOD_SOURCES of this module and
all its dependencies are linked together).',
`Разделяемые библиотеки, которые надо собрать (собираются все исходники
<tt>MOD_SOURCES</tt> из этого модуля и всех модулей, от которых он
зависит).')

<li><tt>SIMPLE_TESTS</tt> -- ENRU(
`Programs returning 0/1 which should be built and run. For each NAME it
should be a source NAME(.test.cpp).',
`Программы, возвращающие 0 или 1, которые
надо собрать и запустить. Для каждой программы NAME должен существовать
файл с исходным кодом NAME(.test.cpp).')

<li><tt>SCRIPT_TESTS</tt> -- ENRU(`Programs with testing scripts. For
each NAME a source file NAME(.test.cpp) and a script
NAME(.test.script) should exist. Program will be built and script will be run.',
`Программы с тестовыми скриптами. Для каждой программы NAME должен
существовать файл с исходным кодом NAME(.test.cpp) и скрипт
NAME(.test.script). Программа будет собрана, после сборки будет запущен
тестовый скрипт.')

<li><tt>OTHER_TESTS</tt> -- ENRU(
`Any programs or scripts which should be run after building.',
`Программы или скрипты, которые должны быть запущены после окончания сборки.')

<li><tt>PKG_CONFIG</tt>   -- ENRU(
`External libraries used in this module (for _PKGCONFIG_).',
`Внешние библиотеки используемые в этом модуле (подключаются с помощью _PKGCONFIG_).')

<li><tt>LDLIBS, LDFLAGS, CXXFLAGS</tt> -- ENRU(
`standard Makefile variables',
`стандартные переменные make').

</ul>


<h3>ENRU(`Projects which use mapsoft2-libs', `Пректы, использующие mapsoft2-libs')</h3>
<p>as git submodule:
<ul>
<li>mapsoft2: WWW(`https://github.com/slazav/mapsoft2')
<li>graphene: WWW(`https://github.com/slazav/graphene')
<li>device2:    WWW(`https://github.com/slazav/device2')
</ul>
<p>copy of some modules:
<ul>
<li>pico_osc: WWW(`https://github.com/slazav/pico_osc')
</ul>


<h3>ENRU(`Modules', `Модули')</h3>
define(_MODTAB_, `<tr><td valign=top>$2`'_MOD_($1)</td><td> ENRU(`$3',`$4')</td></tr>')
<table border=0 cellpadding=1>

_MODTAB_(cache, STABLE,
`Cache of arbitrary objects with automatic cleaning (based on object
number or their size).',
`Кэш объектов с автоматическим удалением (в зависимости от количества
объектов или их суммарного размера).')

_MODTAB_(cairo, UNSTABLE,
`Wrapper for _LIBCAIRO_.', `Обертка для _LIBCAIRO_.')

_MODTAB_(conv, STABLE,
`Coordinate conversions for geometric objects.',
`Преобразования координат геометрических объектов.')

_MODTAB_(downloader, UNSTABLE,
`A multy-thread download manager.',
`Многопоточный менеджер загрузок.')

_MODTAB_(err, STABLE,
`Simple class for throwing errors with human-readable descriptions.
Used in many of my projects.',
`Простой класс для генерации исключений с текстовыми сообщениями.
Используется во многих моих проектах.')

_MODTAB_(fig, STABLE,
`Reading and writing of FIG files. Fig is an old vector graphics format
which I also use for drawing maps.',
`Чтение и запись FIG-файлов. Fig - это древний векторный графический
формат, который я использую, в частности, для рисования карт.')

_MODTAB_(fig_geo, STABLE,
`Extension for FIG format for keeping georeference and geodata.',
`Расширение формата FIG для хранения геоданных и привязки.')

_MODTAB_(fig_opt, STABLE,
`Extension for FIG format for keeping data in object comments.',
`Расширение формата FIG для хранения данных в комментариях объектов.')

_MODTAB_(filename, STABLE,
`Functions for working with files and filenames.',
`Функции для работы с файлами и именами файлов.')

_MODTAB_(geo_data, STABLE,
`Classes for geodata handling. Functions for reading and writing geodata
(GPX, KML, KMZ, GeoJson, OziExplorer formats). Great-circle distance (Haversine
formula). Geo-conversions (_LIBPROJ_ wrapper). Making map references.
<br>WWW(`https://github.com/slazav/mapsoft2-libs/blob/master/geo_data/Formats.md',
`-> Mapsoft2 geodata, supported file formats').
',
`Классы для работы с геоданными. Чтение и запись (форматы GPX,
KML, KMZ, GeoJson, OziExplorer). Расстояния между точками на поверхности Земли
(формула Гаверсинуса). Геодезические преобразования (обертка для
библиотеки _LIBPROJ_).Изготовление
привязок карт.
<br>WWW(`https://github.com/slazav/mapsoft2-libs/blob/master/geo_data/Formats.md',
`-> форматы геоданных в Mapsoft2')
')

_MODTAB_(geo_nom, STABLE,
`Functions for working with Soviet and Finnish nomenclature map names.',
`Функции для работы с именами советских и финских номенклатурных карт.')

_MODTAB_(geo_render, UNSTABLE,
`Rendering geodata, map grids, raster maps...',
`Рисование геоданных, сеток, растровых карт на растровых картинках.')

_MODTAB_(geo_tiles, STABLE,
`Functions for working with TMS/Google tiles.',
`Функции для работы с тайлами TMS/Google.')

_MODTAB_(geohash, STABLE,
`_GEOHASH_ library and simple data storage with spatial indexing',
`Библиотека _GEOHASH_ и простое хранилище данных с гео-индексацией.')

_MODTAB_(geom, STABLE,
`Geometrical forms: Point, Line, Multiline, and Rect classes. Many useful functions for
working with them.',
`Геометрические объекты: классы Point, Line, Multiline и Rect. Множество
полезных функция для работы с ними.')

_MODTAB_(getopt, STABLE,
`getopt wrapper for building complicated command-line interfaces.',
`Обертка getopt для изготовления сложных интерфейсов командной строки.')

_MODTAB_(gis, UNSTABLE,
`Reading some GIS formats (GeoPackage etc.)',
`Чтение некоторых форматов данных GIS (GeoPackage и т.п.')

_MODTAB_(iconv, STABLE,
`Character encoding converter, a wrapper for _LIBICONV_.',
`Преобразование кодировок, обертка _LIBICONV_.')

_MODTAB_(image, UNSTABLE,
`2d array of data for raster images. Reading and writing raster images
(PNG, TIFF, JPEG, GIF), color quantization and other operations.
<br>WWW(`https://github.com/slazav/mapsoft2-libs/blob/master/image/Readme.md',
`-> Mapsoft2 image, supported file formats')
',
`Двухмерный массив данных для хранения растровых изображений. Чтение и
запись растровых изображений (PNG, TIFF, JPEG, GIF), квантизация цветов и
другие операции.
<br>WWW(`https://github.com/slazav/mapsoft2-libs/blob/master/image/Readme.md',
`-> Mapsoft2 image, supported file formats')
')

_MODTAB_(image_cnt, UNSTABLE,
`Tracing contours, rivers etc.',
`Трассировка контуров, рек и т.п.'
)

_MODTAB_(image_tiles, UNSTABLE,
`Tiled images (local, remote, mbtiles).',
`Тайловые картинки (локальные, удаленные, mbtiles).'
)

_MODTAB_(iofilter, STABLE,
`Filtering <tt>std::stream</tt> through external program.',
`Перенаправление <tt>std::stream</tt> через внешнюю программу.')

_MODTAB_(jnx, UNSTABLE,
`JNX map format'
`Формат карт JNX')

_MODTAB_(jsonxx, STABLE,
`Simple C++ wrapper for libjansson library.',
`Простая C++ обертка для библиотеки libjansson.')

_MODTAB_(log, STABLE,
`A simple way of writing log messages to stdin or file.',
`Простой способ вывода сообщений в консоль или в файл.')

_MODTAB_(mapview, UNSTABLE,
`Viewer for maps and geodata. For use in ms2view program.',
`Окно вьюера геоданных и карт. Для использования в программе ms2view.')

_MODTAB_(mp, STABLE,
`Reading and writing of MP files. MP is a vector map format used for
compiling Garmin GPS maps.',
`Чтение и запись MP-файлов. MP -- это формат векторных карт, используемый для
компиляции карт для Garmin GPS.')

_MODTAB_(ocad, UNSTABLE,
`OCAD map format'
`Формат карт OCAD')

_MODTAB_(opt, STABLE,
`A <tt>map&lt;string,string&gt;</tt> container with functions for
getting/putting values of arbitrary types. Used widely in
<tt>mapsoft</tt>.',
`Контейнер <tt>map&lt;string,string&gt;</tt> с функциями для записи и
чтения различных типов данных. Широко используется в <tt>mapsoft</tt>.')

_MODTAB_(osmxml, UNSTABLE,
`Reading OSM XML files.',
`Чтение файлов OSM XML.')

_MODTAB_(rainbow, STABLE,
`Convert double values into RGB color gradients. Useful code which I use
in many places.',
`Преобразовать вещественное число в цвет в соответствии с некоторым
заданным градиентом. Полезная библиотека, которую я использую в разных
местах.')

_MODTAB_(read_words, STABLE,
`Read one line from the <tt>std::istream</tt> and extract words,
separated by spaces. Comments, quoting, escape symbols are supported.
Similar to reading shell argument list.',
`Прочитать строчку из <tt>std::istream</tt> и разбить ее на слова,
разделенные пробелами. Понимает комментарии, кавычки, escape-символ \.
Похоже на чтение строки параметров в shell.')

_MODTAB_(shape, UNSTABLE,
`Reading and writing of Shape-files, wrapper for _SHAPELIB_.',
`Чтение и запись SHAPE-файлов, обертка для _SHAPELIB_.')

_MODTAB_(srtm, STABLE,
`Working with digital elevation models (hgt anf tiff formats).',
`Работа с картами высот (форматы hgt и tiff).')

_MODTAB_(time_fmt, STABLE,
`Functions for reading and writing time in different forms.',
`Функции для чтения и записи времени в различных форматах.')

_MODTAB_(tmpdir, STABLE,
`Class for working with temporary directories and zip files. Create a
temporary directory, fill it with files, zip everything, unzip files to
directory etc. Directory and all files will be deleted in the
destructor. Uses _LIBZIP_ library.',
`Класс для работы со временной директорией и zip-файлами. Можно создать
временную директорию, положить туда файлы, и зазиповать их, можно
раззиповать файлы во временную динекторию. Директория будет удалена при
уничтожении объекта. Использует библиотеку _LIBZIP_.')

_MODTAB_(viewer, STABLE,
`GTK-based viewer for objects which can draw on a raster
image. "Rubber lines" and "actions" for making interactive interfaces
(for example, drawing and editing geometric forms). Examples can be found
in <tt>viewer/examples</tt> folder.',
`Вьюер (GTK) для объектов, которые умеют рисовать растровые изображения.
"Резиновые линии" и "действия" для изготовления интерактивных интерфейсов
(например, рисование и редактирование геометрических объектов). Примеры
использования находятся в директории <tt>viewer/examples</tt>.')

_MODTAB_(vmap, STABLE,
`Reading and writing of VMAP, old mapsoft format for vector maps.',
`Чтение и запись VMAP, старого формата mapsoft для векторных карт.')

_MODTAB_(vmap2, STABLE,
`VMAP2 vector maps, new format (text/database), all operations with this format.',
`VMAP2, новый формат векторных карт (текстовый или база данных) и все операции с ним.')

_MODTAB_(vxi, STABLE,
`Access to VXI devices',
`Доступ к устройствам VXI')

</table>

</ul>
