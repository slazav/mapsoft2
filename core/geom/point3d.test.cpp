///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "point.h"
#include "opt/opt.h"

int
main(){
  try{

  {
    // constructors
    iPoint p1, p2(1,1,3), p3(-1,-2, -5);
    assert(p1==iPoint(0,0,0));

    // =, !=
    assert(iPoint(0,0,0)==iPoint(0,0,0));
    assert(iPoint(0,0,0)!=iPoint(1,0,0));
    assert(iPoint(0,0,1)!=iPoint(0,0,0));

    // swap
    p1.swap(p2);
    assert(p1==iPoint(1,1,3));

    assert(iPoint("[3,4]")==iPoint(3,4,0));
    assert(iPoint("[3,4,2]")==iPoint(3,4,2));
  }

  {
    // +, -, *, /
    iPoint p1(1,1,2);

    assert((p1+iPoint(2,3,1)) == iPoint(3,4,3));
    assert((p1-iPoint(2,1,4)) == iPoint(-1,0,-2));
    assert((p1*8) == iPoint(8,8,16));
    assert((p1/2) == iPoint(0,0,1));

    assert((p1+=iPoint(2,3,1)) == iPoint(3,4,3));
    assert((p1-=iPoint(2,1,4)) == iPoint(1,3,-1));
    assert((p1*=8) == iPoint(8,24,-8));
    assert((p1/=3) == iPoint(2,8,-2));

    assert(dPoint(1,2,3) + dPoint(2,3,4) == dPoint(3,5,7));
    assert(dPoint(3,4,5) - dPoint(1,-1,2) == dPoint(2,5,3));
    assert(dPoint(3,4,1)*2 == dPoint(6,8,2));
    assert(2.0*dPoint(3,4,1) == dPoint(6,8,2));
    assert(dPoint(3,4,6)/2 == dPoint(1.5,2,3));

    assert(-iPoint(1,2,5) == iPoint(-1,-2,-5));

  }

  {
    // < > =
    assert(iPoint(1,2,3) < iPoint(1,3,3));
    assert(iPoint(1,2,3) <= iPoint(1,3,3));
    assert(iPoint(1,2,3) <= iPoint(1,2,4));
    assert(iPoint(1,2,3) <= iPoint(1,2,3));
    assert(iPoint(2,1,3) >= iPoint(1,2,3));
    assert(iPoint(2,1,3) >= iPoint(2,1,2));
    assert(iPoint(2,1,3) >= iPoint(2,1,3));
    assert(iPoint(2,1,3) > iPoint(2,0,5));

    // dPoint -> iPoint
    assert(iPoint(2,1,3) == (iPoint)(dPoint(2,1,3)));
    assert(iPoint(2,1,3) == (iPoint)(dPoint(2.8,1.2,3.9)));
  }

  {
    // mlen/len/norm
    iPoint p1(1,2,2);
    iPoint p2(10,0,0);
    assert(p1.mlen() == 5);
    assert(mlen(p1) == 5);
    assert(p1.len() == 3);
    assert(len(p1) == 3);

    assert(p2.norm() == dPoint(1,0));
    assert(norm(p2) == dPoint(1,0));

    assert(norm(iPoint(0,0,1.23)) == dPoint(0,0,1));
  }

  try {
    norm(dPoint(0,0,0));
    assert(false);
  }
  catch (Err e){
    assert(e.str() == "Point norm: zero length");
  }

  // rint/floor/ceil/abs
  {
    dPoint dp1(1.1,2.8,3.8), dp2(-0.1,-3.9,-4.6);

    assert(iPoint(rint(dp1)) == iPoint(1,3,4));
    assert(iPoint(dp1.rint()) == iPoint(1,3,4));
    assert(iPoint(rint(dp2)) == iPoint(0,-4,-5));
    assert(iPoint(dp2.rint()) == iPoint(0,-4,-5));

    assert(iPoint(floor(dp1)) == iPoint(1,2,3));
    assert(iPoint(dp1.floor()) == iPoint(1,2,3));
    assert(iPoint(floor(dp2)) == iPoint(-1,-4,-5));
    assert(iPoint(dp2.floor()) == iPoint(-1,-4,-5));
    assert(iPoint(ceil(dp2)) == iPoint(0,-3,-4));
    assert(iPoint(dp2.ceil()) == iPoint(0,-3,-4));

    assert(abs(dp1) == dp1);
    assert(dp1.abs() == dp1);
    assert(abs(dp2) == -dp2);
    assert(dp2.abs() == -dp2);
  }
  // rotate
  {
    iPoint p(1000,0);
    double a=30*M_PI/180.0;
    assert(p.rotate2d(iPoint(0,0,10),a) == iPoint(866,-499,10)); // sqrt(3)/2, -1/2
    assert(p.rotate2d(iPoint(500,500,1), a) == iPoint(683,-183,1));
    assert(rotate2d(p,iPoint(500,500,1), a) == iPoint(683,-183,1));

    iPoint p1(1,2,3);
    dPoint p2(1.1,2.2,3.3);
    assert(flatten(p1) == iPoint(1,2));
    assert(flatten(p2) == dPoint(1.1,2.2));

    assert(p1.flatten() == iPoint(1,2));
    assert(p2.flatten() == dPoint(1.1,2.2));
    assert(flatten(p) == p);

  }

  // pscal, dist
  {
    iPoint p1(1,2,2);
    iPoint p2(10,0,1);
    assert(pscal(p1,p2) == 12);
    assert(pscal(p1,p1) == 9);
    assert(dist(p1,p1) == 0);
    assert(dist(p1,iPoint(0,0)) == 3);
  }

  // input/output (also check that dPoint is printed with setprecision(9))
  {
    assert(type_to_str(dPoint(0,0,1)) == "[0,0,1]");
    assert(type_to_str(iPoint(1,3,2)) == "[1,3,2]");
    assert(type_to_str(iPoint(1000000000,-1000000000,-2000000000)) == "[1000000000,-1000000000,-2000000000]");
    assert(type_to_str(dPoint(1.23,3.45,2.22)) == "[1.23,3.45,2.22]");
    assert(type_to_str(dPoint(-1.23,-3.45,-2.22)) == "[-1.23,-3.45,-2.22]");
    assert(type_to_str(dPoint(-1.23e1,-3.45e-1,-1.23e-1)) == "[-12.3,-0.345,-0.123]");
    assert(type_to_str(dPoint(-1.23e5,-3.45e-5,-3.45e-5)) == "[-123000,-3.45e-05,-3.45e-05]");
    assert(type_to_str(dPoint(-1.23e8,-3.45e-8,-3.45e-8)) == "[-123000000,-3.45e-08,-3.45e-08]");
    assert(type_to_str(dPoint(-1.23e15,-3.45e-12,-3.45e-12)) == "[-1.23e+15,-3.45e-12,-3.45e-12]");
    assert(type_to_str(dPoint(8000000,9000000,7000000)) == "[8000000,9000000,7000000]");
    assert(type_to_str(dPoint(80000000,90000000,70000000)) == "[80000000,90000000,70000000]");
    assert(type_to_str(dPoint(800000001,900000001,700000001)) == "[800000001,900000001,700000001]");
    assert(type_to_str(dPoint(8000000001,9000000001,7000000001)) == "[8e+09,9e+09,7e+09]");

    assert(str_to_type<dPoint>("[0,0,0]") == dPoint(0,0));
    assert(str_to_type<dPoint>("[0,0,1]") == dPoint(0,0,1));
    assert(str_to_type<iPoint>("[1,3,2]") == iPoint(1,3,2));
    assert(str_to_type<iPoint>("[1000000000,-1000000000,-1000000000]") == iPoint(1000000000,-1000000000,-1000000000));
    assert(str_to_type<dPoint>("[1.23,3.45,3.45]") == dPoint(1.23,3.45,3.45));
    assert(str_to_type<dPoint>("[-1.23,-3.45,-3.45]") == dPoint(-1.23,-3.45,-3.45));
    assert(str_to_type<dPoint>("[-1.23e1,-3.45e-1,-3.45e-1]") == dPoint(-1.23e1,-3.45e-1,-3.45e-1));
    assert(str_to_type<dPoint>("[-1.23e5,-3.45e-5,-3.45e-5]") == dPoint(-1.23e5,-3.45e-5,-3.45e-5));
    assert(str_to_type<dPoint>("[-1.23e8,-3.45e-8,-3.45e-8]") == dPoint(-1.23e8,-3.45e-8,-3.45e-8));
    assert(str_to_type<dPoint>("[-1.23e15,-3.45e-12,-3.45e-12]") == dPoint(-1.23e15,-3.45e-12,-3.45e-12));
    assert(str_to_type<dPoint>("[8000000,9000000,9000000]") == dPoint(8000000,9000000,9000000));
    assert(str_to_type<dPoint>("[80000000,90000000,90000000]") == dPoint(80000000,90000000,90000000));
    assert(str_to_type<dPoint>("[800000001,900000001,900000001]") == dPoint(800000001,900000001,900000001));

    assert(str_to_type<dPoint>(" [ 0 , 1 , 2 ] ") == dPoint(0,1,2));

    try { str_to_type<dPoint>(" [ 0 , 1 ,1"); }
    catch (Err e) { assert(e.str() == "can't parse value:  [ 0 , 1 ,1"); }

    try { str_to_type<dPoint>("0,1,1"); }
    catch (Err e) { assert(e.str() == "can't parse value: 0,1,1"); }

    try { str_to_type<dPoint>("[0.1]"); }
    catch (Err e) { assert(e.str() == "can't parse value: [0.1]"); }

    try { str_to_type<dPoint>("[0,1,1]a"); }
    catch (Err e) { assert(e.str() == "can't parse value: [0,1,1]a"); }
  }

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}
///\endcond
