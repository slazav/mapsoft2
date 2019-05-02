#ifndef RAINBOW_H
#define RAINBOW_H
#include <vector>

/* rainbow -- convert double values into RGB color gradients.

In the most universal interface to convert a double value into RBG color
gradient a table which maps values into colors is used. It is
`std::vector<rainbow_data>` where `rainbow_data` has two fiels, `v` for
value and `c` for the color. The table must be sorted by `v`, either
increasing or decreasing.

* `Rainbow(const std::vector<rainbow_data> & RD);`

Another constructor makes a color gradient using a string: characters in
the string: `R`, `G`, `B` for red, green, and blue; `C`, `M`, `Y` for
cyan, magenta, and yellow; `W` and `K` for white and black, `r`, `g`,
`b`, `c`, `m`, `y`, `w` for darker colors. Other characters are ignored.
Values `min` and `max` are data values which correspond to the first
and last color. If `max` > `min` the gradient will be reversed.

* `Rainbow(double min, double max, const char *colors);`

Some predefined gradients can be set by following constructor:

* `Rainbow(double min, double max, rainbow_type type=RAINBOW_NORMAL);`

where type can be:
- `RAINBOW_NORMAL`   -- , B-C-G-Y-R-M gradient
- `RAINBOW_BURNING`  -- W-Y-R-M-B-b gradient
- `RAINBOW_BURNING1` -- K-R-Y-W gradient

One more constructor makes a simple two-color gradient:

* `Rainbow(double min, double max, int cmin, int cmax);`

Method `get(v)` converts value to a color. Example:
```c++
std::vector<rainbow_data> RD = {
  {0.1, 0x000000},
  {0.5, 0xFF0000}, // 0.1 - 0.5 black -> blue
  {0.5, 0xFF00FF}, // - color step
  {0.9, 0x000000}, // 0.5 - 0.9 magenta -> black
};
Rainbow R(RD);

int color = R.get(v); // get color for v!
```

Method set_limits(c1,c2) sets colors for data values which are
below minimum and above maximum value in the conversion table;
Use -1 to set default color which correspond to min/max in the table.

In addition there is `color_shade` function for shading colors:
`int c=color_shade(c, 0.2);`

*/
///@{

/// rainbow data type
struct rainbow_data{
  double v; ///< value
  int c;    ///< color
};

int color_shade(int c, double k);

#define RAINBOW_NORMAL   "BCGYRM"
#define RAINBOW_BURNING  "WYRMBb"
#define RAINBOW_BURNING1 "KRYW"

/*******************/

/** Class for simple gradients
*/
class Rainbow{
  // Original rainbow data.
  std::vector<rainbow_data> RD;

  // Additional data for speed-up get() method.
  // Use update_data() to update it after changing RD.
  std::vector<double> vv;
  std::vector<int> cc,rr,gg,bb;
  int N, dir; // data size, data order(+/-1)
  double lv, hv; // min/max data value
  int lc, hc; // color for data below lv and above hv

  void update_data(); ///< update additional data. should be run after changing RD
  void set_color_string(double min, double max, const char *colors);
  void set_range(double min, double max); ///<reset value range for same colors

public:
  Rainbow(const std::vector<rainbow_data> & RD_);
  Rainbow(double min, double max, const char *colors = RAINBOW_NORMAL);
  Rainbow(double min, double max, int cmin, int cmax);

  int get(double val) const;
  void set_limits(int low_c = -1, int high_c = -1);
};

#endif
