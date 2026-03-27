#include "saturate.h"

#include <math.h>

float saturate(struct filter *self, float x)
{
  float mul = get_param(self, 0);

  mul *= 100.0f;

  return tanhf(x*mul)*mul;
}

struct filter *init_filter_saturate(float mul);
{
  return init_filter("saturate", saturate, (params_t){mul}, (params_t){}, 0);
}
