#ifndef FILTERS_AUTO_WAH_H
#define FILTERS_AUTO_WAH_H

#include "util_filter_base.h"

float auto_wah(struct filter *self, float x);

struct filter *init_filter_auto_wah(float atk);

#endif
