#ifndef FILTERS_SATURATE_H
#define FILTERS_SATURATE_H

#include "util_filter_base.h"

float saturate(struct filter *self, float x);

struct filter *init_filter_saturate(float mul, float mix);

#endif
