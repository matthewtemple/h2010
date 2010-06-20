#include "h/external/external.h"
#include "h/math/tools.h"

double h_math_tools_calculate_distance(double x1, double y1, double x2,
    double y2)
{
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

h_core_bool_t h_math_tools_equal_double(double a, double b)
{
  h_core_bool_t equal;

  if (abs(a - b) < 0.0000001) {
    equal = h_core_bool_true;
  } else {
    equal = h_core_bool_false;
  }

  return equal;
}
