#ifndef FILTERS_COMPRESS_H
#define FILTERS_COMPRESS_H

#include "util_filter_base.h"

float compress(struct filter *self, float x);

struct filter *init_filter_compress(float atk, float mix);

#endif
