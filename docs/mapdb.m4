HEADER(mapdb,`
  ENRU(`MapDB file format',`формат файла MapDB')
')

<p>ENRU(`MapDB is a mapsoft2 native format for vector maps. Internaly it
is a folder with a few BerkleyDB databases:', `MapDB - формат для
хранения векторных карт в mapsoft2. Он представляет из себя директорию с
несколькими базами данных BerkleyDB:')

<ul>
<li> mapinfo.db -- ENRU(`map information',`информация о карте')
<li> objects.db -- ENRU(`object data',`информация об объектах')
<li> geohash.db -- ENRU(`spatial index data',`данные для гео-индексации')
<li> labels.db  -- ENRU(`label data',`информация о подписях')
</ul>

<p>ENRU(`In many cases a RIFF-style packing of data is used: 4-byte tag
("crds", "name", etc.), 4-byte data size (in bytes), data.', `Во многих
случаях используется упаковка данных в стиле RIFF: 4-символьный тэг
("crds", "name" и т.п.), 4-байтовое число - длина данных в байтах,
данные.')

<h2>ENRU(`mapinfo.db',`База данных mapinfo.db')</h2>

<p>ENRU(`Database with 4-byte integer key for keeping general
map information. Following keys are supported:',
`База данных с 4-байтовым целочисленным ключем
для хранения информации о карте. Поддерживаются следующие номера ключей:')

<ul>

<li>0. ENRU(`MapDB version, integer in a string form. Currently "0".',
`Версия формата MapDB, целое число записанное в виде строки. Сейчас "0".')

<li>1. ENRU(`Map name (string).', `Название карты (строка).')

<li>2. ENRU(`Map border. Multi-segment line (dMultiLine object) packed
in the RIFF-style form with "crds" tag for each segment. Each segment
contains pairs of coordinates (lon1,lat1, lon2,lat2,...).
Coordinates are in WGS84 degrees multiplied by 1e7 and rounded to the
nearest signed 4-byte integer. The map border can be set by user and have any
value or be empty.',
`Граница карты. Многоугольник с несколькими сегментами (объект
dMultiLine) упакованный в стиле RIFF с тэгом "crds" для каждого
сегмента. Сегмент состоит из пар координат (lon1,lat1,
lon2,lat2,...), Координаты - градусы в системе WGS84, умноженные на 1e7 и
округленные к ближайшему 4-байтовому целому числу. Граница карты
может быть установлена пользователем и иметь любое значение (в частности,
быть пустой).')

</ul>

<h2>ENRU(`objects.db',`База данных objects.db')</h2>

<p>ENRU(`Object information. Key - object ID, value - a structure
with following fields:',
`Информация об объектах. Ключ - ID объекта, значение - структура со
следующими полями:')

<ul>

<li>type: ENRU(`A 32-bit integer, object type. Bits 0..13 are not used,
bits 14 and 15 are used for object classification (supported values:
0-point, 1-line, 2-polygon), bits 16..31 (last two bytes) - object type',
`32-битное челое число, тип объекта. Биты 0..13 не используются, биты
14 и 15 используются для классификации объекта
(поддерживаются значения: 0-точка, 1-линия, 2-многоугольник), биты 16..31
(два последних байта) - собственно, тип объекта.')
</ul>

<p> ENRU(`Other fields are optional, they are packed in the RIFF-like
form in arbitrary order',
`Дальнейшие поля необязаельны. Они запакованы в стиле RIFF в произвольном
порядке:')

<ul>
<li>ENRU(`Object angle (used for points) represented as 4-byte
integer value in 1/1000 degrees units. Packed with "angl" tag.',
`Наклон объекта (используется для точек) представленное в виде 4-байтового
целого числа, в единицах 1/1000 градуса. Запаковано с тэгом "angl".')

<li>ENRU(`Object direction (used for lines) represented as
4-byte integer: 1-forward, 2-backward, 0 or missing - unspecified. Packed
with "dir " tag.', `Ориентация объекта (используется для линий)
представленная в виде 4-байтового целого числа 1 - прямая, 2 - обратная,
0 или отсутствует - не определена. Запаковано с тэгом "dir ".')

<li>ENRU(`Object name (to be shown on the map),
string packed with "name" tag.',
`Название объекта (для показа на карте), строка, запакованая с тэгом "name".')

<li>ENRU(`Object comment, string packed with "comm" tag.',
`Комментарий к объекту, строка, запакованая с тэгом "comm".')

<li>ENRU(`Object tags, arbitrary number of strings packed with
"tags" tag.', `Тэги объекта, произвольное число строк, запакованных с
тэгами "tags".')

<li>ENRU(`Coordinates. A multisegment line encoded in the same way as the
border field in mapinfo.db with "crds" tags. When coordinates are changing
the geohash database should be also updated.',
`Координаты. Многосегментная линия, закодированная в строку также, как поле границы
карты в базе данных mapinfo.db с тэгами "crds". При обновлении координат объекта
также должна обновляться база геоиндексации.')

</ul>

<h2>ENRU(`geohash.db',`База данных geohash.db')</h2>

<p>ENRU(`Database with _GEOHASH_ keys and values containing object id and
type. Used for spatial indexing. Data should be syncronized with
coordinates in objects.db database.',

`База данных для геоиндексации: ключ - строка _GEOHASH_, значение - ID и тип
объекта. Данные должны быть синхронизованы с координатами в базе данных
objects.db.')


<h2>ENRU(`labels.db',`База данных labels.db')</h2>

<p>ENRU(`Database for object labels. Key - object ID (duplication is allowed),
value - a label structure with following fields:',
`База данных для подписей к объектам. Ключ - ID объекта (разрешено дублирование
ключа), значение - структура со следующими полями:')

<ul>
<li>...
</ul>



