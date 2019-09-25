HEADER(geodata,`
  ENRU(`Mapsoft2 geodata',`Геоданные в Mapsoft2')
')

<p>ENRU(`
TT(`GeoData') structure in mapsoft contains lists of tracks (TT(`GeoTrk')),
waypoint lists (TT(`GeoWptList')), and map lists (TT(`GeoMapList')). Tracks
contain trackpoints (TT(`GeoTpt')), waypoint lists contain waypoints
(TT(`GeoWpt')), and map lists contain maps (TT(`GeoMap')).',
`Структура TT(`GeoData') содержит треки  (TT(`GeoTrk')), списки путевых точек
(TT(`GeoWptList')), и списки карт (TT(`GeoMapList')). Треки содержат
точки треков (TT(`GeoTpt')), списки путевых точек содержат путевые точки
(TT(`GeoWpt')), а списки карт содердат карты (TT(`GeoMap')).')

<p>ENRU(`
Each structure has some mandatory parameters and (except for track point)
optional parameters inside TT(`opts') variable.',
`Каждая структура содержит несклько обязательных параметров и (за исключением)
точек трека) - дополнительные параметры в переменной TT(`opts').)
`

<ul>
<li>TT(`GeoWptList') --
 ENRU(`a waypoint list, a route',`список путевых точек, route')
