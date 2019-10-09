///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "err/assert_err.h"
#include "geo_nom.h"

void assert_nom(const std::string & name, const dRect & res, nom_scale_t sc0,
                bool ex=false, std::string error=""){
  nom_scale_t sc;
  dRect r;

  try {
    r = nom_to_range(name, sc, ex);
  } catch (Err e) {
    if (e.str() != error){
      std::cerr << "Error: " <<  name << ":\n";
      std::cerr << " error expected: " <<  error << "\n";
      std::cerr << " error recieved: " <<  e.str() << "\n";
      assert(false);
    }
  }

  if (res.is_empty()){
    if (r.is_empty()) return;
    std::cerr << "Error: " <<  name << " -> " << r << " != empty\n";
    assert(false);
  }

  if (dist(r,res) >= 1e-6){
    std::cerr << "Error: " <<  name << " -> " << r << " != " << res << "\n";
    assert(false);
  }

  if (sc != sc0){
    std::cerr << "Error: " << name << " sc= " << sc << " != " << sc0 << "\n";
    assert(false);
  }
}

void assert_nom1(const std::string & name, const bool single = false, std::string error=""){
  nom_scale_t sc;
  dRect r;
  try {
    r = nom_to_range(name, sc, single);
  } catch (Err e) {
    if (e.str() != error){
      std::cerr << "Error: " <<  name << ":\n";
      std::cerr << " error expected: " <<  error << "\n";
      std::cerr << " error recieved: " <<  e.str() << "\n";
      assert(false);
    }
  }

  std::string name1 = pt_to_nom(r.cnt(), sc, single);
  if (name1!=name) {
    std::cerr << "Error: " << name << " != " << name1 << "\n";
    assert(false);
  }
}


