HEADER(mkref, Mapsoft2)

<h4>ENRU(
`Options for making a map reference',
`Параметры для создания привязки карты')
</h4>

ENRU(`In many cases it is needed to draw some geodata on a raster image. For
this one have to specify a map reference for this image (GeoMap object):
datum and projection, position, scale, border etc. In mapsoft2 one can make
certain types of references using following options:',
`Во многих случаях геоданные нужно нарисовать на растровой картинке. Для
этого нужно создать привязанную карту для этого изображения (объект типа GeoMap).
В mapsoft2 можно построить несколько типов карт, используя параметр mkref
и, в зависимости от его значения, следующие параметры:')

<ul>
<li><pre><tt> --mref nom </tt></pre>
  -- ENRU(`A Soviet nomenclature map.',`Советская номенклатурная карта.')
  <br>ENRU(`Additional paramenters:',`Дополнительные параметры:')
  <ul>
  <li>--name <name> -- map name (mapsoft2 extensions are allowed).
  <li>--dpi <double> -- map resolution in dots per inch (default 300).
  <li>--mag <factor> -- rescale map without changing dpi setting (default 1).
  <li>--margins <pixels>
  <li>--top_margin <pixels>
  <li>--left_margin <pixels>
  <li>--right_margin <pixels>
  <li>--bottom_margin <pixels>
        Set margins between map border and image boundary.
  </ul>

<li><pre><tt>--mkref tms_tile</tt></pre>
<br><pre><tt>--mkref google_tile</tt></pre>
  -- ENRU(`A single google/TMS tile or rectangular area with multiple tiles.',
          `Плитка google/TMS или прямоугольная область из нескольких плиток.')
  <br>ENRU(`Additional paramenters:',`Дополнительные параметры:')
  <ul>
  <li>--zindex <integer>  -- tile zindex

  <li>--tiles <point or rectangle>
      Tile or tile range. If single point is given then non-zero z-coordinate
       overrides zindex option. --tiles "[1,2,3]" is same as --tiles "[1,2]" --zindex 0.

  <li>  --coords <wgs84 point, rectangle, line, or multiline>
      Build a map which covers given figure. If argument is Line or MultiLine then
      border is set to this line (can be ovrriden by --border option).

  <li>  --border <wgs84 line or multiline>
      Set map border.

  </ul>


<li><pre><tt>--mkref proj</tt></pre>
  -- ENRU(`A rectangular area in some coordinates.',
          `Прямоугольная область в каких-либо координатах.')
  <br>ENRU(`Additional paramenters:',`Дополнительные параметры:')
  <ul>

  --proj <proj string> -- set projection (libproj parameter string)
  --coords <rect, line or multiline in proj coords>
  --coords_wgs <rect, line or multiline in wgs84 coordinates>
       Set map area in projection or wgs84 coordinates
  --border <multiline in proj coords>
  --border_wgs <multiline in wgs84 coords>
       Set map border in projection or wgs84 coordinates
  --dpi
  --scale
      Scale (projection units per map cm), default 1000 for non-degree projections,
      1/100 for degree projections.
  --margins <pixels>
  --top_margin <pixels>
  --left_margin <pixels>
  --right_margin <pixels>
  --bottom_margin <pixels>
      Set map margins.
  </ul>
</ul>

map.name
map.proj == "+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=5500000 +lon_0=39"
map.image_dpi -- map resolution, dots per inch
map.image_size -- map image size (in pixels)
map.border -- map border (in pixels)
map.ref -- four reference points

