#include "bit_crush.h"

#include <math.h>

float bit_crush(struct filter *self, float x)
{
  float res = get_param(self, 0);

  if (res >= 64.0f) return x;

  float steps = powf(2.0f, res);
  steps /= 2.0f;

  float y = round(x * steps) / steps;

  return y;
}

struct filter *init_filter_bit_crush(float res)
{
  return init_filter("bit_crush", bit_crush, (params_t){res}, (params_t){}, 0);
}
