///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "cairo_wrapper.h"

int
main(){
  try{
    {
      CairoWrapper cw;
      cw.set_surface_pdf("tmp1.pdf", 150,100);

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

      cw->save_png("tmp1.png");
    }
    {
      CairoWrapper cw;
      cw.set_surface_pdf("tmp2.pdf", 150,100);
      cw->set_color(0xFFFF00);
      cw->paint();
      cw->set_color(0x000000);

      // render text using fig fonts
      cw->render_text_fig("Test/Тест", dPoint(0,0), 0, 0xFF0000FF, 0, 10, 150, 0, 2);
      cw->render_text_fig("Test/Тест", dPoint(0,20), 0.1, 0xFF0000FF, 1, 10, 150, 0, 2);
      cw->render_text_fig("Test/Тест", dPoint(0,40), 0.1, 0xFF0000FF, 16, 10, 150, 0, 2);
      cw->render_text_fig("Test/Тест", dPoint(150,100),0 , 0xFF0000FF, 18, 10, 150, 2, 0);
      cw->save_png("tmp2.png");
    }

    {
      CairoWrapper cw;
      cw.set_surface_pdf("tmp3.pdf", 150,200);
      cw->set_color(0xFFFFFF);
      cw->paint();

      // render text using fontconfig
      cw->render_text_fc("Дод-Уха-Хем-Голын-Барун-Сала", dPoint(10,20),-0.1, 0xFF0000FF,
         "DejaVu Serif:Italic:semicondensed:rgba=none", 8, 0, 0);
      cw->render_text_fc("Дод-Уха-Хем-Голын-Барун-Сала", dPoint(10,30),-0.1, 0xFF0000FF,
         "Free Serif:Italic:semicondensed:rgba=none", 8.5, 0, 0);
      cw->render_text_fc("Дод-Уха-Хем-Голын-Барун-Сала", dPoint(10,40),-0.1, 0xFF0000FF,
         "Liberation Serif:Italic:semicondensed:rgba=none", 8.5, 0, 0);
      cw->render_text_fc("Дод-Уха-Хем-Голын-Барун-Сала", dPoint(10,50),-0.1, 0xFF0000FF,
         "Nimbus Roman No9 L:Italic:semicondensed:rgba=none", 8, 0, 0);
      cw->render_text_fc("Дод-Уха-Хем-Голын-Барун-Сала", dPoint(10,60),-0.1, 0xFF0000FF,
         "URW Bookman L:Italic:semicondensed:rgba=none", 8, 0, 0);
      cw->render_text_fc("Дод-Уха-Хем-Голын-Барун-Сала", dPoint(10,70),-0.1, 0xFF0000FF,
         "URW Palladio L:Italic:semicondensed:rgba=none", 8.5, 0, 0);
      cw->render_text_fc("Дод-Уха-Хем-Голын-Барун-Сала", dPoint(10,80),-0.1, 0xFF0000FF,
         "Century Schoolbook L:Italic:semicondensed:rgba=none", 8, 0, 0);

      cw->render_text_fc("Дод-Уха-Хем-Голын-Барун-Сала", dPoint(10,100),0, 0xFF000000,
         "DejaVu Sans:Bold:semicondensed:rgba=none", 8, 0, 0);
      cw->render_text_fc("Дод-Уха-Хем-Голын-Барун-Сала", dPoint(10,110),0, 0xFF000000,
         "FreeSans:semibold:semicondensed:rgba=none", 8, 0, 0);
      cw->render_text_fc("Дод-Уха-Хем-Голын-Барун-Сала", dPoint(10,120),0, 0xFF000000,
         "Liberation Sans:bold:rgba=none:autohint", 8, 0, 0); //+
      cw->render_text_fc("Дод-Уха-Хем-Голын-Барун-Сала", dPoint(10,130),0, 0xFF000000,
         "Nimbus Sans L:bold:rgba=none:autohint", 8, 0, 0);
      cw->render_text_fc("Дод-Уха-Хем-Голын-Барун-Сала", dPoint(10,140),0, 0xFF000000,
         "URW Gothic L:semibold:rgba=none:autohint", 8, 0, 0);
      cw->render_text_fc("Дод-Уха-Хем-Голын-Барун-Сала", dPoint(10,150),0, 0xFF000000,
         "Open Sans:bold:rgba=none:autohint", 8, 0, 0);
      cw->render_text_fc("Дод-Уха-Хем-Голын-Барун-Сала", dPoint(10,160),0, 0xFF000000,
         "Open Sans:semibold:rgba=none:autohint", 8, 0, 0); //+


      cw->save_png("tmp3.png");
    }

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  } 
  return 0;
}

///\endcond

