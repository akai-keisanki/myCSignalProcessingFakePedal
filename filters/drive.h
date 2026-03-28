#ifndef FILTERS_DRIVE_H
#define FILTERS_DRIVE_H

#include "util_filter_base.h"

float drive(struct filter *self, float x);

struct filter *init_filter_drive(float thr, float mix);

#endif
