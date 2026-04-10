#ifndef FILTERS_BIT_CRUSH_H
#define FILTERS_BIT_CRUSH_H

#include "util_filter_base.h"

float bit_crush(struct filter *self, float x);

struct filter *init_filter_bit_crush(float res, float mix);

#endif
