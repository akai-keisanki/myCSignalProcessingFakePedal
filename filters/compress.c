#include "compress.h"

#include <math.h>

float compress(struct filter *self, float x)
{
  float atk = get_param(self, 0);
  float mix = get_param(self, 1);
  float env = get_static_val(self, 0);

  env += (fabs(x) - env) * atk;

  float gain = powf(tanh(0.1f / (env + 0.05f)), 2.0f) * 4.0f;

  set_static_val(self, 0, env);

  return x * gain * mix + x * (1.0f - mix);
}

struct filter *init_filter_compress(float atk, float mix)
{
  return init_filter("compress", compress, (params_t){atk, mix}, (params_t){0.0f}, 0);
}
