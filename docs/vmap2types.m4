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
создаваться из types.cfg. Знаки перечислены примерно в порядке рисования,
хотя правила рисовки могут быть не такими простыми и линейными.
<h3>Растительность</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 style="width:7em;">тип</th>
<th rowspan=2 style="width:20%;">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 style="width: 80px;">рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th style="width:5em;">уровни</th>
<th style="width:5em;">порядок</th>
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
<td>open area<br>открытое пространство</td>
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
<td style="background-color: #b0ffb0;">line:0x49</span></td>
<td>forest strip<br>лесополоса</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x49.png"></td>
<td>знак введен 24.01.2025</td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x23</span></td>
<td>vegetation boundary<br>контур растительности</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x23.png"></td>
<td></td>
</tr>

</table>
<p>В картах Подмосковья/ММБ используется белая подложка. Поверх нее
рисуется зеленый лес: где-то он покрывает всю площадь листа, где-то
рисуются только лесные участки. Поверх леса рисуются открытые места.
Выше - свежие и старые вырубки, редколесье. Выше - острова
леса. Такая схема была сделана, чтобы избежать полигонов с дырками и
резки полигонов, рисовать все небольшими, удобными для редактирования
кусками. Лесополосы иногда рисовались знаком "контур растительности",
хотя я старался использовать узкую полосу знака "остров леса" - теперь
введен отдельный линейный знак.
<p>TODO: отталкивание лесополос от дорог.
<h3>Другие типы местности</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 style="width:7em;">тип</th>
<th rowspan=2 style="width:20%;">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 style="width: 80px;">рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th style="width:5em;">уровни</th>
<th style="width:5em;">порядок</th>
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
<td style="background-color: #b0ffff;">area:0x19</span></td>
<td>steep slope<br>крутой склон</td>
<td>0 - 1</td>
<td>1</td>
<td><img src="types_render/area:0x19.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x54</span></td>
<td>open rock<br>открытые скалы</td>
<td>0 - 1</td>
<td>5</td>
<td><img src="types_render/area:0x54.png"></td>
<td>Введен 2025.07 для финской карты. Пока не используется</td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x08</span></td>
<td>scree<br>осыпь</td>
<td>0 - 1</td>
<td>5</td>
<td><img src="types_render/area:0x8.png"></td>
<td>не используется</td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x0d</span></td>
<td>sand<br>песок</td>
<td>0 - 1</td>
<td>8</td>
<td><img src="types_render/area:0xd.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x13</span></td>
<td>field, orchard<br>сельхозугодья (поля, сады)</td>
<td>0 - 1</td>
<td>8</td>
<td><img src="types_render/area:0x13.png"></td>
<td>Добавлен 2025.07. Попробовать использовать и для лесопосадок?</td>
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

</table>
<p>Рисуются поверх леса, под рельефом. Через некоторые знаки может просвечивать лес
(например, болото может быть лесное или открытое). На фоне "крутого склона" знак "ледник"
становится темнее (надо бы сделать то же с лесом).
<h3>Искуственные площадные объекты</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 style="width:7em;">тип</th>
<th rowspan=2 style="width:20%;">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 style="width: 80px;">рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th style="width:5em;">уровни</th>
<th style="width:5em;">порядок</th>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x04</span></td>
<td>industrial or military area<br>закрытые территории</td>
<td>0 - 1</td>
<td>4</td>
<td><img src="types_render/area:0x4.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x0e</span></td>
<td>village area<br>деревни</td>
<td>0 - 2</td>
<td>4</td>
<td><img src="types_render/area:0xe.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x01</span></td>
<td>urban area<br>города</td>
<td>0 - 4</td>
<td>4</td>
<td><img src="types_render/area:0x1.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x4e</span></td>
<td>summer houses, etc.<br>дачи, сад.уч., д/о, п/л</td>
<td>0 - 2</td>
<td>5</td>
<td><img src="types_render/area:0x4e.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x1a</span></td>
<td>cemetery<br>кладбище</td>
<td>0 - 1</td>
<td>5</td>
<td><img src="types_render/area:0x1a.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x58</span></td>
<td>mining/deposition area<br>карьеры и отвалы грунта</td>
<td>0 - 1</td>
<td>5</td>
<td><img src="types_render/area:0x58.png"></td>
<td>введен 2025.01</td>
</tr>

