///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "cairo_wrapper.h"

int
main(){
  try{

    {
      CairoWrapper cw;
      Image img(150,100, IMAGE_32ARGB);
      cw.set_surface_img(img);

      assert(cw.width() == 150);
      assert(cw.height() == 100);
      assert(cw.bbox() == dRect(0,0,150,100));
      cw->set_color_a(0xF0FF0000); // red color
      cw->cap_round(); // 1-point line is only visible with round or butt cap
      cw->mkpath(dLine("[[80,50]]"));
      cw->set_line_width(3);
      cw->stroke();
      assert(img.get32(80,50) == 0xF0F00000); // scaled color!
      assert(img.get32(80,60) == 0x00000000);

    }

    {
      CairoWrapper cw;
      cw.set_surface_pdf("tmp1.pdf", 150,100);

      assert(cw.width() == 150);
      assert(cw.height() == 100);
      assert(cw.bbox() == dRect(0,0,150,100));
      cw->set_color(0xFF0000); // red color
      cw->mkpath(dLine("[[10,50],[130,10],[80,90]]"), 0);
      cw->fill_preserve();
      cw->set_color_a(0x8F000000);
      cw->set_line_width(3);
      cw->stroke();

      cw->cap_round(); // 1-point line is only visible with round or butt cap
      cw->mkpath(dLine("[[80,50]]"));
      cw->set_line_width(10);
      cw->stroke();

      //closed polygon
      cw->mkpath(dLine("[[10,10],[10,20],[20,20],[20,10]]"), 1);
      cw->set_line_width(2);
      cw->set_color_a(0xFF000000);
      cw->stroke_preserve();
      cw->set_color(0xFF00FF);
      cw->fill();
//      cw->save_png("tmp1.png");
    }
    {
      CairoWrapper cw;
      cw.set_surface_pdf("tmp2.pdf", 150,100);
      assert(cw.width() == 150);
      assert(cw.height() == 100);
      assert(cw.bbox() == dRect(0,0,150,100));

      cw->set_color(0xFFFF00);
      cw->paint();
      cw->set_color(0x000000);

      // render text using fig fonts
      cw->set_fig_font(0xFF0000FF, 0, 10, 150);
      cw->text("Test/Тест", dPoint(0,0), 0, 0,2);

      cw->set_fig_font(0xFF0000FF, 1, 10, 150);
      cw->text("Test/Тест", dPoint(0,20), 0.1, 0,2);

      cw->set_fig_font(0xFF0000FF, 16, 10, 150);
      cw->text("Test/Тест", dPoint(0,40), 0.1, 0,2);

      cw->set_fig_font(0xFF0000FF, 18, 10, 150);
      cw->text("Test/Тест", dPoint(150,100), 0, 2,0);

//      cw->save_png("tmp2.png");
    }

    {
      CairoWrapper cw;
      cw.set_surface_pdf("tmp3.pdf", 150,200);
      assert(cw.width() == 150);
      assert(cw.height() == 200);
      assert(cw.bbox() == dRect(0,0,150,200));

      cw->set_color(0xFFFFFF);
      cw->paint();

      const char *fonts_it[] = {
        "DejaVu Serif:Italic:semicondensed:rgba=none",
        "Free Serif:Italic:semicondensed:rgba=none",
        "Liberation Serif:Italic:semicondensed:rgba=none",
        "Nimbus Roman No9 L:Italic:semicondensed:rgba=none",
        "URW Bookman L:Italic:semicondensed:rgba=none",
        "URW Palladio L:Italic:semicondensed:rgba=none",
        "Century Schoolbook L:Italic:semicondensed:rgba=none",
        NULL};

      const char *fonts_bf[] = {
         "DejaVu Sans:Bold:semicondensed:rgba=none",
         "FreeSans:semibold:semicondensed:rgba=none",
         "Liberation Sans:bold:rgba=none:autohint",
         "Nimbus Sans L:bold:rgba=none:autohint",
         "URW Gothic L:semibold:rgba=none:autohint",
         "Open Sans:bold:rgba=none:autohint",
         "Open Sans:semibold:rgba=none:autohint",
        NULL};

      const char *text = "Дод-Уха-Хем-Голын-Барун-Сала";

      // render text using fontconfig
      for (int i=0; fonts_it[i]; ++i) {
        double fs = (i==1||i==2||i==5)? 8.5:8;
        cw->set_fc_font(0xFF0000FF, fonts_it[i], fs);
        cw->text(text, dPoint(10,20+10*i),-0.1);
      }

      for (int i=0; fonts_bf[i]; ++i) {
        double fs = 8;
        cw->set_fc_font(0xFF000000, fonts_bf[i], fs);
        cw->text(text, dPoint(10,100+10*i),0);
      }

//      cw->save_png("tmp3.png");
    }

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  } 
  return 0;
}

///\endcond

