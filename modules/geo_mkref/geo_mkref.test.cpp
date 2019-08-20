///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <sstream>
#include "geo_mkref.h"

int
main(){
  try {

    Opt o;
    GeoMap map;

    /**** nom ****/

    { // nomenclature map with margins
      Opt o = Opt("{\"ref\":\"nom\", \"name\":\"n37-001\", \"dpi\":\"200\"}");
      GeoMap map = geo_mkref(o);
      assert(map.name == "n37-001");
      assert(map.proj == "+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=500000 +lon_0=39");
      assert(map.image_dpi == 200);
      assert(map.image_size == iPoint(2583,3020));
      assert(type_to_str(map.border) ==
        "[[[0.249546675,98.9000709],[1238.4568,47.5508663],[2476.75854,0.669798092],"
        "[2582.29167,2921.97078],[1354.55017,2968.63595],[126.905148,3019.74856]]]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0.249546675,98.9000709] [35.9980672,55.6666068]"
        "[126.905148,3019.74856] [35.9980506,55.9999457]"
        "[2476.75854,0.669798092] [36.4980747,55.6666146]"
        "[2582.29167,2921.97078] [36.4980582,55.9999536]");
    }

    { // nomenclature map with margins
      Opt o = Opt("{\"ref\":\"nom\", \"name\":\"n37-001\", \"dpi\":\"100\","
                  "\"margins\": \"100\", \"top_margin\": \"200\" }");
      GeoMap map = geo_mkref(o);
      assert(map.name == "n37-001");
      assert(map.proj == "+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=500000 +lon_0=39");
      assert(map.image_dpi == 100);
      assert(map.image_size == iPoint(1492,1810));
      assert(type_to_str(map.border) ==
        "[[[100.624773,249.450035],[719.728398,223.775433],[1338.87927,200.334899],"
        "[1391.64583,1660.98539],[777.775084,1684.31797],[163.952574,1709.87428]]]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[100.624773,249.450035] [35.9980672,55.6666068]"
        "[163.952574,1709.87428] [35.9980506,55.9999457]"
        "[1338.87927,200.334899] [36.4980747,55.6666146]"
        "[1391.64583,1660.98539] [36.4980582,55.9999536]");
    }

    /**** tms/google tiles ****/

    { // single TMS tile
      GeoMap map = geo_mkref(Opt("{\"ref\": \"tms_tile\", \"tiles\": \"[1,1,10]\"}"));
      GeoMap map1 = geo_mkref(Opt("{\"ref\": \"tms_tile\", \"tiles\": \"[1,1]\", \"zindex\":\"10\"}"));
      assert(map1 == map);

      assert(map.name == "[1,1,1,1]");
      assert(map.proj == "+proj=webmerc +datum=WGS84");
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(256,256));
      assert(type_to_str(map.border) == "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [-179.648438,-85.0207077]"
        "[0,256] [-179.648438,-84.959305]"
        "[256,0] [-178.945312,-85.0207077]"
        "[256,256] [-178.945312,-84.959305]");
    }

    { // 2x3 TMS tile range
      GeoMap map = geo_mkref(Opt("{\"ref\": \"tms_tile\", \"tiles\": \"[1,1,2,3]\", \"zindex\":\"3\"}"));
      assert(map.name == "[1,1,2,3]");
      assert(map.proj == "+proj=webmerc +datum=WGS84");
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(256*2,256*3));
      assert(type_to_str(map.border) == "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [-135,-79.1713346]"
        "[0,768] [-135,40.9798981]"
        "[512,0] [0,-79.1713346]"
        "[512,768] [0,40.9798981]");
    }

    { // single TMS tile covering a given point
      GeoMap map = geo_mkref(Opt("{\"ref\": \"tms_tile\", \"coords\": \"[64.0,32.0]\", \"zindex\":\"3\"}"));
      assert(map.name == "[5,4,1,1]");
      assert(map.proj == "+proj=webmerc +datum=WGS84");
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(256,256));
      assert(type_to_str(map.border) == "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [45,0]"
        "[0,256] [45,66.5132604]"
        "[256,0] [135,0]"
        "[256,256] [135,66.5132604]");
    }

    { // single google tile covering a given point
      GeoMap map = geo_mkref(Opt("{\"ref\": \"google_tile\", \"coords\": \"[64.0,32.0]\", \"zindex\":\"3\"}"));
      assert(map.name == "[5,3,1,1]");
      assert(map.proj == "+proj=webmerc +datum=WGS84");
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(256,256));
      assert(type_to_str(map.border) == "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [45,-40.9798981]"
        "[0,256] [45,40.9798981]"
        "[256,0] [135,-40.9798981]"
        "[256,256] [135,40.9798981]");
    }

    { // tms tiles covering a triangular area.
      GeoMap map = geo_mkref(Opt("{\"ref\": \"tms_tile\", \"coords\": \"[[64,32],[65,31],[63,29]]\", \"zindex\":\"7\"}"));
      assert(map.name == "[86,74,2,3]");
      assert(map.proj == "+proj=webmerc +datum=WGS84");
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(256*2,256*3));
      assert(type_to_str(map.border) == 
        "[[[193.422222,517.133157],[216.177778,490.336727],[233.244444,470.286871],"
        "[258.844444,440.287468],[284.444444,410.377402],[261.688889,383.864666],"
        "[241.777778,360.722437],[224.355556,340.515704],[193.866667,305.248671],"
        "[171,278.876507],[153.85,259.140754],[128.125,229.605981],"
        "[102.4,200.152794]]]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [61.875,27.0591258]"
        "[0,768] [61.875,36.5978891]"
        "[512,0] [70.3125,27.0591258]"
        "[512,768] [70.3125,36.5978891]");
    }

    /**** proj ****/

    { // 2x2 km map, Gauss-Kruger projection, 1:100'000, 300dpi
      GeoMap map = geo_mkref(Opt(
        "{\"ref\": \"proj\", \"coords\": \"[7376000,6208000,2000,2000]\","
        "\"proj\":\"+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=7500000 +lon_0=39\","
        "\"scale\": \"1000\"}"));
      assert(map.name == "");
      assert(map.proj == "+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=7500000 +lon_0=39");
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(236,236));
      assert(type_to_str(map.border) == "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [37.011672,55.9779249]"
        "[0,236.220472] [37.0107502,55.9958768]"
        "[236.220472,0] [37.0436899,55.9784373]"
        "[236.220472,236.220472] [37.0427829,55.9963896]");
    }

    { // L-shaped map, Gauss-Kruger projection, 1:100'000, 300dpi
      GeoMap map = geo_mkref(Opt(
        "{\"ref\": \"proj\", \"coords\": \"[[7376000,6208000],[7380000,6208000],[7380000,6212000],[7378000,6212000],[7378000,6210000],[7376000,6210000]]\","
        "\"proj\":\"+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=7500000 +lon_0=39\","
        "\"scale\": \"1000\"}"));
      assert(map.name == "");
      assert(map.proj == "+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=7500000 +lon_0=39");
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(472,472));
      assert(type_to_str(map.border) == 
        "[[[0,0],[472.440945,0],[472.440945,472.440945],[236.220472,472.440945],"
        "[236.220472,236.220472],[0,236.220472]]]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [37.011672,55.9779249]"
        "[0,472.440945] [37.0098273,56.0138287]"
        "[472.440945,0] [37.0757089,55.9789414]"
        "[472.440945,472.440945] [37.0739236,56.0148465]");
    }

    dLine L("[[24.801507,60.173730],[24.799790,60.176675],[24.805498,60.177358],[24.806914,60.174498]]");

    { // rectangular map defined by wgs rectangle, Gauss-Kruger projection, 1:100'000, 300dpi
      Opt o("{\"ref\": \"proj\","
        "\"proj\":\"+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=5500000 +lon_0=39\","
        "\"scale\": \"250\"}");
      o.put("coords_wgs", L.bbox());
      GeoMap map = geo_mkref(o);

      assert(map.name == "");
      assert(map.proj == "+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=5500000 +lon_0=39");
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(225,228));
      assert(type_to_str(map.border) == "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [24.8001347,60.1729868]"
        "[0,228.281671] [24.7982819,60.1771911]"
        "[225.087023,0] [24.8084556,60.1738965]"
        "[225.087023,228.281671] [24.8066039,60.178101]");
    }

    { // rectangular map defined by wgs border, Gauss-Kruger projection, 1:100'000, 300dpi
      Opt o("{\"ref\": \"proj\","
        "\"proj\":\"+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=5500000 +lon_0=39\","
        "\"scale\": \"250\"}");
      o.put("coords_wgs", L);
      GeoMap map = geo_mkref(o);

      assert(map.name == "");
      assert(map.proj == "+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=5500000 +lon_0=39");
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(159,165));
      assert(type_to_str(map.border) ==
         "[[[10.8205654,0],[0,162.278897],[155.057754,165.329438],[159.089572,9.16016292]]]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [24.8011242,60.1736861]"
        "[0,165.329438] [24.7997825,60.1767311]"
        "[159.089572,0] [24.8070055,60.1743292]"
        "[159.089572,165.329438] [24.8056643,60.1773742]");
    }

  }
  catch (Err E){
    std::cerr << "Error: " << E.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