</table>
<p> Сейчас рисуются под рельефом, хотя, возможно, стоит рисовать поверх.
<h3>Рельеф</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 style="width:7em;">тип</th>
<th rowspan=2 style="width:20%;">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 style="width: 80px;">рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th style="width:5em;">уровни</th>
<th style="width:5em;">порядок</th>
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
<td>contour<br>горизонтали</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x21.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x22</span></td>
<td>thick contour<br>жирная горизонталь</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x22.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6600</span></td>
<td>hachure<br>бергштрих</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6600.png"></td>
<td>Добавлен 2025.07. Ориентируется по горизонтали (сейчас это не очень красиво выглядит на углах, надо исправить)</td>
</tr>

</table>
<h3>Гидрография</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 style="width:7em;">тип</th>
<th rowspan=2 style="width:20%;">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 style="width: 80px;">рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th style="width:5em;">уровни</th>
<th style="width:5em;">порядок</th>
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
<td>river-1<br>река-1</td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/line:0x15.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x18</span></td>
<td>river-2<br>река-2</td>
<td>0 - 3</td>
<td></td>
<td><img src="types_render/line:0x18.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x1f</span></td>
<td>river-3<br>река-3</td>
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
<td>water body<br>водоем</td>
<td>0 - 2</td>
<td>6</td>
<td><img src="types_render/area:0x29.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x3b</span></td>
<td>big water body<br>крупный водоем</td>
<td>0 - 3</td>
<td>6</td>
<td><img src="types_render/area:0x3b.png"></td>
<td>Предполагается, что в img крупные водоемы показываются на более мелких масштабах</td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x53</span></td>
<td>island<br>остров</td>
<td>0 - 2</td>
<td>7</td>
<td><img src="types_render/area:0x53.png"></td>
<td>не использовать, убрать</td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x46</span></td>
<td>coastline<br>четкая береговая линия</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x46.png"></td>
<td>Если в конфигурации рендера use_coastline=1, то этот знак рисуется вместо контуров area:0x29 и area:0x3B</td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x47</span></td>
<td>uncertain coastline<br>нечеткая береговая линия</td>
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
<td>введен 2024.10, ранее - линейное болото</td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x48</span></td>
<td>rapids<br>пороги</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x48.png"></td>
<td>знак введен 2025.01</td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x650e</span></td>
<td>rapid<br>порог</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x650e.png"></td>
<td>Постоянная путаница со знаком водопад 0x6508. При рисовании притягивается к ближайшей реке/озеру/острову (line:0x26 line:0x15 line:0x18 line:0x1F area:0x29 area:0x3B area:0x53)</td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6508</span></td>
<td>waterfall<br>водопад</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6508.png"></td>
<td>при рисовании притягивается и поворачивается к ближайшей реке/озеру/острову (line:0x26 line:0x15 line:0x18 line:0x1F area:0x29 area:0x3B area:0x53)</td>
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
<td style="background-color: #ffb0ff;">point:0x1000</span></td>
<td>water altitude mark<br>отметка уреза воды</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x1000.png"></td>
<td>при рисовании притягивается к ближайшей реке/озеру/острову (line:0x26 line:0x15 line:0x18 line:0x1F area:0x29 area:0x3B area:0x53)</td>
</tr>

</table>
<h3>Пути</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 style="width:7em;">тип</th>
<th rowspan=2 style="width:20%;">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 style="width: 80px;">рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th style="width:5em;">уровни</th>
<th style="width:5em;">порядок</th>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x32</span></td>
<td>route quality 1<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x32.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x33</span></td>
<td>route quality 2<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x33.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x34</span></td>
<td>route quality 3<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x34.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x35</span></td>
<td>route quality 4<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x35.png"></td>
<td></td>
</tr>

</table>
<p>Совмещаются с дорогами/тропами. Так можно маркировать
туристские маршруты, английские footpaths и т.п.
<h3>Прочие линейные объекты</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 style="width:7em;">тип</th>
<th rowspan=2 style="width:20%;">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 style="width: 80px;">рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th style="width:5em;">уровни</th>
<th style="width:5em;">порядок</th>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x0c</span></td>
<td>mountain ridge<br>хребет</td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/line:0xc.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x0f</span></td>
<td>small mountain ridge<br>малый хребет</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0xf.png"></td>
<td>не используется</td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x25</span></td>
<td>gulley<br>овраг</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x25.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x2c</span></td>
<td>levee<br>вал</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x2c.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x2b</span></td>
<td>dry ditch<br>сухая канава</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x2b.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x51</span></td>
<td>moraine boundary<br>граница морен</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x51.png"></td>
<td>добавлен 2025.07, раньше использовался знак line:0x2B</td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x07</span></td>
<td>road embankment<br>дорожная насыпь</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x7.png"></td>
<td>может совмещаться с разными тропами и дорогами</td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x03</span></td>
<td>top edge of steep slope<br>верхний край обрыва</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x3.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x1e</span></td>
<td>bottom edge of steep slope<br>нижний край обрыва</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0x1e.png"></td>
<td>не используется</td>
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
<td style="background-color: #ffb0ff;">point:0x6401</span></td>
<td>gate<br>ворота, проход в заборе</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6401.png"></td>
<td>при рисовании притягивается к знаку забора</td>
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
<td style="background-color: #ffb0ff;">point:0x6608</span></td>
<td>crater<br>кратер</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6608.png"></td>
<td>добавлен 2025.07 для синхронизации с системой знаков О.Власенко</td>
</tr>

