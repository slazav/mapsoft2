HEADER(modules,`
  ENRU(`Mapsoft2 source code library',`Библиотека исходного кода Mapsoft2')
')

<p>ENRU(`
Mapsoft2 code is organized as a set of separate "modules". Each
module can contain its own documentation, tests, programs, etc.. Some of
there modules are stable and used in my other projects, others may be
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

<h3>ENRU(`Building system',`Сборочная система')</h3>

<p>ENRU(` The building system is based on GNU make. All modules are
located in <tt>modules</tt> directory. Programs, libraries and modules
which are not intended to be used by other modules can be located
anywhere (see <tt>programs</tt> directory). Each module has a
<tt>Makefile</tt> there a few variables are set to declare module
components and <tt>Makefile.inc</tt> from the <tt>modules</tt> directory
is included. For building dependency tree a separate script is used:
<tt>modules/get_deps</tt>. For correct dependency search including a
header file <tt>file.h</tt> from a module <tt>name</tt> should be always
written as <tt>#include "name/file.h"</tt>. Including local header file
should be written as <tt>#include "file.h"</tt>.',`

Сборочая система сделана на основе GNU make. Модули расположены в
директории <tt>modules</tt>. Программы, библиотеки и модули, которые  не
используются другими модулями, могут быть расположены в любом месте (см.,
например, директорию <tt>programs</tt>). Каждый модуль содержит
<tt>Makefile</tt> где описаны его компоненты и включен файл
<tt>Makefile.inc</tt> из директории <tt>modules</tt>. Для построения
дерева зависимостей используется отдельный скрипт
<tt>modules/get_deps</tt>. Для правильного поиска зависимостей
подключение заголовочного файла <tt>file.h</tt> из модуля <tt>name</tt>
надо записывать в виде <tt>#include "name/file.h"</tt>, а локальных
заголовочных файлов - в виде <tt>#include "file.h"</tt>.')

<h3>ENRU(`Git and gears',`Git и gears')</h3>

<p>ENRU(`Modules are located in a separate git repository,
and included into <tt>mapsoft2</tt> as git submodule.',`
Модули хранятся в отдельном git-репозитории и подключаются к <tt>mapsoft2</tt>
как git submodule.')

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

<li><tt>PKG_CONFIG   -- ENRU(
`External libraries used in this module (for _PKGCONFIG_). Local pkg-config
files can be put in <tt>$(MODDIR)/pc/</tt>.',
`Внешние библиотеки используемые в этом модуле (подключаются с помощью _PKGCONFIG_).
Локальные файлы для pkg-config можно положить в директорию <tt>$(MODDIR)/pc/</tt>.')
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

_MODTAB_(fig_opt, UNSTABLE,
`Extension for FIG format for keeping data in object comments.',
`Расширение формата FIG для хранения данных в комментариях объектов.')

_MODTAB_(filename, STABLE,
`Functions for working with filenames.',
`Функции для работы с именами файлов.')

_MODTAB_(geo_data, STABLE,
`Classes for geodata handling. Functions for reading and writing geodata
(GPX, KML, KMZ, GeoJson, OziExplorer formats). Great-circle distance (Haversine
formula). Geo-conversions (_LIBPROJ_ wrapper). ',
`Классы для работы с геоданными. Чтение и запись (форматы GPX,
KML, KMZ, GeoJson, OziExplorer). Расстояния между точками на поверхности Земли
(формула Гаверсинуса). Геодезические преобразования (обертка для
библиотеки _LIBPROJ_).')
<br>WWW(`https://github.com/slazav/mapsoft2/blob/master/modules/geo_data/Formats.md',
ENRU(`-> Mapsoft2 geodata, supported file formats',
`-> Геоданные в Mapsoft2, поддерживаемые форматы файлов'))

_MODTAB_(geo_mkref, STABLE, `Making map references.',`Изготовление
привязок карт.')

_MODTAB_(geo_nom, STABLE,
`Functions for working with Soviet nomenclature map names.',
`Функции для работы с именами советских номенклатурных карт.')

_MODTAB_(geo_render, UNSTABLE,
`Rendering geodata, map grids, vector maps...',
`Рисование геоданных, сеток, векторных карт на растровых картинках.')

_MODTAB_(geo_tiles, STABLE,
`Functions for working with TMS/Google tiles.',
`Функции для работы с тайлами TMS/Google.')

_MODTAB_(geohash, STABLE,
`_GEOHASH_ library and simple data storage with spatial indexing',
`Библиотека _GEOHASH_ и простое хранилище данных с гео-индексацией.')

_MODTAB_(geom, STABLE,
`Geometrical forms: Point, Line, Multiline, and Rect classes. Many useful functions for
working with them. LineWalker class.',
`Геометрические объекты: классы Point, Line, Multiline и Rect. Множество
полузных функция для работы с ними. Класс LineWalker.')

_MODTAB_(geom_tools, UNSTABLE,
`Additional functions for working with geometric objects.',
`Дополнительные функции для работы с геометрическими объектами.')

_MODTAB_(getopt, STABLE,
`getopt wrapper for building complicated command-line interfaces.',
`Обертка getopt для изготовления сложных интерфейсов командной строки.')

_MODTAB_(iconv, STABLE,
`Character encoding converter, a wrapper for _LIBICONV_.',
`Преобразование кодировок, обертка _LIBICONV_.')

_MODTAB_(image, UNSTABLE,
`2d array of data for raster images. Reading and writing raster images
(PNG, TIFF, JPEG, GIF), color quantization and other operations.',
`Двухмерный массив данных для хранения растровых изображений. Чтение и
запись растровых изображений (PNG, TIFF, JPEG, GIF), квантизация цветов и
другие операции.')
<br>WWW(`https://github.com/slazav/mapsoft2/blob/master/modules/image/Readme.md',
ENRU(`-> Mapsoft2 image, supported file formats',
`-> Растровые изображения в Mapsoft2, поддерживаемые форматы файлов'))

_MODTAB_(iofilter, STABLE,
`Filtering <tt>std::stream</tt> through external program.',
`Перенаправление <tt>std::stream</tt> через внешнюю программу.')

_MODTAB_(jsonxx, STABLE,
`Simple C++ wrapper for libjansson library.',
`Простая C++ обертка для библиотеки libjansson.')

_MODTAB_(mapdb, UNUSABLE,
`BerkleyDB-based local storage for vector maps with fast access and spatial indexing.',
`Локальное хранилище векторных карт с быстрым доступом и гео-индексацией на основе BerkleyDB.')

_MODTAB_(mapsoft_data, UNSTABLE,
`Mapsoft global data structure and read/write function. Should work with
all supported formats.',
`Глобальная структура данных mapsoft и функции чтения/записи. Должна
работать со всеми поддерживаемыми форматами.')

_MODTAB_(mapview, UNSTABLE,
`Viewer for maps and geodata. For use in ms2view program.',
`Окно вьюера геоданных и карт. Для использования в программе ms2view.')

_MODTAB_(mp, STABLE,
`Reading and writing of MP files. MP is a vector map format used for
compiling Garmin GPS maps.',
`Чтение и запись MP-файлов. MP -- это формат векторных карт, используемый для
компиляции карт для Garmin GPS.')

_MODTAB_(opt, STABLE,
`A <tt>map&lt;string,string&gt;<tt> container with functions for
getting/putting values of arbitrary types. Used widely in
<tt>mapsoft</tt>.',
`Контейнер <tt>map&lt;string,string&gt;<tt> с функциями для записи и
чтения различных типов данных. Широко используется в <tt>mapsoft</tt>.')

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

_MODTAB_(shape, STABLE,
`Reading and writing of Shape-files, wrapper for _SHAPELIB_.',
`Чтение и запись SHAPE-файлов, обертка для _SHAPELIB_.')

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
использования находятся в директории <tt>examples/viewer/</tt>.')

_MODTAB_(vmap, STABLE,
`Reading and writing of VMAP, old mapsoft format for vector maps.',
`Чтение и запись VMAP, старого формата mapsoft для векторных карт.')

</table>

</ul>
