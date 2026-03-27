#include "drive.h"

#include "clip.h"

float drive(struct filter *self, float x)
{
  float mix = get_param(self, 1);

  return clip(self, x) * mix + x * (1.0f - mix);
}

struct filter *init_filter_drive(float thr, float mix)
{
  return init_filter("drive", drive, (params_t){thr, mix}, (params_t){}, 0);
}
