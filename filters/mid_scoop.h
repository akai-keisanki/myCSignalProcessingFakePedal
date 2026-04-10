#ifndef FILTERS_MID_SCOOP_H
#define FILTERS_MID_SCOOP_H

#include "util_filter_base.h"

float mid_scoop(struct filter *self, float x);

struct filter *init_filter_mid_scoop(float low_cut, float high_cut, float mix);

#endif
