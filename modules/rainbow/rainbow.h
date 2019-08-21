#ifndef RAINBOW_H
#define RAINBOW_H
#include <vector>

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
