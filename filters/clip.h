#ifndef FILTERS_CLIP_H
#define FILTERS_CLIP_H

#include "util_filter_base.h"

float clip(struct filter *self, float x);

struct filter *init_filter_clip(float thr);

#endif
