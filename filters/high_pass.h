#ifndef FILTERS_HIGH_PASS_H
#define FILTERS_HIGH_PASS_H

#include "util_filter_base.h"

float high_pass(struct filter *self, float x);

struct filter *init_filter_high_pass(float cut);

#endif
