define(`LANG', RU)
HEADER(mapdb,`
  Система знаков для векторных карт Mapsoft2
')
<p>Идея в том, чтобы иметь по возможности универсальный набор знаков и
правил их рендера в растровую карту и в векторную карту img. Разумеется,
конкретные картографические проекты могут использовать разные варианты
знаков. Однако мне хочется иметь некоторую базовую систему, которую
можно настраивать под конкретные задачи.
<p>Обратите внимание, что существует два отдельных конфигурационных файла:
информация о знаках (types.cfg - на его основе сделана эта
страница) и правила рисования карты (render.cfg).
Есть еще исходник для typ-файла, но предполагается, что он будет
создаваться из types.cfg.
<h3>Слой растительности</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 width=12en>тип</th>
<th rowspan=2 width="20%">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 width=80px>рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th width=12en>уровни</th>
<th width=12en>порядок</th>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x16</span></td>
<td>forest<br>лес</td>
<td>0 - 3</td>
<td>1</td>
<td><img src="types_render/area:0x16.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x52</span></td>
<td>open area<br>открытое пространство, поле</td>
<td>0 - 1</td>
<td>2</td>
<td><img src="types_render/area:0x52.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x4f</span></td>
<td>deforestation area<br>свежая вырубка</td>
<td>0 - 1</td>
<td>3</td>
<td><img src="types_render/area:0x4f.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x50</span></td>
<td>old deforestation area<br>старая вырубка</td>
<td>0 - 1</td>
<td>3</td>
<td><img src="types_render/area:0x50.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x14</span></td>
<td>semi-open area<br>редколесье</td>
<td>0 - 1</td>
<td>3</td>
<td><img src="types_render/area:0x14.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x15</span></td>
<td>forest island<br>остров леса</td>
<td>0 - 1</td>
<td>7</td>
<td><img src="types_render/area:0x15.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x23</span></td>
<td>vegetation boundary<br>контур растительности</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x23.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6618</span></td>
<td>logjam<br>завал</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6618.png"></td>
<td>Знак на картах ММБ</td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x660a</span></td>
<td>tree, group of trees<br>дерево, группа деревьев</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x660a.png"></td>
<td></td>
</tr>

</table>
<p>TODO: добавить отдельный линейный знак "лесополоса".
<p>В картах Подмосковья/ММБ используется белая подложка. Поверх нее
рисуется зеленый лес: где-то он покрывает всю площадь листа, где-то
рисуются только лесные участки. Поверх леса рисуются открытые места.
Выше - свежие и старые вырубки, редколесье. Выше - острова
леса. Такая схема была сделана, чтобы избежать полигонов с дырками и
резки полигонов, рисовать все небольшими, удобными для редактирования
кусками. Лесополосы иногда рисовались знаком "контур растительности",
хотя я старался использовать узкую полосу знака "остров леса" - надо ввести отдельный линейный знак.
Отдельные деревья и завалы рисовались только
на картах ММБ.
<h3>Рельеф</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 width=12en>тип</th>
<th rowspan=2 width="20%">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 width=80px>рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th width=12en>уровни</th>
<th width=12en>порядок</th>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x19</span></td>
<td>steep slope<br>крутой склон</td>
<td>0 - 1</td>
<td>1</td>
<td><img src="types_render/area:0x19.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x20</span></td>
<td>dashed contour<br>пунктирная горизонталь</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x20.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x21</span></td>
<td>горизонтали<br></td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x21.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x22</span></td>
<td>жирная горизонталь<br></td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x22.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0xc</span></td>
<td>mountain ridge<br>хребет</td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/line:0xc.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0xf</span></td>
<td>малый хребет<br></td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0xf.png"></td>
<td>не используется</td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x3</span></td>
<td>верхний край обрыва<br></td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x3.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x1e</span></td>
<td>нижний край обрыва<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x1e.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0xf00</span></td>
<td>triangulation point<br>триангуляционный знак</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0xf00.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x1100</span></td>
<td>summit<br>вершина</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/point:0x1100.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0xd00</span></td>
<td>hight mark<br>отметка высоты</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0xd00.png"></td>
<td>взятая автоматически из srtm и т.п.</td>
</tr>

</table>
<p>TODO: добавить точечный знак "бергштрих", который при рисовании будет
ориентироваться по ближайшей горизонтали.
<h3>Перевалы</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 width=12en>тип</th>
<th rowspan=2 width="20%">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 width=80px>рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th width=12en>уровни</th>
<th width=12en>порядок</th>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6406</span></td>
<td>перевал неизвестной сложности<br></td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/point:0x6406.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6700</span></td>
<td>simple pass<br>перевал н/к</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/point:0x6700.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6701</span></td>
<td>1A pass<br>перевал 1А</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/point:0x6701.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6702</span></td>
<td>1B pass<br>перевал 1Б</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/point:0x6702.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6703</span></td>
<td>2A pass<br>перевал 2А</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/point:0x6703.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6704</span></td>
<td>2B pass<br>перевал 2Б</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/point:0x6704.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6705</span></td>
<td>3A pass<br>перевал 3А</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/point:0x6705.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6706</span></td>
<td>3B pass<br>перевал 3Б</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/point:0x6706.png"></td>
<td></td>
</tr>

</table>
<h3>Гидрография</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 width=12en>тип</th>
<th rowspan=2 width="20%">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 width=80px>рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th width=12en>уровни</th>
<th width=12en>порядок</th>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x4d</span></td>
<td>glacier, snow, river icing<br>ледник, снежник, наледь</td>
<td>0 - 2</td>
<td>3</td>
<td><img src="types_render/area:0x4d.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x26</span></td>
<td>пересыхающий ручей<br></td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x26.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x15</span></td>
<td>река-1<br></td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/line:0x15.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x18</span></td>
<td>река-2<br></td>
<td>0 - 3</td>
<td></td>
<td><img src="types_render/line:0x18.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x1f</span></td>
<td>река-3<br></td>
<td>0 - 4</td>
<td></td>
<td><img src="types_render/line:0x1f.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x40</span></td>
<td>irrigation channel<br>канава, арык</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x40.png"></td>
<td>знак введен 2024.11 для хребтовок</td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x41</span></td>
<td>canal<br>канал</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x41.png"></td>
<td>знак введен 2024.12 в связи с финской картой</td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x29</span></td>
<td>водоемы<br></td>
<td>0 - 2</td>
<td>6</td>
<td><img src="types_render/area:0x29.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x3b</span></td>
<td>крупные водоемы<br></td>
<td>0 - 3</td>
<td>6</td>
<td><img src="types_render/area:0x3b.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x53</span></td>
<td>остров<br></td>
<td>0 - 2</td>
<td>7</td>
<td><img src="types_render/area:0x53.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x46</span></td>
<td>water boundary<br>четкая береговая линия</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x46.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x47</span></td>
<td>uncertain water boundary<br>нечеткая береговая линия</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x47.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x24</span></td>
<td>glacier crevasse<br>ледниковая трещина</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x24.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x48</span></td>
<td>rapids<br>пороги</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x48.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6414</span></td>
<td>spring, water source<br>родник, источник воды</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6414.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6508</span></td>
<td>waterfall<br>водопад</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6508.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x650e</span></td>
<td>rapid<br>порог</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x650e.png"></td>
<td>постоянная путаница со знаком водопад 0x6508</td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x1000</span></td>
<td>water altitude mark<br>отметка уреза воды</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x1000.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x650c</span></td>
<td>hydrography reference point<br>точка для объекта гидрографии</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/point:0x650c.png"></td>
<td>невидимая точка, к которой можно привязать название ледника, бухты, части реки и т.п.</td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x650b</span></td>
<td>small hydrography reference point<br>мелкая точка объекта гидрографии</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x650b.png"></td>
<td>невидимая точка, к которой можно привязать номер ледника, невидимый урез воды и т.п.</td>
</tr>

</table>
<h3>Прочие элементы местности</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 width=12en>тип</th>
<th rowspan=2 width="20%">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 width=80px>рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th width=12en>уровни</th>
<th width=12en>порядок</th>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x51</span></td>
<td>marsh<br>заболоченность</td>
<td>0 - 0</td>
<td>5</td>
<td><img src="types_render/area:0x51.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x4c</span></td>
<td>swamp<br>болото</td>
<td>0 - 1</td>
<td>5</td>
<td><img src="types_render/area:0x4c.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x58</span></td>
<td>mining/deposition area<br>карьеры и отвалы грунта</td>
<td>0 - 1</td>
<td>5</td>
<td><img src="types_render/area:0x58.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x8</span></td>
<td>scree<br>осыпь</td>
<td>0 - 1</td>
<td>5</td>
<td><img src="types_render/area:0x8.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0xd</span></td>
<td>sand<br>песок</td>
<td>0 - 1</td>
<td>8</td>
<td><img src="types_render/area:0xd.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x25</span></td>
<td>овраг<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x25.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x2c</span></td>
<td>вал<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x2c.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x2b</span></td>
<td>сухая канава<br></td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x2b.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x7</span></td>
<td>дорожная насыпь<br></td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x7.png"></td>
<td>может совмещаться с разными тропами и дорогами</td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6603</span></td>
<td>pit, sinkhole<br>яма</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6603.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6613</span></td>
<td>knoll, mound<br>холм, курган</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6613.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6601</span></td>
<td>cave<br>пещера</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6601.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x650a</span></td>
<td>icefall<br>ледопад</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x650a.png"></td>
<td>стараюсь не использовать</td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x660b</span></td>
<td>canyon<br>каньон</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x660b.png"></td>
<td>стараюсь не использовать</td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6616</span></td>
<td>rock pillar, big boulder<br>скала-останец, камень</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/point:0x6616.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x2800</span></td>
<td>reference point for land objects<br>точка привязки текста</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/point:0x2800.png"></td>
<td>невидимая точка, к которой можно привязать название урочища, мыса и т.п.</td>
</tr>

</table>
<p>TODO: добавить знак "сельхозугодья (поля, сады)", который может накладываться на лес/поле
<p>TODO: добавить знак "скалы" (сейчас вместо него используется знак "крутой склон")
<p>TODO: добавить знак "граница морен" (сейчас на хребтовках используется знак "канава")
<p>TODO: добавить знак "маленькая точка привязки" (для лесных кварталов)?
<h3>Транспортная сеть</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 width=12en>тип</th>
<th rowspan=2 width="20%">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 width=80px>рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th width=12en>уровни</th>
<th width=12en>порядок</th>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x27</span></td>
<td>железная дорога<br></td>
<td>0 - 4</td>
<td></td>
<td><img src="types_render/line:0x27.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0xd</span></td>
<td>УЖД и подъездные пути<br></td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/line:0xd.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x1</span></td>
<td>автомагистраль<br></td>
<td>0 - 4</td>
<td></td>
<td><img src="types_render/line:0x1.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0xb</span></td>
<td>большое шоссе<br></td>
<td>0 - 3</td>
<td></td>
<td><img src="types_render/line:0xb.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x2</span></td>
<td>шоссе<br></td>
<td>0 - 3</td>
<td></td>
<td><img src="types_render/line:0x2.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x4</span></td>
<td>проезжий грейдер<br></td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/line:0x4.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x6</span></td>
<td>проезжая грунтовка<br></td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/line:0x6.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0xa</span></td>
<td>непроезжая грунтовка<br></td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0xa.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x2a</span></td>
<td>тропа<br></td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x2a.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x16</span></td>
<td>просека<br></td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x16.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x1c</span></td>
<td>просека широкая<br></td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x1c.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x2e</span></td>
<td>заросшая заметная дорога (знак ММБ)<br></td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x2e.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x2d</span></td>
<td>заросшая дорога<br></td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x2d.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x8</span></td>
<td>мост-1 (пешеходный)<br></td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x8.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x9</span></td>
<td>мост-2 (автомобильный)<br></td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/line:0x9.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0xe</span></td>
<td>мост-5 (на автомагистралях)<br></td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/line:0xe.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x1b</span></td>
<td>пешеходный тоннель<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x1b.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x42</span></td>
<td>ferry<br>паром</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x42.png"></td>
<td>знак введен 2025.01 в связи с финской картой</td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x43</span></td>
<td>cable way, ski lift<br>канатная дорога, лыжный подъемник</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x43.png"></td>
<td>знак введен 2025.01 в связи с финской картой</td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x44</span></td>
<td>runway<br>асфальтовая ВВП</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x44.png"></td>
<td>знак введен 2025.01 в связи с финской картой</td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x45</span></td>
<td>sod runway<br>грунтовая ВВП</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x45.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x28</span></td>
<td>pipeline<br>трубопровод</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x28.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x29</span></td>
<td>power line<br>ЛЭП</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x29.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x1a</span></td>
<td>small power line<br>маленькая ЛЭП</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x1a.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x5905</span></td>
<td>railway station<br>ж/д станция</td>
<td>0 - 3</td>
<td></td>
<td><img src="types_render/point:0x5905.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x2f08</span></td>
<td>bus stop<br>остановка автобуса</td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/point:0x2f08.png"></td>
<td></td>
</tr>

</table>
<h3>Границы и территории</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 width=12en>тип</th>
<th rowspan=2 width="20%">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 width=80px>рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th width=12en>уровни</th>
<th width=12en>порядок</th>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x1d</span></td>
<td>граница стран, областей<br></td>
<td>1 - 4</td>
<td></td>
<td><img src="types_render/line:0x1d.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x36</span></td>
<td>граница областей, районов<br></td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/line:0x36.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x37</span></td>
<td>граница заповедников, парков<br></td>
<td>0 - 3</td>
<td></td>
<td><img src="types_render/line:0x37.png"></td>
<td></td>
</tr>

</table>
TODO: попробовать добавить площадные красные/зеленые территории
<h3>Населенные пункты и строения</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 width=12en>тип</th>
<th rowspan=2 width="20%">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 width=80px>рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th width=12en>уровни</th>
<th width=12en>порядок</th>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x4</span></td>
<td>закрытые территории<br></td>
<td>0 - 1</td>
<td>4</td>
<td><img src="types_render/area:0x4.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0xe</span></td>
<td>деревни<br></td>
<td>0 - 2</td>
<td>4</td>
<td><img src="types_render/area:0xe.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x1</span></td>
<td>города<br></td>
<td>0 - 4</td>
<td>4</td>
<td><img src="types_render/area:0x1.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x4e</span></td>
<td>дачи, сад.уч., д/о, п/л<br></td>
<td>0 - 2</td>
<td>5</td>
<td><img src="types_render/area:0x4e.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x1a</span></td>
<td>кладбище<br></td>
<td>0 - 1</td>
<td>5</td>
<td><img src="types_render/area:0x1a.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x5</span></td>
<td>building<br>строение</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x5.png"></td>
<td>линейный знак для строений, почти не используется</td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x19</span></td>
<td>fence<br>забор</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x19.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x700</span></td>
<td>big town<br>большой населенный пункт, город</td>
<td>0 - 4</td>
<td></td>
<td><img src="types_render/point:0x700.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x800</span></td>
<td>small town<br>средний населенный пункт</td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/point:0x800.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x900</span></td>
<td>village<br>маленький населенный пункт, деревня</td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/point:0x900.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x2a04</span></td>
<td>monument<br>памятник</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x2a04.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x2b03</span></td>
<td>camping place, fireplace<br>место стоянки, место для костра</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x2b03.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x2b04</span></td>
<td>open hut, shelter for tourists<br>открытая изба, навес для ночевки</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x2b04.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x2c0b</span></td>
<td>church<br>церковь</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/point:0x2c0b.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x2e00</span></td>
<td>shop<br>магазин</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x2e00.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6401</span></td>
<td>gate<br>ворота, проход в заборе</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6401.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6402</span></td>
<td>building<br>дом, строение</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6402.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6403</span></td>
<td>cemetery<br>кладбище</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6403.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6411</span></td>
<td>башня, труба<br>tower, chimney, mast</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/point:0x6411.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6415</span></td>
<td>ruins, archeological place<br>развалины, археологический объект</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6415.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x640c</span></td>
<td>mine<br>шахта</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x640c.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6606</span></td>
<td>watch tower, hunting structure, shepperds camp<br>смотровая башня, охотничья вышка, кормушка, стоянка пастухов</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6606.png"></td>
<td></td>
</tr>

</table>
<h3>Пути</h3>
<p>Совмещаются с дорогами/тропами. Так можно маркировать
туристские маршруты, английские footpaths и т.п.

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 width=12en>тип</th>
<th rowspan=2 width="20%">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 width=80px>рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th width=12en>уровни</th>
<th width=12en>порядок</th>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x32</span></td>
<td>плохой путь<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x32.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x33</span></td>
<td>удовлетворительный путь<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x33.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x34</span></td>
<td>хороший путь<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x34.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x35</span></td>
<td>отличный путь<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x35.png"></td>
<td></td>
</tr>

</table>
<h3>Прочие знаки (не используются)</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 width=12en>тип</th>
<th rowspan=2 width="20%">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 width=80px>рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th width=12en>уровни</th>
<th width=12en>порядок</th>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0xa</span></td>
<td>дырка в DEM-данных<br></td>
<td>0 - 1</td>
<td>1</td>
<td><img src="types_render/area:0xa.png"></td>
<td>не нужен?</td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x0</span></td>
<td>кривая надпись<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x0.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x50</span></td>
<td>bad route /не использовать!/<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x50.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x7f</span></td>
<td>text object<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x7f.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x7e</span></td>
<td>border object<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x7e.png"></td>
<td></td>
</tr>

</table>
<h3>Текстовые типы</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 width=12en>тип</th>
<th rowspan=2 width="20%">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 width=80px>рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th width=12en>уровни</th>
<th width=12en>порядок</th>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x1</span></td>
<td>значение по умолчанию<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x1.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x2</span></td>
<td>черный жирный прямой шрифт: города, деревни<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x2.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x3</span></td>
<td>черный курсив: урочища, дачи и т.п.<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x3.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x4</span></td>
<td>синий курсив: реки, озера, ледники<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x4.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x5</span></td>
<td>вершины<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x5.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x6</span></td>
<td>мелкие вершины<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x6.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x7</span></td>
<td>перевалы<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x7.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x8</span></td>
<td>отметки уреза воды<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x8.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x9</span></td>
<td>номер ледника<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x9.png"></td>
<td></td>
</tr>

</table>
TODO: полностью переделать
