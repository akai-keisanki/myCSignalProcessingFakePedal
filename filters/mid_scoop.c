#include "mid_scoop.h"

#include "low_pass.h"
#include "high_pass.h"

float mid_scoop(struct filter *self, float x)
{
  float low_cut = get_param(self, 0);
  float high_cut = get_param(self, 1);
  float mix = get_param(self, 2);

  set_param(self, 1, 1.0f);

  float y = high_pass(self, x);

  float p_hp_y = get_static_val(self, 0);
  float p_lp_y = get_static_val(self, 1);

  set_param(self, 0, high_cut);
  set_static_val(self, 0, p_lp_y);

  y += low_pass(self, x);

  p_lp_y = get_static_val(self, 0);

  set_param(self, 0, low_cut);
  set_param(self, 1, high_cut);
  set_static_val(self, 0, p_hp_y);
  set_static_val(self, 1, p_lp_y);

  return (y - x) * mix + x;
}

struct filter *init_filter_mid_scoop(float low_cut, float high_cut, float mix)
{
  return init_filter("mid_scoop", mid_scoop, (params_t){low_cut, high_cut, mix}, (params_t){0.0f, 0.0f}, 0);
}
