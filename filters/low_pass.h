#ifndef FILTERS_LOW_PASS_H
#define FILTERS_LOW_PASS_H

#include "util_filter_base.h"

float low_pass(struct filter *self, float x);

struct filter *init_filter_low_pass(float cut);

#endif
