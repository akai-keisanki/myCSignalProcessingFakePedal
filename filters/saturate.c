#include "saturate.h"

#include "multiply.h"

#include <math.h>

float saturate(struct filter *self, float x)
{
  float mix = get_param(self, 1);

  float y = multiply(self, tanhf(multiply(self, x)));

  return (y - x) * mix + x;
}

struct filter *init_filter_saturate(float mul, float mix)
{
  return init_filter("saturate", saturate, (params_t){mul, mix}, (params_t){}, 0);
}