</table>
<h3>Границы и территории</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 style="width:7em;">тип</th>
<th rowspan=2 style="width:20%;">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 style="width: 80px;">рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th style="width:5em;">уровни</th>
<th style="width:5em;">порядок</th>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x1d</span></td>
<td>national boundaries<br>граница стран, областей</td>
<td>1 - 4</td>
<td></td>
<td><img src="types_render/line:0x1d.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x36</span></td>
<td>district boundaries<br>граница областей, районов</td>
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
TODO: попробовать добавить площадные полупрозрачные красные/зеленые территории
<h3>Транспортная сеть</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 style="width:7em;">тип</th>
<th rowspan=2 style="width:20%;">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 style="width: 80px;">рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th style="width:5em;">уровни</th>
<th style="width:5em;">порядок</th>
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
<td style="background-color: #b0ffb0;">line:0x01</span></td>
<td>автомагистраль<br></td>
<td>0 - 4</td>
<td></td>
<td><img src="types_render/line:0x1.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x0b</span></td>
<td>большое шоссе<br></td>
<td>0 - 3</td>
<td></td>
<td><img src="types_render/line:0xb.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x02</span></td>
<td>шоссе<br></td>
<td>0 - 3</td>
<td></td>
<td><img src="types_render/line:0x2.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x04</span></td>
<td>проезжий грейдер<br></td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/line:0x4.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x06</span></td>
<td>проезжая грунтовка<br></td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/line:0x6.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x0a</span></td>
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
<td style="background-color: #b0ffb0;">line:0x44</span></td>
<td>runway<br>асфальтовая ВПП</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x44.png"></td>
<td>знак введен 2025.01 в связи с финской картой</td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x45</span></td>
<td>sod runway<br>грунтовая ВПП</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x45.png"></td>
<td>знак введен 2025.01 в связи с финской картой</td>
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
<td style="background-color: #b0ffb0;">line:0x0d</span></td>
<td>УЖД и подъездные пути<br></td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/line:0xd.png"></td>
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
<td style="background-color: #b0ffb0;">line:0x0e</span></td>
<td>мост-5 (на автомагистралях)<br></td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/line:0xe.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x09</span></td>
<td>мост-2 (автомобильный)<br></td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/line:0x9.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x08</span></td>
<td>мост-1 (пешеходный)<br></td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x8.png"></td>
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
<td style="background-color: #ffb0ff;">point:0x5905</span></td>
<td>railway station<br>ж/д станция</td>
<td>0 - 3</td>
<td></td>
<td><img src="types_render/point:0x5905.png"></td>
<td>при рисовании притягивается к ближайшей ж/д (line:0x27 или line:0xD) и поворачивается</td>
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
<p>TODO: добавить знак "шлагбаум" (черный кружок)
<p>TODO: добавить знак "асфальтированная площадь"
<p>TODO: добавить знак "круговая развязка"
<h3>Разные точечные объекты</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 style="width:7em;">тип</th>
<th rowspan=2 style="width:20%;">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 style="width: 80px;">рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th style="width:5em;">уровни</th>
<th style="width:5em;">порядок</th>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6618</span></td>
<td>logjam<br>завал</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6618.png"></td>
<td>Введен 2025.01, до этого использовался на картах ММБ</td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x660a</span></td>
<td>tree, group of trees<br>дерево, группа деревьев</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x660a.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x0d00</span></td>
<td>hight mark<br>отметка высоты</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0xd00.png"></td>
<td>взятая автоматически из srtm и т.п.</td>
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
<td style="background-color: #ffb0ff;">point:0x0f00</span></td>
<td>triangulation point<br>триангуляционный знак</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0xf00.png"></td>
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
<td style="background-color: #ffb0ff;">point:0x6601</span></td>
<td>cave<br>пещера</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6601.png"></td>
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
<td style="background-color: #ffb0ff;">point:0x6403</span></td>
<td>cemetery<br>кладбище</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6403.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x05</span></td>
<td>building<br>строение</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/line:0x5.png"></td>
<td>линейный знак для строений, почти не используется</td>
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
<td style="background-color: #ffb0ff;">point:0x6411</span></td>
<td>башня, труба<br>tower, chimney, mast</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/point:0x6411.png"></td>
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
<td style="background-color: #ffb0ff;">point:0x2e00</span></td>
<td>shop<br>магазин</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x2e00.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x2c04</span></td>
<td>monument<br>памятник</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x2c04.png"></td>
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
<td style="background-color: #ffb0ff;">point:0x6606</span></td>
<td>watch tower, hunting structure, shepherds camp<br>смотровая башня, охотничья вышка, кормушка</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6606.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x6607</span></td>
<td>shepherds camp<br>стоянка пастухов</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0x6607.png"></td>
<td>'введен 2024.07. Отталкивается от знаков "дом", "развалины"'</td>
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
<td>введен 2024.12</td>
</tr>

