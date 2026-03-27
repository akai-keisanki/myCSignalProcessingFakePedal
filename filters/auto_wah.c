#include "auto_wah.h"

#include <math.h>

float auto_wah(struct filter *self, float x)
{
  float atk = get_param(self, 0);
  float p_y = get_static_val(self, 0);
  float env = get_static_val(self, 1);

  env += (fabs(x) - env) * atk;
  env = fminf(env, 1.0f);

  float y = p_y + (x - p_y) * powf(env, 1.5f);
  p_y = y;

  set_static_val(self, 0, p_y);
  set_static_val(self, 1, env);

  return y;
}

struct filter *init_filter_auto_wah(float atk)
{
  return init_filter("auto_wah", auto_wah, (params_t){atk}, (params_t){0.0f, 0.0f}, 0);
}
