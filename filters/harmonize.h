#ifndef FILTERS_HARMONIZE_H
#define FILTERS_HARMONIZE_H

#include "util_filter_base.h"

float harmonize(struct filter *self, float x);

struct filter *init_filter_harmonize(float uni, float mix);

#endif
