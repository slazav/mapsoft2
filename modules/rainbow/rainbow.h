#ifndef RAINBOW_H
#define RAINBOW_H
#include <vector>

/* rainbow -- convert double values into color gradients.

The most universal function is `get_rainbow`. It takes a value `val`,
an array `RD` which maps values into colors (0xRRGGBB), array size
`rd_size` and optionally colors for small and high values, `low_c`
and `high_c`.
```c++
int get_rainbow(double val, const std::vector<rainbow_data> & RD,
                int low_c = -1, int high_c = -1);
```

Vector `RD` contains records of `rainbow_data` type which are pairs of
double and integer value. Array must be sorted by double values,
increasing or decreasing.

Function returns interpolated value of integer color. If value is outside
boundaries of `RD` then boundary values are used unless `low_c` or `high_c`
are set to some positive value.

Example:
```c++
std::vector<rainbow_data> RD={
  {0.1, 0x000000},
  {0.5, 0xFF0000}, // 0.1 - 0.5 black -> blue
  {0.5, 0xFF00FF}, // - color step
  {0.9, 0x000000}, // 0.5 - 0.9 magenta -> black
};

int c1=get_rainbow(v, RD, RDS); // get color for v!
```

Simpler interface is provided by `simple_rainbow` class:
it can creates a few types of predefined gradients between
some values `v1` and `v2`. For reverse gradient direction
use v1>v2:

`simple_rainbow(v1, v2, type);`
where type can be:
* `RAINBOW_NORMAL`   -- usual rainbow, b-c-g-y-r-m
* `RAINBOW_BURNING`  -- white-y-r-m-b-dark blue gradient
* `RAINBOW_BURNING1` -- k-r-y-w

Another constructor makes a simple two-color gradient:
`simple_rainbow(v1, v2, c1, c2);`

The class provides method `get` for converting value to color:
int get(double v, int low_c=-1, int high_c=-1);

In addition there is `color_shade` function for shading colors:
int c=color_shade(c, 0.2);

*/
///@{

/// rainbow data type
struct rainbow_data{
  double v; ///< value
  int c;    ///< color
};


int get_rainbow(double val, const std::vector<rainbow_data> & RD,
                int low_c = -1, int high_c = -1);

int color_shade(int c, double k);

/// predefined gradient types for simple_rainbow class
enum rainbow_type{
  RAINBOW_NORMAL,   ///< usual rainbow, b-c-g-y-r-m
  RAINBOW_BURNING,  ///< white-y-r-m-b-darkblue gradient
  RAINBOW_BURNING1  ///< k-r-y-w
};

/*******************/

/** Class for simple gradients
*/
class simple_rainbow{
  std::vector<rainbow_data> RD;

public:
  simple_rainbow(double min, double max, rainbow_type type=RAINBOW_NORMAL);
  simple_rainbow(double min, double max, int cmin, int cmax);

  void set_range(double min, double max); ///<reset range
  double get_min() const; ///< get min value which was set in constructor
  double get_max() const; ///< get min value which was set in constructor

  int get(double val, int low_c = -1, int high_c = -1) const;

  const rainbow_data * get_data() const; ///< get pointer to the color table
  int get_size() const; ///< get color table size
};

#endif
