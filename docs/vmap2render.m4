define(`LANG', RU)
HEADER(mapdb,`
  Рендер векторных карт Mapsoft2 (2020-02-15)
')

define(RENDER_EXAMPLE, `
<div style="border-style:solid; border-width:0.5;
            margin:3px; padding: 3px; color: black;
            font-family:monospace; white-space:pre">
<img src=$1/render$2.png align=right>
include($1/render$2.cfg)
</div><br>')


<p>В данный момент поддержка векторных карт в mapsoft находится
в состоянии разработки. Еще не все сделано, возможны любые изменения.

<!--#################################################################-->
<!--#################################################################-->

<p>Для получения изображение карты можно использовать команду командной строки:

<p><tt> $ ms2render &lt;vmap2_file&gt; -c &lt;render_config&gt; &lt;options&gt;</tt>

<p>Конфигурационный файл передается через параметр -c, --config. По умолчанию
используется файл render.cfg в той же директории, что и карта.

<p>Кроме того, изображение карты можно смотреть в программе ms2view, передав
директорию с картой через параметр командной строки --vmap и название
конфигурационного файла через параметр --vmap_config (это сейчас не работает!).

<h4>Формат конфигурационного файла и порядок рисования карты</h4>

<p>Рисование карты выполняется в виде последовательности "шагов" (drawing
steps). Каждый шаг описан в конфигурационном файле и содержит набор
свойств (feature). Также, конфигурационный файл может содержать команды,
не являющиеся шагами рисования. Пример конфигурационного файла: data/render.cfg

Формат описания шагов рисования:
<pre><tt>
    (point|line|area|text):&lt;tnum&gt; &lt;feature&gt; &lt;options&gt; ...
    + &lt;feature&gt; &lt;options&gt; ...
    + &lt;feature&gt; &lt;options&gt; ...
    ...
    (map|brd) &lt;feature&gt; &lt;options&gt; ...
    + &lt;feature&gt; &lt;options&gt; ...
    + &lt;feature&gt; &lt;options&gt; ...
    ...
</tt></pre>

<p>Шаги point, line, area, text описывают рисование линейного, точечного,
площадного или текстового объекта с номером типа &lt;tnum&gt;. Шаг map
описывает рисование на всей площади карты (можно нарисовать одноцветную
подложку, но можно делать и более хитрые вещи), Шаг brd описывает
рисование границы и закрашивание области вне ее.

<p>Свойства (features):
<ul>

<li><p><tt> stroke &lt;width&gt; &lt;color&gt; </tt> -- Нарисовать контур
объекта линией заданной толщины и цвета. Применимо к шагам point, line,
area, text, brd. Цвет всегда задается в виде 32-битного числа с прозрачностью
(0xFF000000 - черный, 0xFFFF0000 - красный, 0x80FFFFFF - белый
полупрозрачный и т.п.). Для текстового объекта линией обводятся контуры букв.

<li><p><tt> fill &lt;color&gt; </tt> -- Заливка заданным цветом. Применимо к
шагам point, line, area, map, text, brd.  Для текстового объекта заливка
применяется к контурам букв, результат немного отличается от
использования стандартной функции рендера текста (см. свойство write
ниже). Для точек - влияет только на свойства lines и circles.

<li><p><tt> pulk_grid &lt;step&gt; &lt;color&gt; &lt;line width&gt;</tt> --
Рисование сетки в системе координат Пулково-1942, как на советских картах.
Применимо только к шагу map. Параметр step - в километрах, если <=0, то
выставляется некоторое автоматическое значение.

<li><p><tt> font &lt;size&gt; &lt;font pattern&gt;</tt> -- Установить шрифт
для рисования текстовых объектов. "font pattern" задается в терминах библиотеки
fontconfig, какая-то информация есть тут:
<br>WWW(`https://www.freedesktop.org/software/fontconfig/fontconfig-devel/x19.html')
<br>WWW(`https://www.freedesktop.org/software/fontconfig/fontconfig-user.html')
<br>WWW(`https://wiki.archlinux.org/index.php/Font_configuration')

<li><p><tt> write &lt;color&gt; </tt> -- Нарисовать текстовый объект заданным цветом.

RENDER_EXAMPLE(render_examples, 01)
RENDER_EXAMPLE(render_examples, 14)
RENDER_EXAMPLE(render_examples, 07)

<li><p><tt> patt &lt;image file&gt; &lt;scale&gt; &lt;dx&gt; &lt;dy&gt;
</tt> --  Заливка площади заданной картинкой. Применимо к шагам line,
area, text, map, brd. Картинка - в любом растровом формате,
поддержтиваемом mapsoft2: png, gif, tiff, jpeg. Кроме того,
поддерживаются картинки svg. Путь к картинке должен быть указан
относительно места, где лежит конфигурационный файл. Параметр scale -
масштаб картинки, dx и dy - сдвиг картинки (в единицах размера картинки).
По умолчанию картинка выравнивается по центру. Если dx=dy=-0.5, то
выравнивание будет сделано по левому-нижнему углу. Для текстового объекта
заливка применяется к контурам букв.

<p>И растровые и SVG картинки всегда загружается в виде растра, в
собственном масштабе (то есть, если в SVG указан размер 100x200 точек, то
такой растр и будет создан). После этого картинки перемасштабируются к
нужному размеру. Старайтесь избегать больших картинок,
перемасштабированных к маленькому размеру и заполняющих большую область.
Во-первых, это очень медленно, это самая медленная операция при рисовании
карт. Во-вторых, начиная с некоторого масштаба и размера заливки libcairo
перестает рисовать что-либо. Как это устроено, я пока не понял, сделал
лишь очевидное ограничение: размер картинки ограничен величиной один
пиксел (после этого она перестает масштабироваться).
Но для больших исходных картинок это не помогает.

<p>При рисовании плиточных карт плитки должны укладываться в картинку
целое число раз. Это достигается использованием параметра fit_patt_size
(см.ниже).

<li><p><tt> img  &lt;image file&gt; &lt;scale&gt; &lt;dx&gt; &lt;dy&gt;</tt>
-- Рисование изображения. Применимо к шагам point, area (в этом случае
картинка рисуется в центре площади). Картинка задается так же, как и в
свойстве patt.

<li><p><tt> img_filter &lt;flt&gt;</tt> -- Установить фильтр растровых
изображений. Применимо к шагам point, area, text, map, brd, используется совместно
со свойствами img и patt. Возможные значение: fast, good, best, nearest,
bilinear
(см. WWW(`https://www.cairographics.org/manual/cairo-cairo-pattern-t.html`#'cairo-filter-t'))

RENDER_EXAMPLE(render_examples, 08)
RENDER_EXAMPLE(render_examples, 09)
RENDER_EXAMPLE(render_examples, 10)

<li><p><tt> smooth &lt;distance&gt; </tt> -- Использовать закругленные
линии с заданным размером закругления. Применимо к шагам line, area, brd,
используется совместно со свойствами stroke, fill, patt.

<li><p><tt> dash &lt;len1&gt; ... </tt> -- Использовать штриховые линии.
Параметры задают длины штрихов и промежутков между ними, так как это
принято в библиотеке Cairo. (Если параметр один - длины штрихов и
промежутков равны, если параметров более одного - они задают чередование
длин штрихов и промежутков между ними). Применимо к шагам line, area, text, brd
используется совместно со свойством stroke.

<li><p><tt> cap round|butt|square </tt> -- Описывает, как рисовать
окончание линии. Применимо к шагам line, area, text, brd, используется
совместно со свойством stroke. По умолчанию - round.

<li><p><tt> join round|miter </tt> -- Описывает, как рисовать
стыки сегментов линий. Применимо к шагам line, area, text, brd, используется
совместно со свойством stroke. По умолчанию - round.

RENDER_EXAMPLE(render_examples, 02)
RENDER_EXAMPLE(render_examples, 03)
RENDER_EXAMPLE(render_examples, 04)
RENDER_EXAMPLE(render_examples, 05)

<li><p><tt> operator &lt;op&gt; </tt> -- Установить оператор рисования.
Возможные значения: clear, source, over, in, out, atop, dest,
dest_over, dest_in, dest_out, dest_atop, xor, add, saturate
(см. WWW(https://www.cairographics.org/operators/)). Значение по умолчанию -
over. Применимо к шагам point, line, area, text, map, brd.

RENDER_EXAMPLE(render_examples, 06)

<li><p><tt> clip </tt> -- Установить обрезку изображения по объекту типа
text, area, map или brd. Обрезка будет действовать для всех дальнейших
шагов до нового clip. Чтобы вернуться к обрезке по границе или сбросить
обрезку, сделайте <tt> map clip </tt> или <tt> brd clip </tt>

RENDER_EXAMPLE(render_examples, 15)
RENDER_EXAMPLE(render_examples, 16)

<li><p><tt> outer </tt> -- Использовать внешнюю область для заливки и обрезки
(только для объекта типа brd).

<li><p><tt> lines &lt;lines&gt; ... </tt> -- Вместо самого объекта рисовать
дополнительные линии, привязанные к каким-то местам объекта (см. свойство
draw_pos). Аргументы - одно- или многосегментные линии в виде
json-массивов: [[x1,y1],[x2,y2]]... Для рисования линий используются те
же свойства, что и для рисования самого объекта (stroke, fill, cap,
smooth и т.д.). Для линейных и площадных объектов координты ориентированы
по направлению объекта: x вдоль линии, y - перпендикулярно, вправо от нее.
Для точечных объектов x вправо, y - вниз. На ориентацию также влияет
свойство rotate и параметр наклон объекта.

<li><p><tt> circles &lt;circle&gt; ... </tt> -- Вместо самого объекта
рисовать дополнительные окружности, привязанные к каким-то местам объекта
(см. свойство draw_pos). Аргументы - параметры окружностей в виде
трехэлементных json-массивов: [x,y,r]. Для рисования окружностей
используются те же свойства, что и для рисования самого объекта (stroke,
fill, cap, smooth и т.д.). Координты ориентированы по направлению
объекта: x вдоль линии, y - перпендикулярно, вправо от нее.  На
ориентацию также влияет свойство rotate и параметр наклон объекта.

<li><p><tt>draw_pos (point|begin|end) </tt>
<li><p><tt>draw_pos (dist|edist) &lt;dist&gt; [&lt;dist_b&gt;] [&lt;dist_e&gt;]</tt>
-- Место рисования элементов lines и circles: point -- в каждом узле
объекта (значение по умолчанию и единственное возможное значение для
точечных объектов); begin/end -- в начальной/конечной точке; dist, edist
-- периодически вдоль объекта, на заданном расстоянии друг от друга; При
этом параметры &lt;dist&gt; &lt;dist_b&gt; &lt;dist_e&gt; задают период,
начальное и конечное расстояние. Значения по умолчанию:
&lt;dist_b&gt;=&lt;dist&gt;/2,  &lt;dist_e&gt;= &lt;dist_b&gt;. Если
второй параметр имеет значение dist, то начальное расстояние и период
отсчитывюаются точно, а конечное расстояние получается не менее &lt;dist_e&gt;.
Если edist -- то период подстраивается так, чтобы конечное расстояние было
равно в точности &lt;dist_e&gt;.

RENDER_EXAMPLE(render_examples, 11)

<li><p><tt> move_to &lt;max_distance&gt; (area|line):&lt;tnum&gt;  </tt> --
Сдвинуть точечный объект к ближайшему линейному объекту или ближайшей границе
площадного объекта типа type, но не далее max_distance.
Применимо к шагам типа point.

<li><p><tt> rotate_to &lt;max_distance&gt; (area|line):&lt;tnum&gt; </tt> --
То же, что и move_to, но картинка объекта также поворачивается по направлению
линии.

<li><p><tt> rotate &lt;angle,deg&gt; </tt> --
Повернуть картинку объекта или текст на фиксированный угол (градусы, по часовой стрелке).
Добавляется к собственным поворотам объекта или повороту с помощью свойства rotate_to.
Свойство применимо к шагам point, line, area, text. См ниже раздел про повороты объектов.

RENDER_EXAMPLE(render_examples, 12)

<li><p><tt> short_expand &lt;length&gt; </tt> --
Удлиннить короткие линии до указанной длины, пропорционально удлиннив крайние сегменты.

<li><p><tt> short_skip &lt;length&gt; </tt> --
Удалить линии, имеющие длину меньше указанной.

RENDER_EXAMPLE(render_short, 1)

<li><p><tt> sel_range &lt;width&gt; &lt;color&gt; </tt> -- Нарисовать
предполагаемый диапазон объекта, по которому он выбирается из базы
данных. Для расчета диапазона используются остальные правила рисования
(например, stroke с ненулевой толщиной линии увеличивает диапазон на
толщину линии, картинка - на диагональный размер картинки и т.п.)
Для поиска текста используется параметр max_text_size (см.ниже).

RENDER_EXAMPLE(render_examples, 13)

<li><p><tt> pix_align &lt;(0|1)&gt; </tt> -- Округлять координаты текста
к целым пикселам. Похоже, что этот параметр не очень нужен, если правильно
настроен hinting шрифта. Применимо к шагу типа text.

<li><p><tt> name  &lt;name&gt; </tt> -- Объявить название шага (для показа в
интерфейсе). По умолчанию название составляется из типа объекта,
напрмер "line:0x25".

<li><p><tt> group &lt;name&gt; </tt> -- Название группы для данного шага.
Группа может включать несколько шагов.



</ul>

<p>В одном шаге рисования могут присутствовать несколько свойств. На
каждом шаге рисование происходит в следующем порядке:

<ul>

<li><p>Определяется диапазон, в котором надо искать объекты. На него влияют
свойства, которые приводят к сдвигу объектов или созданию картинки
конечного размера: stroke, img, patt, move_to, rotate_to.
Для текстовых объектов используется некий фиксированный размер
(по умолчанию 1024 точки, см. команду max_text_size ниже).

<li><p>Если шаг имеет тип point, line, area, text, то выбираются соответствующие
объекты в нужном диапазоне.

<li><p>Если присутствует свойство sel_range, то рисуются прямоугольники
вокруг объектов.

<li><p>Настраивается оператор рисования (свойство operator), настраивается
шрифт (свойство font).

<li><p>Если присутствуют свойства stroke, fill, patt, то строится "путь"
рисования (path).

<li><p>Выполняется заливка картинкой (свойство patt).

<li><p>Выполняется заливка цветом (свойство fill).

<li><p>Выполняется рисование контура (свойство stroke). При этом
настраиваются параметры рисования, соответствующие свойствам dash, cap,
join.

<li><p>Выполняется рисование картинок (свойство img).

<li><p>Выполняется рисование текста (свойство write).
</ul>

<p>Если хочется использовать другой порядок (например, сперва контур,
потом заливку, потом паттерн) - придется сделать несколько последовательных
шагов рисования.

<p>Дополнительные команды, которые могут встречаться в конфигурационном файле:

<ul>

<li><p><tt> set_ref file &lt;filename&gt; </tt> -- установить "естественную"
привязку карты из файла (сейчас поддерживаются только файлы OziExplorer).
Потом карта может быть нарисована в другой системе координат, но относительно
"естественной" привязки считаются толщины линий, размер шрифтов и т.п.
При установки привязки устанавливается также и граница карты.

<li><p><tt> set_ref nom &lt;name&gt; &lt;dpi&gt;</tt> -- установить "естественную"
привязку карты по советскому номенклатурному листу, с заданным разрешением
(в точках на дюйм, например 300). Использовуется "расширенный формат" номенклатурных
названий, допускающий одиночные листы (например r36-010) и "диапазоны" листов
(например j42-040.3x3 -- блок из девяти одиночных листов).
При установки привязки устанавливается также и граница карты.

<li><p><tt> set_ref none</tt> -- убрать "естественную" привязку и границу карты.

<li><p><tt> set_brd file &lt;filename&gt; </tt> -- установить отдельно границу карты,
используя трек, записанный в файле (возможно, с несколькими сегментами).

<li><p><tt> set_brd nom &lt;name&gt; </tt> -- установить отдельно границу карты,
используя название номенклатурного листа.

<li><p><tt> set_brd none</tt> -- очистить границу карты.


<li><p><tt> max_text_size  &lt;number&gt;</tt> -- изменить максимальный размер текста
(в точках). Этот параметр используется при поиске текстовых объектов на карте.
Значение по умолчанию - 1024 точки. Размер относится к "естественному" масштабу карты,
при изменении масштаба параметр перемасштабируется соответствующим образом.

<li><p><tt> minsc &lt;number&gt;</tt> -- изменить минимальный масштаб карты (меньше
которого она рисуется сплошной заливкой). Масштаб считается относительно
"собственной" привязки, которая устанавливается командой set_ref.
По умолчанию этот параметр равен 0.01.

<li><p><tt> minsc_color &lt;color&gt;</tt> -- изменить цвет заливки для карты с
масштабом меньше минимального. По умолчанию 0xFFDB5A00.

<li><p><tt> define &lt;name&gt; &lt;definition&gt;</tt> -- определить
переменную. В последующем файле все вхождения ${&lt;name&gt;} будут
заменены на &lt;definition&gt;.

<li><p><tt> define_if_undef &lt;name&gt; &lt;definition&gt;</tt> -- определить
переменную, если она еще не определена.

<li><p><tt> if &lt;name&gt; (==|!=) &lt;name&gt;</tt>
<li><p><tt> ifdef &lt;name&gt;</tt>
<li><p><tt> ifundef &lt;name&gt;</tt>
<li><p><tt> else
<li><p><tt> endif  -- команды if/else/endif: в зависимости
от истинности условия, обрабатывать или игнорировать текст, заключенный
между if и endif. Пары if/endif могут быть вложенными, команда else
инвертирует последнее условие, заданное командой if.

<li><p><tt> include &lt;name&gt;</tt> -- прочитать внешний файл. Путь
к файлу может быть абсолютный, или относительный, от директории текущего
конфигурационного файла. Директории относительных путей, используемых
во вложенном файле (картинки, другие include'ы) считаются от положения
этого вложенного файла.

<li><p><tt> obj_scale &lt;val&gt;</tt> -- увеличить все объекты (шрифт,
толщины линий, картинки и т.д.), то же, что параметр командной строки
--obj_scale.

<li><p><tt> fit_patt_size 0|1</tt> -- подстраивать размер паттернов
под размер картинки. Это полезно при генерации плиточных карт.
То же, что параметр командной строки --fit_patt_size.

</ul>




<h4>TODO - чего пока не хватает</h4>

<p>Возможность отключать в интерфейсы свойства move_to, rotate_to, а,
может, и любые свойства.

<p>Более разверyтый интерфейс команды ms2mapdr render: параметры для
задания привязки и масштаба линий, исключения заданных групп шагов
рисования и т.п.

<p>"Расталкивание" линейных объектов. Например, рисовать дорогу на неком
минимальном расстоянии от реки, чтобы линии не наползали.


<!--#################################################################-->
<hr><h3>Наклон объектов и подписей</h3>

<p>Объекты имеют параметр "наклон" (angle). Наклон применяется только к
тексту, растровым картинкам или картинкам, нарисованным с помощью свойств
lines и circles, но не к координатам объекта. Наклон может отсутствовать,
в этом случае картинка объекта ориентируется на верх карты. Если наклон
задан, то он отсчитывается от географического севера, по часовой
стрелке.

<p>Свойство рисования rotate_to ориентирует картинку объекта по указанной линии,
игнорируя его собственный наклон.

<p>Свойство рисования rotate добавляет к наклону (собственному или
возникшему из-за использования rotate_to) фиксированный угол (в градусах,
по часовой стрелке).

<p>В формате VMAP параметр наклон объектов и текста определялся
весьма странно: <tt>angle = atan2(dlat, dlon)</tt>, где отрезок с
координатами (dlat, dlon) определяет направление объекта в географических
координатах. Поскольку координаты имеют разных масштаб по lat и lon, эта
величина сильно отличается от настоящего угла поворота. При
экспорте/импорте объектов из vmap наклоны объектов пересчитываются. Надо
ли делать так при работе с MP (куда такие наклоны тоже попадали) -
непонятно. Можно, наверное, сделать специальный параметр в
конфигурационных файлах для импорта/экспорта...

RENDER_EXAMPLE(render_ex_ang, 1)
RENDER_EXAMPLE(render_ex_ang, 2)
RENDER_EXAMPLE(render_ex_ang, 3)
RENDER_EXAMPLE(render_ex_ang, 4)
RENDER_EXAMPLE(render_ex_ang, 5)

<!--#################################################################-->
<hr><h3>Привязка, граница, разрешение карты</h3>

<p>Привязка карты может быть установлена через конфигурационный файл
(команда <tt>set_ref</tt>), из файла или по номенклатурному листу. При
этом также устанавливается граница (которую потом можно нарисовать
командой <tt>brd</tt>). Также, границу можно изменить командой
<tt>set_brd</tt>, из файла с треком (трек может быть многосегментным).

<p>Такая, установленная через конфигурационный файл привязка не
обязательно будет использована для рисования карты: при отрисовке
используется свой механизм преобразований в разные системы координат и
ничто не мешает нарисовать карту в любой другой системе. Например, при
рисовании карты через <tt>ms2render</tt> используется связка
функций <tt>geo_mkref</tt> (задание привязки с помощью набора параметров)
и <tt>write_geoimg</tt> - рисование картинки в соответствии с привязкой.
По умолчанию используется собственная привязка карты, но ее можно
поменять с помощью параметров --mkref. Отдельно можно поменять добавить
"внешнюю" границу с помощью переметров --border_wgs или --border_file.
Можно использовать параметр --tmap, чтобы сгенерировать набор плиток
(они, разумеется, рисуются в своей собственной системе координат).

<p>При установке привязки через конфигурационный файл запоминается ее
"естественный" масштаб, размер точки растра в метрах. Он важен при выборе
толщины линий, размер шрифтов и т.п. Если карта рисуется в другой системе
координат, то все эти величины будут перемасштабироваться. Кроме того,
может быть удобно использовать параметр --obj_scale - дополнительный
множитель для всех таких размеров. Если карта не имеет собственной привязки
(установленной через коннфигурационный файл), то размеры объектов не
будут меняться при перемасштабировании карты.

<p>Один из возможных рецептов: инкрементальное создание набора плиток
из листов карты. Каждый лист имеет свою привязку и границу (например,
это номенклатурные листы). При рисовании должна быть включена обрезка
объектов по границе карты (см. команду clip_border). Программе map_render
даются параметры --tmap - делать набор плиток, --add - дорисовывать листы
на старые плитки, если они имеются, --border_file/--border_wgs - дополнительно
обрезать все по внешней границе.






