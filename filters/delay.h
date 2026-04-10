#ifndef FILTERS_DELAY_H
#define FILTERS_DELAY_H

#include "util_filter_base.h"

float delay(struct filter *self, float x);

struct filter *init_filter_delay(float dly, float fdb, float mix);

#endif
