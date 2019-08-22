///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <sstream>
#include "geo_mkref.h"
#include "geo_data/geo_io.h"
#include "geo_data/conv_geo.h"

int
main(){
  try {

    Opt o;
    GeoMap map;

    /**** nom ****/

    { // nomenclature map with margins
      Opt o = Opt("{\"mkref\":\"nom\", \"name\":\"n37-001\", \"dpi\":\"200\"}");
      GeoMap map = geo_mkref(o);
      assert(map.name == "n37-001");
      assert(map.proj == "+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=500000 +lon_0=39");
      assert(map.image_dpi == 200);
      assert(map.image_size == iPoint(2583,3020));
      assert(type_to_str(map.border) ==
          "[[[0.249546675,2921.09993],[1238.4568,2972.44913],[2476.75854,3019.3302],"
          "[2582.29167,98.0292246],[1354.55017,51.3640527],[126.905148,0.251437578]]]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0.249546675,2921.09993] [35.9980672,55.6666068]"
        "[126.905148,0.251437578] [35.9980506,55.9999457]"
        "[2476.75854,3019.3302] [36.4980747,55.6666146]"
        "[2582.29167,98.0292246] [36.4980582,55.9999536]");
    }

    { // nomenclature map with margins
      Opt o = Opt("{\"mkref\":\"nom\", \"name\":\"n37-001\", \"dpi\":\"100\","
                  "\"margins\": \"100\", \"top_margin\": \"200\" }");
      GeoMap map = geo_mkref(o);
      assert(map.name == "n37-001");
      assert(map.proj == "+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=500000 +lon_0=39");
      assert(map.image_dpi == 100);
      assert(map.image_size == iPoint(1492,1810));
      assert(type_to_str(map.border) ==
        "[[[100.624773,1660.54996],[719.728398,1686.22457],[1338.87927,1709.6651],"
        "[1391.64583,249.014612],[777.775084,225.682026],[163.952574,200.125719]]]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[100.624773,1660.54996] [35.9980672,55.6666068]"
        "[163.952574,200.125719] [35.9980506,55.9999457]"
        "[1338.87927,1709.6651] [36.4980747,55.6666146]"
        "[1391.64583,249.014612] [36.4980582,55.9999536]");
    }

    { // nomenclature map with margins -- write map for manual test
      Opt o = Opt("{\"mkref\":\"nom\", \"name\":\"m47-022\", \"dpi\":\"50\","
                  "\"margins\": \"0\", \"top_margin\": \"0\" }");
      o.put("top_margin",16);
      o.put("left_margin",18);
      o.put("bottom_margin",775-16-747);
      o.put("right_margin",733-18-702);
      GeoMap map = geo_mkref(o);
      assert(map.name == "m47-022");
      assert(map.proj == "+ellps=krass +towgs84=28,-130,-95 +proj=tmerc +x_0=500000 +lon_0=99");
      assert(map.image_dpi == 50);
      assert(map.image_size == iPoint(733,775));
      assert(type_to_str(map.border) ==
        "[[[33.381525,762.471297],[376.336858,754.874646],[719.282191,746.108754],"
        "[699.333161,16.1578047],[358.882564,24.9002931],[18.421525,32.4766734]]]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[18.421525,32.4766734] [100.499944,51.6670979]"
        "[33.381525,762.471297] [100.499945,51.3337565]"
        "[699.333161,16.1578047] [100.999961,51.6670981]"
        "[719.282191,746.108754] [100.999962,51.3337567]");
      map.image = "m47-022.jpg";
      write_ozi_map("test_data/m47-022.map", map, Opt());
    }

    /**** tms/google tiles ****/

    { // single TMS tile
      GeoMap map = geo_mkref(Opt("{\"mkref\": \"tms_tile\", \"tiles\": \"[1,1,10]\"}"));
      GeoMap map1 = geo_mkref(Opt("{\"mkref\": \"tms_tile\", \"tiles\": \"[1,1]\", \"zindex\":\"10\"}"));
      assert(map1 == map);

      assert(map.name == "[1,1,1,1]");
      assert(map.proj == GEO_PROJ_WEB);
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(256,256));
      assert(type_to_str(map.border) == "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [-179.648438,-84.9901002]"
        "[0,256] [-179.648438,-85.0207077]"
        "[256,0] [-179.296875,-84.9901002]"
        "[256,256] [-179.296875,-85.0207077]");
    }

    { // 2x3 TMS tile range
      GeoMap map = geo_mkref(Opt("{\"mkref\": \"tms_tile\", \"tiles\": \"[1,1,2,3]\", \"zindex\":\"3\"}"));
      assert(map.name == "[1,1,2,3]");
      assert(map.proj == GEO_PROJ_WEB);
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(256*2,256*3));
      assert(type_to_str(map.border) == "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [-135,0]"
        "[0,768] [-135,-79.1713346]"
        "[512,0] [-45,0]"
        "[512,768] [-45,-79.1713346]");
    }

    { // single TMS tile covering a given point
      GeoMap map = geo_mkref(Opt("{\"mkref\": \"tms_tile\", \"coords_wgs\": \"[64.0,32.0]\", \"zindex\":\"3\"}"));
      assert(map.name == "[5,4,1,1]");
      assert(map.proj == GEO_PROJ_WEB);
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(256,256));
      assert(type_to_str(map.border) == "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [45,40.9798981]"
        "[0,256] [45,0]"
        "[256,0] [90,40.9798981]"
        "[256,256] [90,0]");
    }

    { // single google tile covering a given point
      GeoMap map = geo_mkref(Opt("{\"mkref\": \"google_tile\", \"coords_wgs\": \"[64.0,32.0]\", \"zindex\":\"3\"}"));
      assert(map.name == "[5,3,1,1]");
      assert(map.proj == GEO_PROJ_WEB);
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(256,256));
      assert(type_to_str(map.border) == "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [45,40.9798981]"
        "[0,256] [45,0]"
        "[256,0] [90,40.9798981]"
        "[256,256] [90,0]");
    }

    { // tms tiles covering a triangular area.
      GeoMap map = geo_mkref(Opt("{"
        "\"mkref\": \"tms_tile\", "
        "\"border_wgs\": \"[[64,32],[65,31],[63,29]]\", "
        "\"coords_wgs\": \"[[64,32],[65,31],[63,29]]\", "
        "\"zindex\":\"7\"}"));
      assert(map.name == "[86,74,2,3]");
      assert(map.proj == GEO_PROJ_WEB);
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(256*2,256*3));
      assert(type_to_str(map.border) == 
        "[[[193.422222,250.866843],[216.177778,277.663273],[233.244444,297.713129],"
        "[258.844444,327.712532],[284.444444,357.622598],[261.688889,384.135334],"
        "[241.777778,407.277563],[224.355556,427.484296],[193.866667,462.751329],"
        "[171,489.123493],[153.85,508.859246],[128.125,538.394019],[102.4,567.847206]]]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [61.875,34.3071439]"
        "[0,768] [61.875,27.0591258]"
        "[512,0] [67.5,34.3071439]"
        "[512,768] [67.5,27.0591258]");
    }

    { // single google tile covering a given point -- write map for manual test
      GeoMap map = geo_mkref(Opt("{"
        "\"mkref\": \"google_tile\", "
        "\"coords_wgs\": \"[26.77188,61.33552]\", "
        "\"zindex\":\"14\"}"));
      assert(map.name == "[9410,4633,1,1]");
      assert(map.proj == GEO_PROJ_WEB);
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(256,256));
      assert(type_to_str(map.border) == "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [26.7626953,61.3440784]"
        "[0,256] [26.7626953,61.3335397]"
        "[256,0] [26.784668,61.3440784]"
        "[256,256] [26.784668,61.3335397]");
      map.image = "9410_4633_14.png";
      write_ozi_map("test_data/9410_4633_14.map", map, Opt());

    }


    /**** proj ****/

    { // 2x2 km map, Gauss-Kruger projection, 1:100'000, 300dpi
      Opt o;
      o.put("mkref", "proj");
      o.put("proj", GEO_PROJ_SU(39,7));
      o.put("coords", "[7376000,6208000,2000,2000]");
      o.put("scale", 1000);
      GeoMap map = geo_mkref(o);

      assert(map.name == "");
      assert(map.proj == GEO_PROJ_SU(39,7));
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(237,237));
      assert(type_to_str(map.border) == "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [37.0107346,55.9959095]"
        "[0,237] [37.0116594,55.9778984]"
        "[237,0] [37.0428731,55.9964239]"
        "[237,237] [37.043783,55.9784124]");
    }

    { // L-shaped map, Gauss-Kruger projection, 1:100'000, 300dpi
      dLine L("[[7376000,6208000],[7380000,6208000],[7380000,6212000],[7378000,6212000],[7378000,6210000],[7376000,6210000]]");
      Opt o;
      o.put("mkref", "proj");
      o.put("proj", GEO_PROJ_SU(39,7));
      o.put("scale", 1000);
      o.put("coords", L);
      o.put("border", L);
      GeoMap map = geo_mkref(o);
      assert(map.name == "");
      assert(map.proj == GEO_PROJ_SU(39,7));
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(473,473));
      assert(type_to_str(map.border) ==
        "[[[0,473],[473,473],[473,0],[236,0],[236,236],[0,236]]]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [37.0098126,56.0138446]"
        "[0,473] [37.0116594,55.9778984]"
        "[473,0] [37.0739848,56.0148637]"
        "[473,473] [37.0757721,55.978916]");
    }


    { // rectangular map defined by wgs rectangle, Gauss-Kruger projection, 1:100'000, 300dpi
      dLine L("[[24.801507,60.173730],[24.799790,60.176675],[24.805498,60.177358],[24.806914,60.174498]]");
      Opt o;
      o.put("mkref", "proj");
      o.put("proj", GEO_PROJ_SU(29,5));
      o.put("scale", 250);
      o.put("coords_wgs", L.bbox());
      GeoMap map = geo_mkref(o);

      assert(map.name == "");
      assert(map.proj == GEO_PROJ_SU(29,5));
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(200,204));
      assert(type_to_str(map.border) == "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
//std::cerr << "IMG: " << map.image_size << "\n";
//std::cerr << "BRD: " << map.border << "\n";
//std::cerr << "REF: " << ss.str() << "\n";
      assert(ss.str() ==
        "[0,0] [24.7993147,60.1773537]"
        "[0,204] [24.799809,60.1734885]"
        "[200,0] [24.8069217,60.1775949]"
        "[200,204] [24.8074152,60.1737297]");
    }

    { // rectangular map defined by wgs border, Gauss-Kruger projection, 1:100'000, 300dpi
      dLine L("[[24.801507,60.173730],[24.799790,60.176675],[24.805498,60.177358],[24.806914,60.174498]]");
      Opt o;
      o.put("mkref", "proj");
      o.put("proj", GEO_PROJ_SU(27,5));
      o.put("scale", 250);
      o.put("coords_wgs", L);
      o.put("border_wgs", L);
      o.put("margins", 10);
      o.put("left_margin", 5);
      o.put("top_margin", 15);
      GeoMap map = geo_mkref(o);
      assert(map.name == "");
      assert(map.proj == GEO_PROJ_SU(27,5));
      assert(map.image_dpi == 300);
      assert(map.image_size == iPoint(199,214));
      assert(type_to_str(map.border) ==
         "[[[45,203],[5,47],[156,16],[188,167]]]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [24.7995347,60.1775561]"
        "[0,214] [24.7998066,60.1734935]"
        "[199,0] [24.8071186,60.1776818]"
        "[199,214] [24.8073895,60.1736192]");
    }

    { // 2x2 km map, Gauss-Kruger projection, 1:100'000, 300dpi -- write map for manual test
      Opt o;
      o.put("mkref", "proj");
      o.put("proj", GEO_PROJ_SU(99,17));
      o.put("coords", "[17552000,5624000,12000,6000]");
      o.put("dpi", 200);
      o.put("scale", 1000);
      GeoMap map = geo_mkref(o);
      assert(map.name == "");
      assert(map.proj == GEO_PROJ_SU(99,17));
      assert(map.image_dpi == 200);
      assert(map.image_size == iPoint(946,474));
      assert(type_to_str(map.border) == "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert(ss.str() ==
        "[0,0] [99.7374411,50.7987634]"
        "[0,474] [99.7365908,50.744654]"
        "[946,0] [99.9078347,50.7975618]"
        "[946,474] [99.9067879,50.7434547]");
      map.image = "12x6+17552+5624k.png";
      write_ozi_map("test_data/12x6+17552+5624k.map", map, Opt());

    }

  }
  catch (Err E){
    std::cerr << "Error: " << E.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
