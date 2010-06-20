#include "h/core/color.h"
#include "h/external/external.h"

void h_core_color_init(h_core_color_t *color, unsigned long red,
    unsigned long blue, unsigned long green)
{
  assert(color);

  color->red = red;
  color->green = green;
  color->blue = blue;
}