</table>
<p>Надо либо вообще убрать знаки "каньон" и "ледопад", либо сделать
их коричневыми/синими кружками с категорями, как перевалы.
<h3>Перевалы</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 style="width:7em;">тип</th>
<th rowspan=2 style="width:20%;">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 style="width: 80px;">рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th style="width:5em;">уровни</th>
<th style="width:5em;">порядок</th>
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
<h3>Невидимые точки для привязки текста</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 style="width:7em;">тип</th>
<th rowspan=2 style="width:20%;">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 style="width: 80px;">рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th style="width:5em;">уровни</th>
<th style="width:5em;">порядок</th>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x0700</span></td>
<td>big town<br>большой населенный пункт</td>
<td>0 - 4</td>
<td></td>
<td><img src="types_render/point:0x700.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x0800</span></td>
<td>small town<br>средний населенный пункт</td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/point:0x800.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0x0900</span></td>
<td>village<br>маленький населенный пункт</td>
<td>0 - 2</td>
<td></td>
<td><img src="types_render/point:0x900.png"></td>
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
<tr>
<td style="background-color: #ffb0ff;">point:0x2800</span></td>
<td>land reference point<br>точка привязки текста</td>
<td>0 - 1</td>
<td></td>
<td><img src="types_render/point:0x2800.png"></td>
<td>невидимая точка, к которой можно привязать название урочища, мыса и т.п.</td>
</tr>

</table>
<p>TODO: добавить знак "маленькая точка привязки" (для лесных кварталов)?
<h3>Тестовые знаки</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 style="width:7em;">тип</th>
<th rowspan=2 style="width:20%;">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 style="width: 80px;">рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th style="width:5em;">уровни</th>
<th style="width:5em;">порядок</th>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0xff01</span></td>
<td>test area R<br>тестовая площадь R</td>
<td>0 - 0</td>
<td>1</td>
<td><img src="types_render/area:0xff01.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0xff02</span></td>
<td>test area G<br>тестовая площадь G</td>
<td>0 - 0</td>
<td>1</td>
<td><img src="types_render/area:0xff02.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0xff03</span></td>
<td>test area B<br>тестовая площадь B</td>
<td>0 - 0</td>
<td>1</td>
<td><img src="types_render/area:0xff03.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0xff04</span></td>
<td>test area C<br>тестовая площадь C</td>
<td>0 - 0</td>
<td>1</td>
<td><img src="types_render/area:0xff04.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0xff05</span></td>
<td>test area M<br>тестовая площадь M</td>
<td>0 - 0</td>
<td>1</td>
<td><img src="types_render/area:0xff05.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0xff06</span></td>
<td>test area Y<br>тестовая площадь Y</td>
<td>0 - 0</td>
<td>1</td>
<td><img src="types_render/area:0xff06.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0xff01</span></td>
<td>test line R<br>тестовая линия R</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0xff01.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0xff02</span></td>
<td>test line G<br>тестовая линия G</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0xff02.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0xff03</span></td>
<td>test line B<br>тестовая линия B</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0xff03.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0xff04</span></td>
<td>test line C<br>тестовая линия C</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0xff04.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0xff05</span></td>
<td>test line M<br>тестовая линия M</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0xff05.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0xff06</span></td>
<td>test line Y<br>тестовая линия Y</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/line:0xff06.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0xff01</span></td>
<td>test point R<br>тестовая точка R</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0xff01.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0xff02</span></td>
<td>test point G<br>тестовая точка G</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0xff02.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0xff03</span></td>
<td>test point B<br>тестовая точка B</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0xff03.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0xff04</span></td>
<td>test point C<br>тестовая точка C</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0xff04.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0xff05</span></td>
<td>test point M<br>тестовая точка M</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0xff05.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffb0ff;">point:0xff06</span></td>
<td>test point Y<br>тестовая точка Y</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/point:0xff06.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0xff01</span></td>
<td>test text R<br>тестовый текст R</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0xff01.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0xff02</span></td>
<td>test text G<br>тестовый текст G</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0xff02.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0xff03</span></td>
<td>test text B<br>тестовый текст B</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0xff03.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0xff04</span></td>
<td>test text C<br>тестовый текст C</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0xff04.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0xff05</span></td>
<td>test text M<br>тестовый текст M</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0xff05.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0xff06</span></td>
<td>test text Y<br>тестовый текст Y</td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0xff06.png"></td>
<td></td>
</tr>

