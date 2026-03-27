#include "low_pass.h"

float low_pass(struct filter *self, float x)
{
  float cut = get_param(self, 0);
  float p_y = get_static_val(self, 0);

  float y = p_y + (x - p_y) * cut;
  p_y = y;

  set_static_val(self, 0, p_y);

  return y;
}

struct filter *init_filter_low_pass(float cut)
{
  return init_filter("low_pass", low_pass, (params_t){cut}, (params_t){0.0f}, 0);
}
