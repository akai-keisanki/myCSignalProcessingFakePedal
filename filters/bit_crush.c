#include "bit_crush.h"

#include <math.h>

float bit_crush(struct filter *self, float x)
{
  float res = get_param(self, 0);
  float mix = get_param(self, 1);

  if (res >= 64.0f) return x;

  float steps = powf(2.0f, res);
  steps /= 2.0f;

  float y = round(x * steps) / steps;

  return (y - x) * mix + x;
}

struct filter *init_filter_bit_crush(float res, float mix)
{
  return init_filter("bit_crush", bit_crush, (params_t){res, mix}, (params_t){}, 0);
}
