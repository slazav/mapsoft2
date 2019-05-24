-----------------
## ConvGeo class

Geo transformation, libproj wrapper, child of ConvBase.

Functions `rescale_src()` and `rescale_dst()` affects only `x` and `y`
coordinates. If point coordinate `z` is `NaN` then 2D transformation is
done.

Constructor:
 - `ConvGeo(const std::string & src,
            const std::string & dst = "+datum=WGS84 +proj=lonlat");`

-----------------
## ConvMap class

Transformation from GeoMap image points to some geo-projection.

Constructor:
 - `ConvGeo(const GeoMap & src,
            const std::string & dst = "+datum=WGS84 +proj=lonlat");`