int
main(){
  try {
  nom_scale_t sc;

  assert_eq(str_to_type<nom_scale_t>("1:1000000"), SC_1M);
  assert_eq(str_to_type<nom_scale_t>("1:1'000'000"), SC_1M);
  assert_eq(str_to_type<nom_scale_t>("1000000"), SC_1M);
  assert_eq(str_to_type<nom_scale_t>("1M"), SC_1M);
  assert_eq(str_to_type<nom_scale_t>("10km/cm"), SC_1M);
  assert_eq(str_to_type<nom_scale_t>("10km"), SC_1M);

  assert_eq(str_to_type<nom_scale_t>("1:500000"), SC_500k);
  assert_eq(str_to_type<nom_scale_t>("1:500'000"), SC_500k);
  assert_eq(str_to_type<nom_scale_t>("500000"), SC_500k);
  assert_eq(str_to_type<nom_scale_t>("500k"), SC_500k);
  assert_eq(str_to_type<nom_scale_t>("5km/cm"), SC_500k);
  assert_eq(str_to_type<nom_scale_t>("5km"), SC_500k);

  assert_eq(str_to_type<nom_scale_t>("1:200000"), SC_200k);
  assert_eq(str_to_type<nom_scale_t>("1:200'000"), SC_200k);
  assert_eq(str_to_type<nom_scale_t>("200000"), SC_200k);
  assert_eq(str_to_type<nom_scale_t>("200k"), SC_200k);
  assert_eq(str_to_type<nom_scale_t>("2km/cm"), SC_200k);
  assert_eq(str_to_type<nom_scale_t>("2km"), SC_200k);

  assert_eq(str_to_type<nom_scale_t>("1:100000"), SC_100k);
  assert_eq(str_to_type<nom_scale_t>("1:100'000"), SC_100k);
  assert_eq(str_to_type<nom_scale_t>("100000"), SC_100k);
  assert_eq(str_to_type<nom_scale_t>("100k"), SC_100k);
  assert_eq(str_to_type<nom_scale_t>("1km/cm"), SC_100k);
  assert_eq(str_to_type<nom_scale_t>("1km"), SC_100k);

  assert_eq(str_to_type<nom_scale_t>("1:50000"), SC_50k);
  assert_eq(str_to_type<nom_scale_t>("1:50'000"), SC_50k);
  assert_eq(str_to_type<nom_scale_t>("50000"), SC_50k);
  assert_eq(str_to_type<nom_scale_t>("50k"), SC_50k);
  assert_eq(str_to_type<nom_scale_t>("500m/cm"), SC_50k);
  assert_eq(str_to_type<nom_scale_t>("500m"), SC_50k);

  assert_err(str_to_type<nom_scale_t>("a"),
    "can't parse nomenclature map scale: a");

  assert_err(str_to_type<nom_scale_t>("500m "),
    "can't parse value: 500m ");

  assert_eq(type_to_str<nom_scale_t>(SC_1M), "1:1000000");
  assert_eq(type_to_str<nom_scale_t>(SC_500k), "1:500000");
  assert_eq(type_to_str<nom_scale_t>(SC_200k), "1:200000");
  assert_eq(type_to_str<nom_scale_t>(SC_100k), "1:100000");
  assert_eq(type_to_str<nom_scale_t>(SC_50k), "1:50000");


  // 1:1'000'000
  assert_nom("",        dRect(), SC_1M, false, "nom_to_range: can't parse name: ");
  assert_nom("x10-001", dRect(), SC_1M, false, "nom_to_range: can't parse name: x10-001");
  assert_nom("aa-001",  dRect(), SC_1M, false, "nom_to_range: can't parse name: aa-001");
  assert_nom("a -001",  dRect(), SC_1M, false, "nom_to_range: can't parse name: a -001");
  assert_nom("v-01",    dRect(), SC_1M, false, "nom_to_range: can't parse name: v-01");
  assert_nom("xx-01",   dRect(), SC_1M, false, "nom_to_range: can't parse name: xx-01");
  assert_nom("a-61",    dRect(), SC_1M, false, "nom_to_range: can't parse name: a-61");
  assert_nom("a-0",     dRect(), SC_1M, false, "nom_to_range: can't parse name: a-0");
  assert_nom("a0",      dRect(), SC_1M, false, "nom_to_range: can't parse name: a0");
  assert_nom("a5",      dRect(), SC_1M, false, "nom_to_range: can't parse name: a5");
  assert_nom("a-019",   dRect(), SC_1M, false, "nom_to_range: can't parse name: a-019: extra symbols after the name");
  assert_nom("a019",    dRect(), SC_1M, false, "nom_to_range: can't parse name: a019: extra symbols after the name");
  assert_nom("R33,36",  dRect(), SC_1M, false, "nom_to_range: can't parse name: R33,36");
  assert_nom("R33,33",  dRect(), SC_1M, false, "nom_to_range: can't parse name: R33,33");
  assert_nom("R33,034", dRect(), SC_1M, false, "nom_to_range: can't parse name: R33,034");
  assert_nom("R33,34,", dRect(), SC_1M, false, "nom_to_range: can't parse name: R33,34,");
  assert_nom("R33,34,35,36", dRect(), SC_1M, false, "nom_to_range: can't parse name: R33,34,35,36: maps P .. S should be in pairs");

  assert_nom("A01,02",          dRect(), SC_1M, false, "nom_to_range: can't parse name: A01,02: maps A .. O should be single");
  assert_nom("A01,02,03",       dRect(), SC_1M, false, "nom_to_range: can't parse name: A01,02,03");
  assert_nom("A01,02,03,04",    dRect(), SC_1M, false, "nom_to_range: can't parse name: A01,02,03,04: maps A .. O should be single");
  assert_nom("A01,02,03,04,05", dRect(), SC_1M, false, "nom_to_range: can't parse name: A01,02,03,04,05");

  assert_nom("R01",             dRect(), SC_1M, false, "nom_to_range: can't parse name: R01: maps P .. S should be in pairs");
  assert_nom("R01,02,03",       dRect(), SC_1M, false, "nom_to_range: can't parse name: R01,02,03");
  assert_nom("R01,02,03,04",    dRect(), SC_1M, false, "nom_to_range: can't parse name: R01,02,03,04: maps P .. S should be in pairs");
  assert_nom("R01,02,03,04,05", dRect(), SC_1M, false, "nom_to_range: can't parse name: R01,02,03,04,05");

  assert_nom("T01",             dRect(), SC_1M, false, "nom_to_range: can't parse name: T01: maps T .. U should be in quadruples");
  assert_nom("T01,02",          dRect(), SC_1M, false, "nom_to_range: can't parse name: T01,02: maps T .. U should be in quadruples");
  assert_nom("T01,02,03",       dRect(), SC_1M, false, "nom_to_range: can't parse name: T01,02,03");
  assert_nom("T01,02,03,04,05", dRect(), SC_1M, false, "nom_to_range: can't parse name: T01,02,03,04,05");

  assert_nom("R02,03",          dRect(), SC_1M, false, "nom_to_range: can't parse name: R02,03"); // n%2!=1
  assert_nom("T03,04,05,06",    dRect(), SC_1M, false, "nom_to_range: can't parse name: T03,04,05,06");  // n%4!=1

  assert_nom("A001",    dRect(), SC_1M, false, "nom_to_range: can't parse name: A001");
  assert_nom("A1",      dRect(), SC_1M, false, "nom_to_range: can't parse name: A1");
  assert_nom("R01,002", dRect(), SC_1M, false, "nom_to_range: can't parse name: R01,002");
  assert_nom("R001,02", dRect(), SC_1M, false, "nom_to_range: can't parse name: R001,02");

  // good
  assert_nom("a-01",         dRect(-180,  0,  6,4), SC_1M);
  assert_nom("A-01",         dRect(-180,  0,  6,4), SC_1M);
  assert_nom("A01",          dRect(-180,  0,  6,4), SC_1M);
  assert_nom("n-37",         dRect( 36,  52,  6,4), SC_1M);
  assert_nom("o-60",         dRect( 174, 56,  6,4), SC_1M);
  assert_nom("xA-60",        dRect( 174, -4,  6,4), SC_1M);
  assert_nom("xK-18",        dRect( -78,-44,  6,4), SC_1M);
  assert_nom("xK18",         dRect( -78,-44,  6,4), SC_1M);
  assert_nom("T33,34,35,36", dRect(  12, 76, 24,4), SC_1M);
  assert_nom("R33,34",       dRect(  12, 68, 12,4), SC_1M);


  // 1:500'000
  assert_nom("A01-5",   dRect(), SC_500k, false, "nom_to_range: can't parse name: A01-5");
  assert_nom("A01-0",   dRect(), SC_500k, false, "nom_to_range: can't parse name: A01-0");
  assert_nom("R01-1",   dRect(), SC_500k, false, "nom_to_range: can't parse name: R01-1: maps P .. S should be in pairs");
  assert_nom("R01-2,3", dRect(), SC_500k, false, "nom_to_range: can't parse name: R01-2,3");
  assert_nom("T01-1",   dRect(), SC_500k, false, "nom_to_range: can't parse name: T01-1: maps T .. U should be in quadruples");
  assert_nom("T01-1,2", dRect(), SC_500k, false, "nom_to_range: can't parse name: T01-1,2: maps T .. U should be in quadruples");
  assert_nom("R01-1,2,",dRect(), SC_500k, false, "nom_to_range: can't parse name: R01-1,2,: extra symbols after the name");
  assert_nom("R01-1,3", dRect(), SC_500k, false, "nom_to_range: can't parse name: R01-1,3");
  assert_nom("A01-4,1", dRect(), SC_500k, false, "nom_to_range: can't parse name: A01-4,1");
  assert_nom("A01-4 ",  dRect(), SC_500k, false, "nom_to_range: can't parse name: A01-4 : extra symbols after the name");
  assert_nom("A01-4-1", dRect(), SC_500k, false, "nom_to_range: can't parse name: A01-4-1: extra symbols after the name");
  assert_nom("P37-1,2,P38-1,2", dRect(), SC_500k, false, "nom_to_range: can't parse name: P37-1,2,P38-1,2: extra symbols after the name");
  assert_nom("T37-1,2,T38-3,4", dRect(), SC_500k, false, "nom_to_range: can't parse name: T37-1,2,T38-3,4");
  assert_nom("T37-1,2,T37-1,2", dRect(), SC_500k, false, "nom_to_range: can't parse name: T37-1,2,T37-1,2");
  assert_nom("T37-1,2,T39-1,2", dRect(), SC_500k, false, "nom_to_range: can't parse name: T37-1,2,T39-1,2");
  assert_nom("T37-1,2,T39-1,2-1", dRect(), SC_500k, false, "nom_to_range: can't parse name: T37-1,2,T39-1,2-1");

  // good
  assert_nom("A01-1", dRect(-180,2,3,2),  SC_500k);
  assert_nom("A01-2", dRect(-177,2,3,2),  SC_500k);
  assert_nom("A01-3", dRect(-180,0,3,2),  SC_500k);
  assert_nom("A01-4", dRect(-177,0,3,2),  SC_500k);
  assert_nom("P37-1,2", dRect(36,62,6,2), SC_500k);
  assert_nom("T37-1,2,T38-1,2", dRect(36,78,12,2), SC_500k);

  // 1:200'000
  assert_nom("A01-00",    dRect(), SC_200k, false, "nom_to_range: can't parse name: A01-00");
  assert_nom("A01-37",    dRect(), SC_200k, false, "nom_to_range: can't parse name: A01-37");
  assert_nom("A01-01-02", dRect(), SC_200k, false, "nom_to_range: can't parse name: A01-01-02: extra symbols after the name");
  assert_nom("A01-01,02", dRect(), SC_200k, false, "nom_to_range: can't parse name: A01-01,02: maps A .. O should be single");
  assert_nom("A01-01,02,03,04", dRect(), SC_200k, false, "nom_to_range: can't parse name: A01-01,02,03,04: maps A .. O should be single");
  assert_nom("P01-01",    dRect(), SC_200k, false, "nom_to_range: can't parse name: P01-01: maps P .. S should be in pairs");
  assert_nom("P01-01,03", dRect(), SC_200k, false, "nom_to_range: can't parse name: P01-01,03");
  assert_nom("P01-02,03", dRect(), SC_200k, false, "nom_to_range: can't parse name: P01-02,03");
  assert_nom("P01-01,02,03,04", dRect(), SC_200k, false, "nom_to_range: can't parse name: P01-01,02,03,04: maps P .. S should be in pairs");

  assert_nom("T01-01",    dRect(), SC_200k, false, "nom_to_range: can't parse name: T01-01: maps T .. U should be in quadruples");
  assert_nom("T01-01,02", dRect(), SC_200k, false, "nom_to_range: can't parse name: T01-01,02: maps T .. U should be in quadruples");
  assert_nom("T01-02,03", dRect(), SC_200k, false, "nom_to_range: can't parse name: T01-02,03");
  assert_nom("T01-03,04,05,06",  dRect(), SC_200k, false, "nom_to_range: can't parse name: T01-03,04,05,06");
  assert_nom("T01-01,02,03,004", dRect(), SC_200k, false, "nom_to_range: can't parse name: T01-01,02,03,004");
  assert_nom("T01-01,02,03,04,", dRect(), SC_200k, false, "nom_to_range: can't parse name: T01-01,02,03,04,: extra symbols after the name");

  // good
  assert_nom("A01-01",        dRect(-180,  3+1/3., 1, 2/3.), SC_200k);
  assert_nom("O37-01",          dRect(36, 59+1/3., 1, 2/3.), SC_200k);
  assert_nom("P37-01,02",       dRect(36, 63+1/3., 2, 2/3.), SC_200k);
  assert_nom("T37-01,02,03,04", dRect(36, 79+1/3., 4, 2/3.), SC_200k);

  // 1:100'000
  assert_nom("A01-000",             dRect(), SC_100k, false, "nom_to_range: can't parse name: A01-000");
  assert_nom("A01-0145",            dRect(), SC_100k, false, "nom_to_range: can't parse name: A01-0145: extra symbols after the name");
  assert_nom("A01-001-002",         dRect(), SC_100k, false, "nom_to_range: can't parse name: A01-001-002");
  assert_nom("A01-001,002",         dRect(), SC_100k, false, "nom_to_range: can't parse name: A01-001,002: maps A .. O should be single");
  assert_nom("A01-001,002,003,004", dRect(), SC_100k, false, "nom_to_range: can't parse name: A01-001,002,003,004: maps A .. O should be single");
  assert_nom("P01-001",             dRect(), SC_100k, false, "nom_to_range: can't parse name: P01-001: maps P .. S should be in pairs");
  assert_nom("P01-001,003",         dRect(), SC_100k, false, "nom_to_range: can't parse name: P01-001,003");
  assert_nom("P01-002,003",         dRect(), SC_100k, false, "nom_to_range: can't parse name: P01-002,003");
  assert_nom("P01-001,002,003,004", dRect(), SC_100k, false, "nom_to_range: can't parse name: P01-001,002,003,004: maps P .. S should be in pairs");

  assert_nom("T01-001",             dRect(), SC_100k, false, "nom_to_range: can't parse name: T01-001: maps T .. U should be in quadruples");
  assert_nom("T01-001,002",         dRect(), SC_100k, false, "nom_to_range: can't parse name: T01-001,002: maps T .. U should be in quadruples");
  assert_nom("T01-002,003",         dRect(), SC_100k, false, "nom_to_range: can't parse name: T01-002,003");
  assert_nom("T01-003,004,005,006", dRect(), SC_100k, false, "nom_to_range: can't parse name: T01-003,004,005,006");
  assert_nom("T01-001,002,03,004",  dRect(), SC_100k, false, "nom_to_range: can't parse name: T01-001,002,03,004");
  assert_nom("T01-001,002,003,004,",dRect(), SC_100k, false, "nom_to_range: can't parse name: T01-001,002,003,004,: extra symbols after the name");

  // good
  assert_nom("A01-025",             dRect(-180,3,0.5,1.0/3.0), SC_100k);
  assert_nom("O37-025",             dRect(36,59,0.5,1.0/3.0), SC_100k);
  assert_nom("P37-025,026",         dRect(36,63,1,1.0/3.0), SC_100k);
  assert_nom("T37-025,026,027,028", dRect(36,79,2,1.0/3.0), SC_100k);

  // 1:50'000
  assert_nom("A01-001-0",  dRect(), SC_50k, false, "nom_to_range: can't parse name: A01-001-0");
  assert_nom("A01-001-5",  dRect(), SC_50k, false, "nom_to_range: can't parse name: A01-001-5");
  assert_nom("A01-001-01", dRect(), SC_50k, false, "nom_to_range: can't parse name: A01-001-01");
  assert_nom("A01-001-",   dRect(), SC_50k, false, "nom_to_range: can't parse name: A01-001-");
  assert_nom("A01-001-1,2",dRect(), SC_50k, false, "nom_to_range: can't parse name: A01-001-1,2: maps A .. O should be single");
  assert_nom("A01-001-1,2,A01-002-1,2", dRect(), SC_50k, false, "nom_to_range: can't parse name: A01-001-1,2,A01-002-1,2: maps A .. O should be single");

  assert_nom("P01-001-1",   dRect(), SC_50k, false, "nom_to_range: can't parse name: P01-001-1: maps P .. S should be in pairs");
  assert_nom("P01-001-2,3", dRect(), SC_50k, false, "nom_to_range: can't parse name: P01-001-2,3");
  assert_nom("P01-001-0,1", dRect(), SC_50k, false, "nom_to_range: can't parse name: P01-001-0,1");
  assert_nom("P01-001-1,2,P01-002-1,2", dRect(), SC_50k, false, "nom_to_range: can't parse name: P01-001-1,2,P01-002-1,2: extra symbols after the name");

  assert_nom("T01-001-1",   dRect(), SC_50k, false, "nom_to_range: can't parse name: T01-001-1: maps T .. U should be in quadruples");
  assert_nom("T01-001-1,2", dRect(), SC_50k, false, "nom_to_range: can't parse name: T01-001-1,2: maps T .. U should be in quadruples");
  assert_nom("T01-001-1,2,T01-002-1,2,",dRect(), SC_50k, false, "nom_to_range: can't parse name: T01-001-1,2,T01-002-1,2,");
  assert_nom("T01-001-1,2,T01-001-1,2", dRect(), SC_50k, false, "nom_to_range: can't parse name: T01-001-1,2,T01-001-1,2");
  assert_nom("T01-001-1,2,T01-002-3,4", dRect(), SC_50k, false, "nom_to_range: can't parse name: T01-001-1,2,T01-002-3,4");
  assert_nom("T01-001-1,2,T02-002-3,4", dRect(), SC_50k, false, "nom_to_range: can't parse name: T01-001-1,2,T02-002-3,4");
  assert_nom("T01-001-1,2,U01-002-3,4", dRect(), SC_50k, false, "nom_to_range: can't parse name: T01-001-1,2,U01-002-3,4");
  assert_nom("T01-001-1,2,zU01-002-3,4",dRect(), SC_50k, false, "nom_to_range: can't parse name: T01-001-1,2,zU01-002-3,4");

  // good
  assert_nom("A01-025-3",               dRect(-180,3,0.25,1.0/6.0), SC_50k);
  assert_nom("O37-025-3",               dRect(36,59,0.25,1.0/6.0), SC_50k);
  assert_nom("P37-025-3,4",             dRect(36,63,0.5,1.0/6.0), SC_50k);
  assert_nom("T37-025-3,4,T37-026-3,4", dRect(36,79,1,1.0/6.0), SC_50k);

  // extended
  assert_nom("A-01", dRect(-180,0,6,4), SC_1M, 1);
  assert_nom("T-33",  dRect(12,76,6,4), SC_1M, 1);
  assert_nom("R-33",  dRect(12,68,6,4), SC_1M, 1);
  assert_nom("T-34",  dRect(18,76,6,4), SC_1M, 1);
  assert_nom("R-34",  dRect(18,68,6,4), SC_1M, 1);

  assert_nom("A-01.1x1", dRect(-180,0,6,4), SC_1M, 1);
  assert_nom("T-33.1x1",  dRect(12,76,6,4), SC_1M, 1);
  assert_nom("R-33.1x1",  dRect(12,68,6,4), SC_1M, 1);

  assert_nom("A-01.2x3", dRect(-180,0,12,12), SC_1M, 1);
  assert_nom("T-33.2x3",  dRect(12,76,12,12), SC_1M, 1);
  assert_nom("R-33.2x3",  dRect(12,68,12,12), SC_1M, 1);

  assert_nom("T-33 ",     dRect(), SC_1M, 1, "nom_to_range: can't parse name: T-33 : extra symbols after the name");
  assert_nom("R-33.2x2 ", dRect(), SC_1M, 1, "nom_to_range: can't parse name: R-33.2x2 : extra symbols after the name");
  assert_nom("R-33.2x",   dRect(), SC_1M, 1, "nom_to_range: can't parse name: R-33.2x: empty <M>x<N> range");
  assert_nom("R-33.x2",   dRect(), SC_1M, 1, "nom_to_range: can't parse name: R-33.x2: empty <M>x<N> range");
  assert_nom("R-33.2x0",  dRect(), SC_1M, 1, "nom_to_range: can't parse name: R-33.2x0: empty <M>x<N> range");
  assert_nom("R-33.0x2",  dRect(), SC_1M, 1, "nom_to_range: can't parse name: R-33.0x2: empty <M>x<N> range");

  assert_nom("P37-1", dRect(36,62,3,2), SC_500k, 1);
  assert_nom("P37-1.1x1", dRect(36,62,3,2), SC_500k, 1);
  assert_nom("P37-1.2x3", dRect(36,62,6,6), SC_500k, 1);

  assert_nom("T37-01,02,03,04", dRect(36, 79+1/3., 4, 2/3.), SC_200k, 1);
  assert_nom("T37-01.4x1",      dRect(36, 79+1/3., 4, 2/3.), SC_200k, 1);
  assert_nom("T37-01",          dRect(36, 79+1/3., 1, 2/3.), SC_200k, 1);
  assert_nom("T37-01.1x1",      dRect(36, 79+1/3., 1, 2/3.), SC_200k, 1);

  assert_nom("T37-01,02", dRect(), SC_200k, 1, "nom_to_range: can't parse name: T37-01,02: maps T .. U should be in quadruples");
  assert_nom("A37-01,02", dRect(), SC_200k, 1, "nom_to_range: can't parse name: A37-01,02: maps A .. O should be single");

  assert_nom("P37-025,026",     dRect(36,63,1,1.0/3.0), SC_100k, 1);
  assert_nom("P37-025",         dRect(36,63,0.5,1.0/3.0), SC_100k, 1);
  assert_nom("P37-025.1x1",     dRect(36,63,0.5,1.0/3.0), SC_100k, 1);
  assert_nom("P37-025.2x3",     dRect(36,63,1,1), SC_100k, 1);

  assert_nom("P37-025-3,4",             dRect(36,63,0.5,1.0/6.0), SC_50k, 1);
  assert_nom("P37-025-3.2x1",           dRect(36,63,0.5,1.0/6.0), SC_50k, 1);
  assert_nom("P37-025-3",               dRect(36,63,0.25,1.0/6.0), SC_50k, 1);
  assert_nom("T37-025-3,4,T37-026-3,4", dRect(36,79,1,1.0/6.0), SC_50k, 1);
  assert_nom("T37-025-3.4x1",           dRect(36,79,1,1.0/6.0), SC_50k, 1);


  /// nom -> nom
  // 1:1'000'000

  assert_nom1("a01");
  assert_nom1("r33,34");
  assert_nom1("t33,34,35,36");

  assert_nom1("a01-1");
  assert_nom1("p37-1,2");
  assert_nom1("t37-1,2,t38-1,2");
  assert_nom1("t37-3,4,t38-3,4");

  assert_nom1("o37-01");
  assert_nom1("p37-01,02");
  assert_nom1("t37-01,02,03,04");

  assert_nom1("o37-025");
  assert_nom1("p37-025,026");
  assert_nom1("t37-025,026,027,028");

  assert_nom1("o37-025-3");
  assert_nom1("o37-025-4");
  assert_nom1("p37-025-3,4");
  assert_nom1("p37-025-1,2");
  assert_nom1("t37-025-3,4,t37-026-3,4");
  assert_nom1("t37-025-1,2,t37-026-1,2");

  assert_nom1("a01", 1);
  assert_nom1("r34", 1);
  assert_nom1("t36", 1);

  assert_nom1("a01-1", 1);
  assert_nom1("p37-2", 1);
  assert_nom1("t37-2", 1);
  assert_nom1("t37-4", 1);

  assert_nom1("o37-01", 1);
  assert_nom1("p37-02", 1);
  assert_nom1("t37-02", 1);

  assert_nom1("o37-025", 1);
  assert_nom1("p37-026", 1);
  assert_nom1("t37-027", 1);

  assert_nom1("o37-025-3", 1);
  assert_nom1("o37-025-4", 1);
  assert_nom1("p37-025-3", 1);
  assert_nom1("p37-025-1", 1);
  assert_nom1("t37-025-3", 1);
  assert_nom1("t37-025-1", 1);

  assert_eq(pt_to_nom(dPoint(0,0), SC_100k), "a31-133");




  assert_eq(nom_shift("j42-030", iPoint(2,0)), "j42-032");
  assert_eq(nom_shift("j42-030", iPoint(0,3)), "k42-138");
  assert_eq(nom_shift("j42-030", iPoint(-1,-1)), "j42-041");

  assert_eq(nom_shift("p41-3,4", iPoint(2,0)), "p43-3,4");
  assert_eq(nom_shift("p41-3,4", iPoint(0,-1)), "o41-1");
  assert_eq(nom_shift("p41-3", iPoint(0,-1), 1), "o41-1");
  assert_eq(nom_shift("p41-4", iPoint(0,-1), 1), "o41-2");

  assert_eq(nom_shift("t41,42,43,44", iPoint(0,-5), 1), "o41");
  assert_eq(nom_shift("t41,42,43,44", iPoint(-1,-5), 1), "o37");
  assert_eq(nom_shift("t42", iPoint(0,-5), 1), "o42");

// std::cerr << nom_to_range("n37-001", sc).tlc() << "\n";
// std::cerr << pt_to_nom(nom_to_range("n37-001", sc).tlc(), sc) << "\n";

//  assert_eq(pt_to_nom(nom_to_range("n37-013", sc).tlc(), sc), "n37-013");

  std::set<std::string> names;

  names = range_to_nomlist(nom_to_range("j42-012", sc, 1), SC_100k);
  assert_eq(names.size(), 1);
  assert_eq(names.count("j42-012"),1);

  names = range_to_nomlist(nom_to_range("j42-012.2x3", sc, 1), SC_100k);
  assert_eq(names.size(), 6);
  assert_eq(names.count("j42-012"),1);
  assert_eq(names.count("j43-001"),1);
  assert_eq(names.count("k42-132"),1);
  assert_eq(names.count("k42-144"),1);
  assert_eq(names.count("k43-121"),1);
  assert_eq(names.count("k43-133"),1);

  names = range_to_nomlist(dRect(71.80,39.9, 0.4,0.2), SC_100k);
  assert_eq(names.size(), 4);
  assert_eq(names.count("j42-012"),1);
  assert_eq(names.count("j43-001"),1);
  assert_eq(names.count("k42-144"),1);
  assert_eq(names.count("k43-133"),1);

  names = range_to_nomlist(dRect(64,58, 10,20), SC_1M);
//  for (auto const & i:names) std::cerr << i << "\n";
  assert_eq(names.size(), 12);
  assert_eq(names.count("o41"),1);
  assert_eq(names.count("o42"),1);
  assert_eq(names.count("o43"),1);
  assert_eq(names.count("p41,42"),1);
  assert_eq(names.count("p43,44"),1);
  assert_eq(names.count("q41,42"),1);
  assert_eq(names.count("q43,44"),1);
  assert_eq(names.count("r41,42"),1);
  assert_eq(names.count("r43,44"),1);
  assert_eq(names.count("s41,42"),1);
  assert_eq(names.count("s43,44"),1);
  assert_eq(names.count("t41,42,43,44"),1);

  // same but with single=true
  names = range_to_nomlist(dRect(64,58, 10,20), SC_1M, true);
//  for (auto const & i:names) std::cerr << i << "\n";
  assert_eq(names.size(), 18);
  assert_eq(names.count("o41"),1);
  assert_eq(names.count("o42"),1);
  assert_eq(names.count("o43"),1);
  assert_eq(names.count("p41"),1);
  assert_eq(names.count("p42"),1);
  assert_eq(names.count("p43"),1);
  assert_eq(names.count("q41"),1);
  assert_eq(names.count("q42"),1);
  assert_eq(names.count("q43"),1);
  assert_eq(names.count("r41"),1);
  assert_eq(names.count("r42"),1);
  assert_eq(names.count("r43"),1);
  assert_eq(names.count("s41"),1);
  assert_eq(names.count("s42"),1);
  assert_eq(names.count("s43"),1);
  assert_eq(names.count("t41"),1);
  assert_eq(names.count("t42"),1);
  assert_eq(names.count("t43"),1);

  // same but opposit sign
  names = range_to_nomlist(dRect(-74,-78, 10,20), SC_1M);
//  for (auto const & i:names) std::cerr << i << "\n";
  assert_eq(names.size(), 12);
  assert_eq(names.count("xo18"),1);
  assert_eq(names.count("xo19"),1);
  assert_eq(names.count("xo20"),1);
  assert_eq(names.count("xp17,18"),1);
  assert_eq(names.count("xp19,20"),1);
  assert_eq(names.count("xq17,18"),1);
  assert_eq(names.count("xq19,20"),1);
  assert_eq(names.count("xr17,18"),1);
  assert_eq(names.count("xr19,20"),1);
  assert_eq(names.count("xs17,18"),1);
  assert_eq(names.count("xs19,20"),1);
  assert_eq(names.count("xt17,18,19,20"),1);


  }
  catch (Err E){
    std::cerr << "Error: " << E.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
