#include "high_pass.h"

#include "low_pass.h"

float high_pass(struct filter *self, float x)
{
  float cut = get_param(self, 0);
  float mix = get_param(self, 1);

  set_param(self, 1, 1.0f);

  float lp_y = low_pass(self, x);

  set_param(self, 1, cut);

  float y = x - lp_y;

  return (y - x) * mix + x;
}

struct filter *init_filter_high_pass(float cut, float mix)
{
  return init_filter("high_pass", high_pass, (params_t){cut, mix}, (params_t){0.0f}, 0);
}