</table>
<p>Рисуются яркими цветами поверх карты. Удобны для
разглядывания непонятных картографических данных
<h3>Устаревшие знаки (не используются)</h3>

<table cellspacing=0 cellpadding=3 border=1 width="100%">
<tr>
<th rowspan=2 style="width:7em;">тип</th>
<th rowspan=2 style="width:20%;">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 style="width: 80px;">рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th style="width:5em;">уровни</th>
<th style="width:5em;">порядок</th>
</tr>
<tr>
<td style="background-color: #b0ffff;">area:0x0a</span></td>
<td>дырка в DEM-данных<br></td>
<td>0 - 1</td>
<td>1</td>
<td><img src="types_render/area:0xa.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #b0ffb0;">line:0x00</span></td>
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
<th rowspan=2 style="width:7em;">тип</th>
<th rowspan=2 style="width:20%;">название</th>
<th colspan=2>MP/TYP/IMG</th>
<th rowspan=2 style="width: 80px;">рендер</th>
<th rowspan=2>комментарий</td>
</tr>

<tr>
<th style="width:5em;">уровни</th>
<th style="width:5em;">порядок</th>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x01</span></td>
<td>значение по умолчанию<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x1.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x02</span></td>
<td>черный жирный прямой шрифт: города, деревни<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x2.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x03</span></td>
<td>черный курсив: урочища, дачи и т.п.<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x3.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x04</span></td>
<td>синий курсив: реки, озера, ледники<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x4.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x05</span></td>
<td>вершины<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x5.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x06</span></td>
<td>мелкие вершины<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x6.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x07</span></td>
<td>перевалы<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x7.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x08</span></td>
<td>отметки уреза воды<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x8.png"></td>
<td></td>
</tr>
<tr>
<td style="background-color: #ffffff;">text:0x09</span></td>
<td>номер ледника<br></td>
<td>0 - 0</td>
<td></td>
<td><img src="types_render/text:0x9.png"></td>
<td></td>
</tr>

</table>
TODO: полностью переделать

