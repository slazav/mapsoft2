///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "fig_data.h"

int
main(){
  try {

    // static members
    assert(Fig::colors.size() == 33);
    assert(Fig::colors.find(-1)->second == 0);
    assert(Fig::colors.find(0)->second == 0);
    assert(Fig::colors.find(1)->second == 0xff0000);
    assert(Fig::colors.find(10)->second == 0xd00000);
    assert(Fig::colors.find(31)->second == 0x00d7ff);

    assert(Fig::psfonts.size() == 36);
    assert(Fig::psfonts.find(-1)->second == "Default");
    assert(Fig::psfonts.find(0)->second  == "Times-Roman");
    assert(Fig::psfonts.find(34)->second == "ZapfDingbats");

    assert(Fig::texfonts.size() == 6);
    assert(Fig::texfonts.find(0)->second == "Default");
    assert(Fig::texfonts.find(1)->second  == "Roman");
    assert(Fig::texfonts.find(5)->second == "Typewriter");

    assert(Fig::cm2fig == 1200.0 / 1.05 / 2.54);
    assert(Fig::fig2cm == 1.0 / Fig::cm2fig);

    // constructing objects from a header string -- ellipse
    {
      FigObj o("1 3 0 2 0 7 50 -1 -1 0.000 1 0.0000 7380 5310 664 664 7380 5310 7695 4725");
      assert(o.is_ellipse());
      assert(o.is_closed());
      assert(o.size()==1); // center
      assert(o[0].x==7380);
      assert(o[0].y==5310);

      assert(o.sub_type==3);
      assert(o.line_style==0);
      assert(o.thickness==2);
      assert(o.pen_color==0);
      assert(o.fill_color==7);
      assert(o.depth==50);
      assert(o.pen_style==-1);
      assert(o.area_fill==-1);
      assert(o.style_val==0);
      assert(o.direction==1);
      assert(o.angle==0);
      assert(o.radius_x==664);
      assert(o.radius_y==664);
      assert(o.start_x==7380);
      assert(o.start_y==5310);
      assert(o.end_x==7695);
      assert(o.end_y==4725);
    }

    {
      FigObj o("1 2 0 1 0 7 50 -1 -1 0.000 1 0.2618 9877 3375 1260 405 8765 4092 10990 2658");
      assert(o.is_ellipse());
      assert(o.is_closed());
      assert(o.size()==1); // center
      assert(o[0].x==9877);
      assert(o[0].y==3375);

      assert(o.sub_type==2);
      assert(o.line_style==0);
      assert(o.thickness==1);
      assert(o.pen_color==0);
      assert(o.fill_color==7);
      assert(o.depth==50);
      assert(o.pen_style==-1);
      assert(o.area_fill==-1);
      assert(o.style_val==0);
      assert(o.direction==1);
      assert(abs(o.angle-0.2618)<1e-6);
      assert(o.radius_x==1260);
      assert(o.radius_y==405);
      assert(o.start_x==8765);
      assert(o.start_y==4092);
      assert(o.end_x==10990);
      assert(o.end_y==2658);
    }
    try {
      FigObj o("1 2 0 1 0 7 50 -1 -1 0.000 1 0.2618 9877 3375 1260 405 8765 4092 10990");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read ellipse object: "
         "[1 2 0 1 0 7 50 -1 -1 0.000 1 0.2618 9877 3375 1260 405 8765 4092 10990]");
    }

    try {
      FigObj o("1 2 0 1 0 7 50 -1 -1 0.000 1 0.2618 9877 3375 1260 x 8765 4092 10990 2658");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read ellipse object: "
         "[1 2 0 1 0 7 50 -1 -1 0.000 1 0.2618 9877 3375 1260 x 8765 4092 10990 2658]");
    }

    try {
      FigObj o("1 2 0 1 0 7 50 -1 -1 0.000 1 0.2618 9877 3375 1260 405 8765 4092 10990 2658 1");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read ellipse object: "
         "[1 2 0 1 0 7 50 -1 -1 0.000 1 0.2618 9877 3375 1260 405 8765 4092 10990 2658 1]");
    }

    // constructing objects from a header string -- lines
    {
      FigObj o("2 1 0 1 0 7 50 -1 -1 0.000 0 0 -1 0 0 4");
      assert(o.is_polyline());
      assert(!o.is_closed());
      assert(o.size()==4);

      assert(o.sub_type==1);
      assert(o.line_style==0);
      assert(o.thickness==1);
      assert(o.pen_color==0);
      assert(o.fill_color==7);
      assert(o.depth==50);
      assert(o.pen_style==-1);
      assert(o.area_fill==-1);
      assert(o.style_val==0);
      assert(o.join_style==0);
      assert(o.cap_style==0);
      assert(o.radius==-1);
      assert(o.forward_arrow==0);
      assert(o.backward_arrow==0);
    }

    try {
      FigObj o("2 1 0 1 0 7 50 -1 -1 0.000 0 0 -1 0 0");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read line object: "
         "[2 1 0 1 0 7 50 -1 -1 0.000 0 0 -1 0 0]");
    }

    try {
      FigObj o("2 1 0.1 1 0 7 50 -1 -1 0.000 0 0 -1 0 0 4");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read line object: "
         "[2 1 0.1 1 0 7 50 -1 -1 0.000 0 0 -1 0 0 4]");
    }

    try {
      FigObj o("2 1 0 1 0 7 50 -1 -1 0.000 0 0 -1 0 0 4 1");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read line object: "
         "[2 1 0 1 0 7 50 -1 -1 0.000 0 0 -1 0 0 4 1]");
    }

    // constructing objects from a header string -- splines
    {
      FigObj o("3 0 0 1 0 7 50 -1 -1 0.000 0 0 0 4");
      assert(o.is_spline());
      assert(!o.is_closed());
      assert(o.size()==4);

      assert(o.sub_type==0);
      assert(o.line_style==0);
      assert(o.thickness==1);
      assert(o.pen_color==0);
      assert(o.fill_color==7);
      assert(o.depth==50);
      assert(o.pen_style==-1);
      assert(o.area_fill==-1);
      assert(o.style_val==0);
      assert(o.cap_style==0);
      assert(o.forward_arrow==0);
      assert(o.backward_arrow==0);
    }

    try {
      FigObj o("3 0 0 1 0 7 50 -1 -1 0.000 0 0 0");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read spline object: "
         "[3 0 0 1 0 7 50 -1 -1 0.000 0 0 0]");
    }

    try {
      FigObj o("3 0 0 1 0 7 50 -1 -1 0.000 a 0 0 4");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read spline object: "
         "[3 0 0 1 0 7 50 -1 -1 0.000 a 0 0 4]");
    }

    try {
      FigObj o("3 0 0 1 0 7 50 -1 -1 0.000 0 0 0 4 1");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read spline object: "
         "[3 0 0 1 0 7 50 -1 -1 0.000 0 0 0 4 1]");
    }

    // constructing objects from a header string -- text
    {
      FigObj o("4 0 0 50 -1 0 12 0.0000 4 210 300 -1575 6840 text\\001");
      assert(o.is_text());
      assert(!o.is_closed());
      assert(o.size()==1);
      assert(o[0].x==-1575);
      assert(o[0].y==6840);

      assert(o.sub_type==0);
      assert(o.pen_color==0);
      assert(o.depth==50);
      assert(o.pen_style==-1);
      assert(o.font==0);
      assert(o.font_size==12);
      assert(o.angle==0.0);
      assert(o.font_flags==4);
      assert(o.height==210);
      assert(o.length==300);
      assert(o.text=="text");

    }

    try {
      FigObj o("4 0 0 50 -1 0 12 0.0000 4 210 z -1575 6840 text\\001");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read text object: "
         "[4 0 0 50 -1 0 12 0.0000 4 210 z -1575 6840 text\\001]");
    }

    try {
      FigObj o("4 0 0 50 -1 0 12 0.0000 4 210 300 -1575 6840");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read text object: "
         "[4 0 0 50 -1 0 12 0.0000 4 210 300 -1575 6840]");
    }

    {
      FigObj o;
      o = FigObj("4 0 0 50 -1 0 12 0.0000 4 210 300 -1575 6840 text\\001");
      assert(o.text == "text");

      o = FigObj("4 0 0 50 -1 0 12 0.0000 4 210 300 -1575 6840 te\\xt\\001");
      assert(o.text == "text");

      o = FigObj("4 0 0 50 -1 0 12 0.0000 4 210 300 -1575 6840 te\\170t\\001");
      assert(o.text == "text");

      o = FigObj("4 0 0 50 -1 0 12 0.0000 4 210 300 -1575 6840 te\\170t\\001aaa");
      assert(o.text == "text");
      assert(o.multiline == false);

      o = FigObj("4 0 0 50 -1 0 12 0.0000 4 210 300 -1575 6840 text");
      assert(o.text == "text");
      assert(o.multiline == true);
    }

    // constructing objects from a header string -- arc
    {
      FigObj o("5 1 0 1 0 7 50 -1 -1 0.000 0 0 0 0 3915.000 7477.500 3420 7380 3870 6975 4410 7380\n");
      assert(o.is_arc());
      assert(!o.is_closed());
      assert(o.size()==3);
      assert(o[0].x==3420);
      assert(o[0].y==7380);
      assert(o[1].x==3870);
      assert(o[1].y==6975);
      assert(o[2].x==4410);
      assert(o[2].y==7380);

      assert(o.sub_type==1);
      assert(o.line_style==0);
      assert(o.thickness==1);
      assert(o.pen_color==0);
      assert(o.fill_color==7);
      assert(o.depth==50);
      assert(o.pen_style==-1);
      assert(o.area_fill==-1);
      assert(o.style_val==0);
      assert(o.cap_style==0);
      assert(o.direction==0);
      assert(o.forward_arrow==0);
      assert(o.backward_arrow==0);
      assert(o.center_x==3915.000);
      assert(o.center_y==7477.500);
    }

    try {
      FigObj o("5 1 0 1 0 7 50 -1 -1 0.000 0 0 0 0 3915.000 7477.500 3420 7380 3870 6975 4410 x");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read arc object: "
         "[5 1 0 1 0 7 50 -1 -1 0.000 0 0 0 0 3915.000 7477.500 3420 7380 3870 6975 4410 x]");
    }

    try {
      FigObj o("5 1 0 1 0 7 50 -1 -1 0.000 0 0 0 0 3915.000 7477.500 3420 7380 3870 6975 4410 7380 1");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read arc object: "
         "[5 1 0 1 0 7 50 -1 -1 0.000 0 0 0 0 3915.000 7477.500 3420 7380 3870 6975 4410 7380 1]");
    }

    try {
      FigObj o("5 1 0 1 0 7 50 -1 -1 0.000 0 0 0 0 3915.000 7477.500 3420 7380 3870 6975 4410");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read arc object: "
         "[5 1 0 1 0 7 50 -1 -1 0.000 0 0 0 0 3915.000 7477.500 3420 7380 3870 6975 4410]");
    }


    // constructing objects from a header string -- compound
    {
      FigObj o("6");
      assert(o.is_compound());
      assert(!o.is_closed());
      assert(o.size()==2);
      assert(o[0].x==0);
      assert(o[0].y==0);
      assert(o[1].x==0);
      assert(o[1].y==0);
    }

    {
      FigObj o("6 1 2 3 4");
      assert(o.is_compound());
      assert(!o.is_closed());
      assert(o.size()==2);
      assert(o[0].x==1);
      assert(o[0].y==2);
      assert(o[1].x==3);
      assert(o[1].y==4);
    }

    try {
      FigObj o("6 1 2 3");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read compound object: "
         "[6 1 2 3]");
    }

    try {
      FigObj o("6 1 2 3 4 1");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read compound object: "
         "[6 1 2 3 4 1]");
    }

    try {
      FigObj o("6 1 2 3.2 4");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read compound object: "
         "[6 1 2 3.2 4]");
    }

    {
      FigObj o("-6");
      assert(o.is_compound_end());
      assert(!o.is_closed());
      assert(o.size()==0);
    }

    try {
      FigObj o("-6 1");
      assert(false);
    }
    catch (Err e) {
      assert(e.str() == "FigObj: can't read compound end object: "
         "[-6 1]");
    }


  }
  catch (Err E){
    std::cerr << "Error: " << E.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond