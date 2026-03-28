#ifndef FILTERS_MULTIPLY_H
#define FILTERS_MULTIPLY_H

#include "util_filter_base.h"

float multiply(struct filter *self, float x);

struct filter *init_filter_multiply(float mul);

#endif
