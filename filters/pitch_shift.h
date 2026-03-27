#ifndef FILTERS_PITCH_SHIFT_H
#define FILTERS_PITCH_SHIFT_H

#include "util_filter_base.h"

float pitch_shift(struct filter *self, float x);

struct filter *init_filter_pitch_shift(float uni);

#endif
