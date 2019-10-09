///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include <sstream>
#include "geo_mkref.h"
#include "geo_data/geo_io.h"
#include "geo_data/geo_utils.h"
#include "geo_data/conv_geo.h"
#include "geo_nom/geo_nom.h"
#include "err/assert_err.h"

int
main(){
  try {

    Opt o;
    GeoMap map;

    /**** nom ****/

    { // nomenclature map with margins
      Opt o = Opt("{\"mkref\":\"nom\", \"name\":\"n37-001\", \"dpi\":\"200\"}");

      GeoMap map = geo_mkref(o);
      assert_eq(map.name, "n37-001");
      assert_eq(map.proj, "SU39");
      assert_eq(map.image_dpi, 200);
      assert_eq(map.image_size, iPoint(2583,3020));
      assert_eq(type_to_str(rint(map.border)),
            "[[[0,2921],[1239,2972],[2477,3019],[2582,98],[1355,51],[127,0]]]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert_eq(ss.str(),
        "[0,2922] [35.9980041,55.6665025]"
        "[127,1] [35.9980557,55.9998604]"
        "[2476,3020] [36.497906,55.6665348]"
        "[2582,99] [36.4979852,55.9998413]");
    }

    { // nomenclature map with margins
      Opt o = Opt("{\"mkref\":\"nom\", \"name\":\"n37-001\", \"dpi\":\"100\","
                  "\"margins\": \"100\", \"top_margin\": \"200\" }");
      GeoMap map = geo_mkref(o);
      assert_eq(map.name, "n37-001");
      assert_eq(map.proj, "SU39");
      assert_eq(map.image_dpi, 100);
      assert_eq(map.image_size, iPoint(1492,1810));
      assert_eq(type_to_str(rint(map.border)),
          "[[[100,1661],[719,1686],[1338,1710],[1391,249],[777,226],[164,200]]]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert_eq(ss.str(),
        "[100,1661] [35.9980041,55.6665025]"
        "[163,201] [35.9978612,55.9997415]"
        "[1338,1710] [36.497906,55.6665348]"
        "[1391,250] [36.4979926,55.9997274]");
    }

    { // nomenclature map with margins -- write map for manual test
      Opt o = Opt("{\"mkref\":\"nom\", \"name\":\"m47-022\", \"dpi\":\"50\","
                  "\"margins\": \"0\", \"top_margin\": \"0\" }");
      o.put("top_margin",16);
      o.put("left_margin",18);
      o.put("bottom_margin",775-16-747);
      o.put("right_margin",733-18-702);
      GeoMap map = geo_mkref(o);
      assert_eq(map.name, "m47-022");
      assert_eq(map.proj, "SU99");
      assert_eq(map.image_dpi, 50);
      assert_eq(map.image_size, iPoint(732,775));
      assert_eq(type_to_str(rint(map.border)),
          "[[[33,762],[376,755],[719,746],[699,16],[359,25],[18,32]]]");

      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert_eq(ss.str(),
        "[18,33] [100.49987,51.6668599]"
        "[33,763] [100.4999,51.3335156]"
        "[699,17] [100.999943,51.6667138]"
        "[718,747] [100.999251,51.3333618]");
      map.image = "m47-022.jpg";
      write_ozi_map("test_data/m47-022.map", map, Opt());
    }

    /**** tms/google tiles ****/

    { // single TMS tile
      GeoMap map = geo_mkref(Opt("{\"mkref\": \"tms_tile\", \"tiles\": \"[1,1,10]\"}"));
      GeoMap map1 = geo_mkref(Opt("{\"mkref\": \"tms_tile\", \"tiles\": \"[1,1]\", \"zindex\":\"10\"}"));
      assert(map1 == map);

      assert_eq(map.name, "[1,1,1,1]");
      assert_eq(map.proj, "WEB");
      assert_eq(map.image_dpi, 300);
      assert_eq(map.image_size, iPoint(256,256));
      assert_eq(type_to_str(map.border), "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert_eq(ss.str(),
        "[0,0] [-179.648438,-84.9901002]"
        "[0,256] [-179.648438,-85.0207077]"
        "[256,0] [-179.296875,-84.9901002]"
        "[256,256] [-179.296875,-85.0207077]");
    }

    { // 2x3 TMS tile range
      GeoMap map = geo_mkref(Opt("{\"mkref\": \"tms_tile\", \"tiles\": \"[1,1,2,3]\", \"zindex\":\"3\"}"));
      assert_eq(map.name, "[1,1,2,3]");
      assert_eq(map.proj, "WEB");
      assert_eq(map.image_dpi, 300);
      assert_eq(map.image_size, iPoint(256*2,256*3));
      assert_eq(type_to_str(map.border), "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert_eq(ss.str(),
        "[0,0] [-135,0]"
        "[0,768] [-135,-79.1713346]"
        "[512,0] [-45,0]"
        "[512,768] [-45,-79.1713346]");
    }

    { // single TMS tile covering a given point
      GeoMap map = geo_mkref(Opt("{\"mkref\": \"tms_tile\", \"coords_wgs\": \"[64.0,32.0]\", \"zindex\":\"3\"}"));
      assert_eq(map.name, "[5,4,1,1]");
      assert_eq(map.proj, "WEB");
      assert_eq(map.image_dpi, 300);
      assert_eq(map.image_size, iPoint(256,256));
      assert_eq(type_to_str(map.border), "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert_eq(ss.str(),
        "[0,0] [45,40.9798981]"
        "[0,256] [45,0]"
        "[256,0] [90,40.9798981]"
        "[256,256] [90,0]");
    }

    { // single google tile covering a given point
      GeoMap map = geo_mkref(Opt("{\"mkref\": \"google_tile\", \"coords_wgs\": \"[64.0,32.0]\", \"zindex\":\"3\"}"));
      assert_eq(map.name, "[5,3,1,1]");
      assert_eq(map.proj, "WEB");
      assert_eq(map.image_dpi, 300);
      assert_eq(map.image_size, iPoint(256,256));
      assert_eq(type_to_str(map.border), "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert_eq(ss.str(),
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
      assert_eq(map.name, "[86,74,2,3]");
      assert_eq(map.proj, "WEB");
      assert_eq(map.image_dpi, 300);
      assert_eq(map.image_size, iPoint(256*2,256*3));
      assert_eq(type_to_str(map.border), 
        "[[[193.422222,250.866843],[216.177778,277.663273],[233.244444,297.713129],"
        "[258.844444,327.712532],[284.444444,357.622598],[261.688889,384.135334],"
        "[241.777778,407.277563],[224.355556,427.484296],[193.866667,462.751329],"
        "[171,489.123493],[153.85,508.859246],[128.125,538.394019],[102.4,567.847206]]]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert_eq(ss.str(),
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
      assert_eq(map.name, "[9410,4633,1,1]");
      assert_eq(map.proj, "WEB");
      assert_eq(map.image_dpi, 300);
      assert_eq(map.image_size, iPoint(256,256));
      assert_eq(type_to_str(map.border), "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert_eq(ss.str(),
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
      o.put("proj", "SU39");
      o.put("coords", "[7376000,6208000,2000,2000]");
      o.put("scale", 1000);
      GeoMap map = geo_mkref(o);

      assert_eq(map.name, "");
      assert_eq(map.proj, "SU39");
      assert_eq(map.image_dpi, 300);
      assert_eq(map.image_size, iPoint(237,237));
      assert_eq(type_to_str(map.border), "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert_eq(ss.str(),
        "[0,0] [37.0107346,55.9959095]"
        "[0,237] [37.0116594,55.9778984]"
        "[237,0] [37.0428731,55.9964239]"
        "[237,237] [37.043783,55.9784124]");
    }

    { // L-shaped map, Gauss-Kruger projection, 1:100'000, 300dpi
      dLine L("[[7376000,6208000],[7380000,6208000],[7380000,6212000],[7378000,6212000],[7378000,6210000],[7376000,6210000]]");
      Opt o;
      o.put("mkref", "proj");
      o.put("proj", "SU39");
      o.put("scale", 1000);
      o.put("coords", L);
      o.put("border", L);
      GeoMap map = geo_mkref(o);
      assert_eq(map.name, "");
      assert_eq(map.proj, "SU39");
      assert_eq(map.image_dpi, 300);
      assert_eq(map.image_size, iPoint(473,473));
      assert_eq(type_to_str(map.border),
        "[[[0,473],[473,473],[473,0],[236,0],[236,236],[0,236]]]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert_eq(ss.str(),
        "[0,0] [37.0098126,56.0138446]"
        "[0,473] [37.0116594,55.9778984]"
        "[473,0] [37.0739848,56.0148637]"
        "[473,473] [37.0757721,55.978916]");
    }


    { // rectangular map defined by wgs rectangle, Gauss-Kruger projection, 1:100'000, 300dpi
      dLine L("[[24.801507,60.173730],[24.799790,60.176675],[24.805498,60.177358],[24.806914,60.174498]]");
      Opt o;
      o.put("mkref", "proj");
      o.put("proj", "SU27");
      o.put("scale", 250);
      o.put("coords_wgs", L.bbox());
      GeoMap map = geo_mkref(o);

      assert_eq(map.name, "");
      assert_eq(map.proj, "SU27");
      assert_eq(map.image_dpi, 300);
      assert_eq(type_to_str(map.border), "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
//std::cerr << "IMG: " << map.image_size << "\n";
//std::cerr << "BRD: " << map.border << "\n";
//std::cerr << "REF: " << ss.str() << "\n";
      assert_eq(ss.str(),
        "[0,0] [24.7995093,60.1773656]"
        "[0,198] [24.7997608,60.1736068]"
        "[195,0] [24.8069407,60.1774888]"
        "[195,198] [24.8071913,60.1737299]");
    }

    { // rectangular map defined by wgs border, Gauss-Kruger projection, 1:100'000, 300dpi
      dLine L("[[24.801507,60.173730],[24.799790,60.176675],[24.805498,60.177358],[24.806914,60.174498]]");
      Opt o;
      o.put("mkref", "proj");
      o.put("proj", "SU27");
      o.put("scale", 250);
      o.put("coords_wgs", L);
      o.put("border_wgs", L);
      o.put("margins", 10);
      o.put("left_margin", 5);
      o.put("top_margin", 15);
      GeoMap map = geo_mkref(o);
      assert_eq(map.name, "");
      assert_eq(map.proj, "SU27");
      assert_eq(map.image_dpi, 300);
      assert_eq(map.image_size, iPoint(199,214));
      assert_eq(type_to_str(map.border),
         "[[[45,203],[5,47],[156,16],[188,167]]]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert_eq(ss.str(),
        "[0,0] [24.7995347,60.1775561]"
        "[0,214] [24.7998066,60.1734935]"
        "[199,0] [24.8071186,60.1776818]"
        "[199,214] [24.8073895,60.1736192]");
    }

    { // 2x2 km map, Gauss-Kruger projection, 1:100'000, 300dpi -- write map for manual test
      Opt o;
      o.put("mkref", "proj");
      o.put("proj", "SU99");
      o.put("coords", "[17552000,5624000,12000,6000]");
      o.put("dpi", 200);
      o.put("scale", 1000);
      GeoMap map = geo_mkref(o);
      assert_eq(map.name, "");
      assert_eq(map.proj, "SU99");
      assert_eq(map.image_dpi, 200);
      assert_eq(map.image_size, iPoint(946,474));
      assert_eq(type_to_str(map.border), "[]");
      std::ostringstream ss;
      for (auto & r:map.ref) ss << r.first << " " << r.second;
      assert_eq(ss.str(),
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