<hr><h3>Список знаков по порядку номеров</h3>
<table cellspacing=0 cellpadding=2 border=0>
<tr><td><b>area:0x01</b></td><td>urban area / города</td></tr>
<tr><td><b>area:0x04</b></td><td>industrial or military area / закрытые территории</td></tr>
<tr><td><b>area:0x08</b></td><td>scree / осыпь</td></tr>
<tr><td><b>area:0x0a</b></td><td>дырка в DEM-данных / </td></tr>
<tr><td><b>area:0x0d</b></td><td>sand / песок</td></tr>
<tr><td><b>area:0x0e</b></td><td>village area / деревни</td></tr>
<tr><td><b>area:0x13</b></td><td>field, orchard / сельхозугодья (поля, сады)</td></tr>
<tr><td><b>area:0x14</b></td><td>semi-open area / редколесье</td></tr>
<tr><td><b>area:0x15</b></td><td>forest island / остров леса</td></tr>
<tr><td><b>area:0x16</b></td><td>forest / лес</td></tr>
<tr><td><b>area:0x19</b></td><td>steep slope / крутой склон</td></tr>
<tr><td><b>area:0x1a</b></td><td>cemetery / кладбище</td></tr>
<tr><td><b>area:0x29</b></td><td>water body / водоем</td></tr>
<tr><td><b>area:0x3b</b></td><td>big water body / крупный водоем</td></tr>
<tr><td><b>area:0x4c</b></td><td>swamp / болото</td></tr>
<tr><td><b>area:0x4d</b></td><td>glacier, snow, river icing / ледник, снежник, наледь</td></tr>
<tr><td><b>area:0x4e</b></td><td>summer houses, etc. / дачи, сад.уч., д/о, п/л</td></tr>
<tr><td><b>area:0x4f</b></td><td>deforestation area / свежая вырубка</td></tr>
<tr><td><b>area:0x50</b></td><td>old deforestation area / старая вырубка</td></tr>
<tr><td><b>area:0x51</b></td><td>marsh / заболоченность</td></tr>
<tr><td><b>area:0x52</b></td><td>open area / открытое пространство</td></tr>
<tr><td><b>area:0x53</b></td><td>island / остров</td></tr>
<tr><td><b>area:0x54</b></td><td>open rock / открытые скалы</td></tr>
<tr><td><b>area:0x58</b></td><td>mining/deposition area / карьеры и отвалы грунта</td></tr>
<tr><td><b>area:0xff01</b></td><td>test area R / тестовая площадь R</td></tr>
<tr><td><b>area:0xff02</b></td><td>test area G / тестовая площадь G</td></tr>
<tr><td><b>area:0xff03</b></td><td>test area B / тестовая площадь B</td></tr>
<tr><td><b>area:0xff04</b></td><td>test area C / тестовая площадь C</td></tr>
<tr><td><b>area:0xff05</b></td><td>test area M / тестовая площадь M</td></tr>
<tr><td><b>area:0xff06</b></td><td>test area Y / тестовая площадь Y</td></tr>
<tr><td><b>line:0x00</b></td><td>кривая надпись / </td></tr>
<tr><td><b>line:0x01</b></td><td>автомагистраль / </td></tr>
<tr><td><b>line:0x02</b></td><td>шоссе / </td></tr>
<tr><td><b>line:0x03</b></td><td>top edge of steep slope / верхний край обрыва</td></tr>
<tr><td><b>line:0x04</b></td><td>проезжий грейдер / </td></tr>
<tr><td><b>line:0x05</b></td><td>building / строение</td></tr>
<tr><td><b>line:0x06</b></td><td>проезжая грунтовка / </td></tr>
<tr><td><b>line:0x07</b></td><td>road embankment / дорожная насыпь</td></tr>
<tr><td><b>line:0x08</b></td><td>мост-1 (пешеходный) / </td></tr>
<tr><td><b>line:0x09</b></td><td>мост-2 (автомобильный) / </td></tr>
<tr><td><b>line:0x0a</b></td><td>непроезжая грунтовка / </td></tr>
<tr><td><b>line:0x0b</b></td><td>большое шоссе / </td></tr>
<tr><td><b>line:0x0c</b></td><td>mountain ridge / хребет</td></tr>
<tr><td><b>line:0x0d</b></td><td>УЖД и подъездные пути / </td></tr>
<tr><td><b>line:0x0e</b></td><td>мост-5 (на автомагистралях) / </td></tr>
<tr><td><b>line:0x0f</b></td><td>small mountain ridge / малый хребет</td></tr>
<tr><td><b>line:0x15</b></td><td>river-1 / река-1</td></tr>
<tr><td><b>line:0x16</b></td><td>просека / </td></tr>
<tr><td><b>line:0x18</b></td><td>river-2 / река-2</td></tr>
<tr><td><b>line:0x19</b></td><td>fence / забор</td></tr>
<tr><td><b>line:0x1a</b></td><td>small power line / маленькая ЛЭП</td></tr>
<tr><td><b>line:0x1b</b></td><td>пешеходный тоннель / </td></tr>
<tr><td><b>line:0x1c</b></td><td>просека широкая / </td></tr>
<tr><td><b>line:0x1d</b></td><td>national boundaries / граница стран, областей</td></tr>
<tr><td><b>line:0x1e</b></td><td>bottom edge of steep slope / нижний край обрыва</td></tr>
<tr><td><b>line:0x1f</b></td><td>river-3 / река-3</td></tr>
<tr><td><b>line:0x20</b></td><td>dashed contour / пунктирная горизонталь</td></tr>
<tr><td><b>line:0x21</b></td><td>contour / горизонтали</td></tr>
<tr><td><b>line:0x22</b></td><td>thick contour / жирная горизонталь</td></tr>
<tr><td><b>line:0x23</b></td><td>vegetation boundary / контур растительности</td></tr>
<tr><td><b>line:0x24</b></td><td>glacier crevasse / ледниковая трещина</td></tr>
<tr><td><b>line:0x25</b></td><td>gulley / овраг</td></tr>
<tr><td><b>line:0x26</b></td><td>пересыхающий ручей / </td></tr>
<tr><td><b>line:0x27</b></td><td>железная дорога / </td></tr>
<tr><td><b>line:0x28</b></td><td>pipeline / трубопровод</td></tr>
<tr><td><b>line:0x29</b></td><td>power line / ЛЭП</td></tr>
<tr><td><b>line:0x2a</b></td><td>тропа / </td></tr>
<tr><td><b>line:0x2b</b></td><td>dry ditch / сухая канава</td></tr>
<tr><td><b>line:0x2c</b></td><td>levee / вал</td></tr>
<tr><td><b>line:0x2d</b></td><td>заросшая дорога / </td></tr>
<tr><td><b>line:0x2e</b></td><td>заросшая заметная дорога (знак ММБ) / </td></tr>
<tr><td><b>line:0x32</b></td><td>route quality 1 / </td></tr>
<tr><td><b>line:0x33</b></td><td>route quality 2 / </td></tr>
<tr><td><b>line:0x34</b></td><td>route quality 3 / </td></tr>
<tr><td><b>line:0x35</b></td><td>route quality 4 / </td></tr>
<tr><td><b>line:0x36</b></td><td>district boundaries / граница областей, районов</td></tr>
<tr><td><b>line:0x37</b></td><td>граница заповедников, парков / </td></tr>
<tr><td><b>line:0x40</b></td><td>irrigation channel / канава, арык</td></tr>
<tr><td><b>line:0x41</b></td><td>canal / канал</td></tr>
<tr><td><b>line:0x42</b></td><td>ferry / паром</td></tr>
<tr><td><b>line:0x43</b></td><td>cable way, ski lift / канатная дорога, лыжный подъемник</td></tr>
<tr><td><b>line:0x44</b></td><td>runway / асфальтовая ВПП</td></tr>
<tr><td><b>line:0x45</b></td><td>sod runway / грунтовая ВПП</td></tr>
<tr><td><b>line:0x46</b></td><td>coastline / четкая береговая линия</td></tr>
<tr><td><b>line:0x47</b></td><td>uncertain coastline / нечеткая береговая линия</td></tr>
<tr><td><b>line:0x48</b></td><td>rapids / пороги</td></tr>
<tr><td><b>line:0x49</b></td><td>forest strip / лесополоса</td></tr>
<tr><td><b>line:0x50</b></td><td>bad route /не использовать!/ / </td></tr>
<tr><td><b>line:0x51</b></td><td>moraine boundary / граница морен</td></tr>
<tr><td><b>line:0x7e</b></td><td>border object / </td></tr>
<tr><td><b>line:0x7f</b></td><td>text object / </td></tr>
<tr><td><b>line:0xff01</b></td><td>test line R / тестовая линия R</td></tr>
<tr><td><b>line:0xff02</b></td><td>test line G / тестовая линия G</td></tr>
<tr><td><b>line:0xff03</b></td><td>test line B / тестовая линия B</td></tr>
<tr><td><b>line:0xff04</b></td><td>test line C / тестовая линия C</td></tr>
<tr><td><b>line:0xff05</b></td><td>test line M / тестовая линия M</td></tr>
<tr><td><b>line:0xff06</b></td><td>test line Y / тестовая линия Y</td></tr>
<tr><td><b>point:0x0700</b></td><td>big town / большой населенный пункт</td></tr>
<tr><td><b>point:0x0800</b></td><td>small town / средний населенный пункт</td></tr>
<tr><td><b>point:0x0900</b></td><td>village / маленький населенный пункт</td></tr>
<tr><td><b>point:0x0d00</b></td><td>hight mark / отметка высоты</td></tr>
<tr><td><b>point:0x0f00</b></td><td>triangulation point / триангуляционный знак</td></tr>
<tr><td><b>point:0x1000</b></td><td>water altitude mark / отметка уреза воды</td></tr>
<tr><td><b>point:0x1100</b></td><td>summit / вершина</td></tr>
<tr><td><b>point:0x2800</b></td><td>land reference point / точка привязки текста</td></tr>
<tr><td><b>point:0x2b03</b></td><td>camping place, fireplace / место стоянки, место для костра</td></tr>
<tr><td><b>point:0x2b04</b></td><td>open hut, shelter for tourists / открытая изба, навес для ночевки</td></tr>
<tr><td><b>point:0x2c04</b></td><td>monument / памятник</td></tr>
<tr><td><b>point:0x2c0b</b></td><td>church / церковь</td></tr>
<tr><td><b>point:0x2e00</b></td><td>shop / магазин</td></tr>
<tr><td><b>point:0x2f08</b></td><td>bus stop / остановка автобуса</td></tr>
<tr><td><b>point:0x5905</b></td><td>railway station / ж/д станция</td></tr>
<tr><td><b>point:0x6401</b></td><td>gate / ворота, проход в заборе</td></tr>
<tr><td><b>point:0x6402</b></td><td>building / дом, строение</td></tr>
<tr><td><b>point:0x6403</b></td><td>cemetery / кладбище</td></tr>
<tr><td><b>point:0x6406</b></td><td>перевал неизвестной сложности / </td></tr>
<tr><td><b>point:0x640c</b></td><td>mine / шахта</td></tr>
<tr><td><b>point:0x6411</b></td><td>башня, труба / tower, chimney, mast</td></tr>
<tr><td><b>point:0x6414</b></td><td>spring, water source / родник, источник воды</td></tr>
<tr><td><b>point:0x6415</b></td><td>ruins, archeological place / развалины, археологический объект</td></tr>
<tr><td><b>point:0x6508</b></td><td>waterfall / водопад</td></tr>
<tr><td><b>point:0x650a</b></td><td>icefall / ледопад</td></tr>
<tr><td><b>point:0x650b</b></td><td>small hydrography reference point / мелкая точка объекта гидрографии</td></tr>
<tr><td><b>point:0x650c</b></td><td>hydrography reference point / точка для объекта гидрографии</td></tr>
<tr><td><b>point:0x650e</b></td><td>rapid / порог</td></tr>
<tr><td><b>point:0x6600</b></td><td>hachure / бергштрих</td></tr>
<tr><td><b>point:0x6601</b></td><td>cave / пещера</td></tr>
<tr><td><b>point:0x6603</b></td><td>pit, sinkhole / яма</td></tr>
<tr><td><b>point:0x6606</b></td><td>watch tower, hunting structure, shepherds camp / смотровая башня, охотничья вышка, кормушка</td></tr>
<tr><td><b>point:0x6607</b></td><td>shepherds camp / стоянка пастухов</td></tr>
<tr><td><b>point:0x6608</b></td><td>crater / кратер</td></tr>
<tr><td><b>point:0x660a</b></td><td>tree, group of trees / дерево, группа деревьев</td></tr>
<tr><td><b>point:0x660b</b></td><td>canyon / каньон</td></tr>
<tr><td><b>point:0x6613</b></td><td>knoll, mound / холм, курган</td></tr>
<tr><td><b>point:0x6616</b></td><td>rock pillar, big boulder / скала-останец, камень</td></tr>
<tr><td><b>point:0x6618</b></td><td>logjam / завал</td></tr>
<tr><td><b>point:0x6700</b></td><td>simple pass / перевал н/к</td></tr>
<tr><td><b>point:0x6701</b></td><td>1A pass / перевал 1А</td></tr>
<tr><td><b>point:0x6702</b></td><td>1B pass / перевал 1Б</td></tr>
<tr><td><b>point:0x6703</b></td><td>2A pass / перевал 2А</td></tr>
<tr><td><b>point:0x6704</b></td><td>2B pass / перевал 2Б</td></tr>
<tr><td><b>point:0x6705</b></td><td>3A pass / перевал 3А</td></tr>
<tr><td><b>point:0x6706</b></td><td>3B pass / перевал 3Б</td></tr>
<tr><td><b>point:0xff01</b></td><td>test point R / тестовая точка R</td></tr>
<tr><td><b>point:0xff02</b></td><td>test point G / тестовая точка G</td></tr>
<tr><td><b>point:0xff03</b></td><td>test point B / тестовая точка B</td></tr>
<tr><td><b>point:0xff04</b></td><td>test point C / тестовая точка C</td></tr>
<tr><td><b>point:0xff05</b></td><td>test point M / тестовая точка M</td></tr>
<tr><td><b>point:0xff06</b></td><td>test point Y / тестовая точка Y</td></tr>
<tr><td><b>text:0x01</b></td><td>значение по умолчанию / </td></tr>
<tr><td><b>text:0x02</b></td><td>черный жирный прямой шрифт: города, деревни / </td></tr>
<tr><td><b>text:0x03</b></td><td>черный курсив: урочища, дачи и т.п. / </td></tr>
<tr><td><b>text:0x04</b></td><td>синий курсив: реки, озера, ледники / </td></tr>
<tr><td><b>text:0x05</b></td><td>вершины / </td></tr>
<tr><td><b>text:0x06</b></td><td>мелкие вершины / </td></tr>
<tr><td><b>text:0x07</b></td><td>перевалы / </td></tr>
<tr><td><b>text:0x08</b></td><td>отметки уреза воды / </td></tr>
<tr><td><b>text:0x09</b></td><td>номер ледника / </td></tr>
<tr><td><b>text:0xff01</b></td><td>test text R / тестовый текст R</td></tr>
<tr><td><b>text:0xff02</b></td><td>test text G / тестовый текст G</td></tr>
<tr><td><b>text:0xff03</b></td><td>test text B / тестовый текст B</td></tr>
<tr><td><b>text:0xff04</b></td><td>test text C / тестовый текст C</td></tr>
<tr><td><b>text:0xff05</b></td><td>test text M / тестовый текст M</td></tr>
<tr><td><b>text:0xff06</b></td><td>test text Y / тестовый текст Y</td></tr>

</table>
