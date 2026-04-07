#include "compress.h"

#include <math.h>

float compress(struct filter *self, float x)
{
  float rat = get_param(self, 0);
  float thr_db = 20.0f * log10f(get_param(self, 1));
  float atk = get_param(self, 2);
  float mix = get_param(self, 3);
  float env = get_static_val(self, 0);

  env += (fabs(x) - env) * atk;
  if (env < 1e-6f) env = 1e-6f;

  float env_db = 20.0f * log10f(env);
  float gain_db = 0.0f;
  float gain;

  if (rat > 0.0f)
    if (env_db > thr_db)
      gain_db = (thr_db + (env_db - thr_db) / rat) - env_db;
  else if (rat < 0.0f)
    if (env_db < thr_db)
      gain_db = (thr_db - (thr_db - env_db) / -rat) - env_db;

  gain = powf(10.0f, gain_db / 20.0f);

  set_static_val(self, 0, env);

  return x * gain * mix + x * (1.0f - mix);
}

struct filter *init_filter_compress(float rat, float thr, float atk, float mix)
{
  return init_filter("compress", compress, (params_t){rat, thr, atk, mix}, (params_t){0.0f}, 0);
}
