#ifndef h_core_color_h
#define h_core_color_h

struct h_core_color_t {
  unsigned long red;
  unsigned long blue;
  unsigned long green;
};
typedef struct h_core_color_t h_core_color_t;

void h_core_color_init(h_core_color_t *color, unsigned long red,
    unsigned long blue, unsigned long green);

#endif
