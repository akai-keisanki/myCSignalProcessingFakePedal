#include "harmonize.h"

#include <math.h>

#include "util_time_recorder.h"

float harmonize(struct filter *self, float x)
{
  float mix = get_param(self, 1);

  return x + pitch_shift(self, x) * mix;
}

struct filter *init_filter_harmonize(float uni, float mix)
{
  return init_filter("harmonize", harmonize, (params_t){uni, mix}, (params_t){0.0f, 4999.0f}, 5000);
}

