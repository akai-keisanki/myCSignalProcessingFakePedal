#include "clip.h"

float clip(struct filter *self, float x)
{
  float thr = get_param(self, 0);

  if (x > thr) return thr;
  else if (x < -thr) return -thr;
  else return x;
}

struct filter *init_filter_clip(float thr)
{
  return init_filter("clip", clip, (params_t){thr}, 0);
}
